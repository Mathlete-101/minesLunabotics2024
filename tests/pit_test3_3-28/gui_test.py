import pygame
import json
import time
import queue
import threading
import serial  # Import the serial library
from gui import init_gui

gui_queue = queue.Queue()  # Create gui_queue outside main
running = True # define running globally before threads start.

threading.Thread(target=init_gui, args=(gui_queue,), daemon=True).start()

# Serial setup
SERIAL_PORT = 'COM5'  # Replace with your Arduino's serial port
BAUD_RATE = 115200  # Match your Arduino's baud rate
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=6)  # Initialize serial connection
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

time.sleep(2)

def receive_data():
    
    # while running:
    try:
        if ser.in_waiting > 0:  # check if there is data waiting.
            line = ser.readline().decode('utf-8').rstrip()  # read line from serial
            try:
                json_data = json.loads(line)
                gui_queue.put(json_data)  # send valid JSON to GUI
                print("Received JSON:", json_data)
            except json.JSONDecodeError:
                print("Received non-JSON data:", line)
    except Exception as e:
        print("Received error:", e)
    # end receive_data


# Initialize pygame and controller
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick detected.")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Connected to {joystick.get_name()}")

# Button mappings for Xbox controller
BUTTON_MAP = {
    "A": 0,  # actuator extend
    "B": 1,  # actuator retract
    "X": 2,  # dig belt on
    "Y": 3  # dump belt on
}

Kp = 10
Ki = 1

DEAD_ZONE = 0.1

# Max speed values
MAX_SPEED_MPS = 0.412  # Max wheel speed in m/s
MAX_ANGULAR_SPEED_RPS = (25 * 2 * 3.14159) / 60  # Convert 25 RPM to rad/s

MESSAGES_PER_SECOND = 10
UPDATE_TIME_S = 0.25  # get an update every x seconds

update = {
    "cmd": False
    # "dig_belt": {"get_state": True, "get_pwm": {}, "get_speed": {}},
}

last_update_time = time.time()

try:
    while running:

        receive_data()

        pygame.event.pump()

        # Read joystick axes
        left_x = joystick.get_axis(0)  # Left stick X
        left_y = -joystick.get_axis(1)  # Left stick Y (invert for standard controls)

        # Apply dead zone
        left_x = 0.0 if abs(left_x) < DEAD_ZONE else left_x
        left_y = 0.0 if abs(left_y) < DEAD_ZONE else left_y

        # Convert joystick input to speed commands
        desired_speed_mps = round(left_y * MAX_SPEED_MPS, 3)
        desired_angular_speed_rps = round(left_x * MAX_ANGULAR_SPEED_RPS, 3)

        # Read XYAB buttons
        buttons = {btn: joystick.get_button(BUTTON_MAP[btn]) for btn in BUTTON_MAP}

        # Read D-pad (hat switch)
        dpad_x, dpad_y = joystick.get_hat(0)

        # Create data packet (command)
        data = {
            "cmd": True,
            "linearx_mps": desired_speed_mps,  # desired fwd speed
            "angularz_rps": desired_angular_speed_rps,  # desired angular speed
            "actuator_extend": buttons["A"],
            "actuator_retract": buttons["B"],
            "dig_belt": buttons["X"],
            "dump_belt": buttons["Y"],
            "dpad": {"x": dpad_x, "y": dpad_y},
            # "Kp": Kp,
            # "Ki": Ki,
            # "dutyA": {"extend_speed": 12, "retract_speed": 43},                #actuator % duty (for extend 50, for retract 100)
            # "dutyB": 69,                #dig belt % duty (probably set value)
            # "dutyD": 42              #dump belt % duty (run as fast as possible)
        }

        # Convert to JSON and send
        json_string = json.dumps(data)
        ser.write((json_string + '\n').encode())  # send over serial with newline termination
        # print(json_string)

        # request an update from arduino
        if (time.time() - last_update_time >= UPDATE_TIME_S):
            time.sleep(1 / 20)  # let the arduino process the cmd before sending update
            json_string = json.dumps(update)
            ser.write((json_string + '\n').encode())  # send over serial with newline termination
            print(json_string)
            last_update_time = time.time()

        time.sleep(1 / MESSAGES_PER_SECOND)

        # Exit condition (check for ESC key or a specific button combo)
        for event in pygame.event.get():
            if event.type == pygame.QUIT or joystick.get_button(7):  # Button 7 is usually "Start"
                running = False

        

except KeyboardInterrupt:
    print("\nExiting...")

finally:
    # Send three zero-movement messages before quitting
    zero_data = json.dumps({
        "linearx_mps": 0.0,
        "angularz_rps": 0.0,
        "buttons": {btn: 0 for btn in BUTTON_MAP},
        "dpad": {"x": 0, "y": 0},
        "Kp": Kp,
        "Ki": Ki,
    })

    for _ in range(3):
        ser.write((zero_data + '\n').encode())  # send over serial with newline termination
        print("Sent stop command:", zero_data)
        time.sleep(1 / MESSAGES_PER_SECOND)

    # Clean up resources
    joystick.quit()
    pygame.quit()
    if ser is not None:
        ser.close()  # close the serial port
    print("Controller and serial port closed.")
# end main

# threading.Thread(target=receive_data, daemon=True).start()

