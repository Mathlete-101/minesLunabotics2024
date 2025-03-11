import serial
import json
import time
import socket

# Network setup
PORT = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", PORT))

# Serial setup
try:
    arduino = serial.Serial("/dev/ttyACM0", 115200, timeout=5)   # connect Arduino to middle USB tower, bottom port
    time.sleep(2)  # Allow Arduino to initialize
    print("Connected to Arduino")
except serial.SerialException as e:
    print("Failed to connect to Arduino:", e)
    exit()

print("Listening for controller data...")

while True:
    try:
        # Receive data from socket
        data, addr = sock.recvfrom(1024)
        decoded_data = data.decode(errors='ignore').strip()

        # Forward the exact received data to Arduino
        arduino.write(decoded_data.encode() + b"\n")

        # print("Sent to Arduino:", decoded_data)

        # Check for any incoming data from Arduino and print it
        if arduino.in_waiting > 0:
            arduino_data = arduino.readline().decode('utf-8', errors='ignore').strip()
            print(arduino_data)

    except Exception as e:
        print("Error:", e)