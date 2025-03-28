import serial
import time
import socket

# Network setup
PORT = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", PORT))

# Serial setup
try:
    arduino = serial.Serial("/dev/ttyACM0", 115200, timeout=5)  # Connect Arduino
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

        # Forward the received data to Arduino
        arduino.write(decoded_data.encode() + b"\n")

        # Check for any incoming data from Arduino
        if arduino.in_waiting > 0:
            arduino_data = arduino.readline().decode('utf-8', errors='ignore').strip()
            print(arduino_data)
            
            # Send Arduino response back to the original sender
            sock.sendto(arduino_data.encode(), addr)
    
    except Exception as e:
        print("Error:", e)