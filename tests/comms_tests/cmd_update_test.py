import serial
import keyboard
import json
import time


def send_command(ser):
    command = {
        "cmd": True,
        "linearx_mps": 1.5,
        "angularz_rps": 0.3,
        "buttons": {"A": 1, "B": 0, "X": 1, "Y": 1},
        "dpad": {"x": -1, "y": 1},
        "Kp": 0.75,
        "Ki": 0.02,
        "dutyA": 50,
        "dutyB": 60,
        "dutyD": 40
    }
    ser.write((json.dumps(command) + "\n").encode())
    print("Sent command")

def request_update(ser):
    update_request = {
        "cmd": False,
        "drive_train": {"set_linearx_mps": False, "set_angularz_rps": False, "actual_linearx_mps": True, "actual_angularz_rps": True, "get_Kp": False, "get_Ki": False, "get_state": {}},
        "actuator": {"get_state": True,"get_pwm": {}},
        "dig_belt": {"get_state": True, "get_pwm": {}, "get_speed": {}},
        "dump_belt": {"get_state": True,"get_pwm": {}, "get_loads": {}, "get_full": {}}
    }
    ser.write((json.dumps(update_request) + "\n").encode())

    return time.time()
    # print("Requested update")

def main():
    ser = serial.Serial('COM5', 115200, timeout=1)  # Change 'COM3' to the correct port
    print("Listening for key presses (Press 'e' to send a command, 'u' to request updates, 'q' to quit)")
    timeSent = 0
    try:
        while True:
            if keyboard.is_pressed('e'):
                send_command(ser)
                while keyboard.is_pressed('e'):  # Prevent repeated triggers
                    pass
            elif keyboard.is_pressed('u'):
                timeSent = request_update(ser)
                while keyboard.is_pressed('u'):
                    pass
            elif keyboard.is_pressed('q'):
                print("Exiting...")
                break
            if ser.in_waiting > 0:
                print(time.time() - timeSent)
                msg = ser.readline().decode('utf-8', errors='ignore').strip()
                print(msg)
    except KeyboardInterrupt:
        print("Interrupted")


if __name__ == "__main__":
    main()
