import pygame
import socket
import json
import time

# Network setup
PI_IP = "192.168.0.90"  # Raspberry Pi's IP
PORT = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 2.5, 0.05
Kp = 2.5
Ki = 0.05

DEAD_ZONE = 0.15

# Max speed values
MAX_SPEED_MPS = 0.412  # Max wheel speed in m/s
MAX_ANGULAR_SPEED_RPS = (25 * 2 * 3.14159) / 60  # Convert 25 RPM to rad/s

MESSAGES_PER_SECOND = 10

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
    "A": 0,     # actuator extend
    "B": 1,     # actuator retract
    "X": 2,     #  dig belt on
    "Y": 3    # dump belt on
}

running = True  # Flag to control the main loop

last_update_time = time.time()
update_time_s = 3 # get an update every 3 seconds

update = {
        "cmd": False,
        "drive_train": {"set_linearx_mps": False, "set_angularz_rps": False, "actual_linearx_mps": True, "actual_angularz_rps": True, "get_Kp": False, "get_Ki": False, "get_state": {}},
        "actuator": {"get_state": True,"get_pwm": {}},
        "dig_belt": {"get_state": True, "get_pwm": {}, "get_speed": {}},
        "dump_belt": {"get_state": True,"get_pwm": {}, "get_loads": {}, "get_full": {}}
    }

try:
    while running:
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
            "linearx_mps": desired_speed_mps, # desired fwd speed
            "angularz_rps": desired_angular_speed_rps, # desired angular speed
            "actuator_extend": buttons["A"],
            "actuator_retract": buttons["B"],
            "dig_belt": buttons["X"],
            "dump_belt": buttons["Y"],
            "dpad": {"x": dpad_x, "y": dpad_y},
            "Kp": Kp,
            "Ki": Ki,
            # "dutyA": 75,                #actuator % duty
            # "dutyB": 80,                #dig belt % duty
            # "dutyD": 85              #dump belt % duty
        }

        # Convert to JSON and send
        json_string = json.dumps(data)
        sock.sendto(json_string.encode(), (PI_IP, PORT))
        print(json_string)

        

        # request an update from arduino 
        if (time.time() - last_update_time >= update_time_s):
            time.sleep(1 / 20) # let the arduino process the cmd before sending update
            json_string = json.dumps(update)
            sock.sendto(json_string.encode(), (PI_IP, PORT))
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
        sock.sendto(zero_data.encode(), (PI_IP, PORT))
        print("Sent stop command:", zero_data)
        time.sleep(1 / MESSAGES_PER_SECOND)

    # Clean up resources
    joystick.quit()
    pygame.quit()
    sock.close()
    print("Controller and socket closed.")
