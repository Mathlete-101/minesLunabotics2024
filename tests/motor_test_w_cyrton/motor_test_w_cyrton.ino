#include <Encoder.h>
#include <CytronMotorDriver.h>

// Define motor and encoder pins

#define BWL_MTR_PWM 7        
#define BWL_MTR_DIR 6 
#define BWL_ENC_A 20
#define BWL_ENC_B 27


// Create Cytron Motor Driver object
CytronMD cnvMotor(PWM_DIR, BWL_MTR_PWM, BWL_MTR_DIR); // (Mode, PWM Pin, DIR Pin)

// State Variables
bool motorForward = false;
bool motorActive = false;
int motorSpeed = 0;         // Current speed of the motor (0-255)

// Encoder object
Encoder myEnc1(BWL_ENC_A, BWL_ENC_B);

void setup() {
    Serial.begin(115200);
    Serial.println("Enter 'd' to activate the motor, '+' to increase speed, or '-' to decrease speed.");

    // Ensure motor is stopped initially
    cnvMotor.setSpeed(0);
}

long encCount1 = 0;

void loop() {
    long newEnc1 = myEnc1.read();

    if (newEnc1 != encCount1) {
        Serial.println(newEnc1);
        encCount1 = newEnc1;
    }
    
    // Check if there is input from the Serial Monitor
    if (Serial.available() > 0) {
        char command = Serial.read(); // Read the input character

        if (command == 'd' && !motorActive) {
            // Toggle motor direction
            Serial.println("Activating the motor...");
            if (!motorForward) {
                cnvMotor.setSpeed(motorSpeed); // Move Forward
                Serial.println(" Forward");
                motorForward = true;
            } else {
                cnvMotor.setSpeed(-motorSpeed); // Move Reverse
                Serial.println(" Reverse");
                motorForward = false;
            }
            motorActive = true;            

        } else if (command == '+') {
            // Increase motor speed
            if (motorActive) {
                motorSpeed = constrain(motorSpeed + 5, 0, 255);
                Serial.print("Motor speed increased to: ");
                Serial.println(motorSpeed);
                cnvMotor.setSpeed(motorForward ? motorSpeed : -motorSpeed);
            }

        } else if (command == '-') {
            // Decrease motor speed
            if (motorActive) {
                motorSpeed = constrain(motorSpeed - 5, 0, 255);
                Serial.print("Motor speed decreased to: ");
                Serial.println(motorSpeed);
                cnvMotor.setSpeed(motorForward ? motorSpeed : -motorSpeed);
            }

        } else if (command == '0') {
            // Reset encoder count
            newEnc1 = myEnc1.readAndReset();

        } else {
            // Stop motor
            Serial.println("Stopping all motors...");
            cnvMotor.setSpeed(0);
            motorActive = false;
        }
    }
}
