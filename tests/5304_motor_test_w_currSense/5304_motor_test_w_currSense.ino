#include <Encoder.h>

#define MOTOR_PWM 9         // PWM pin for the second motor
#define MOTOR_DIR 8         // Direction pin for the second motor
#define CURRENT_SENSOR_PIN A0 // Analog pin for ACS712 sensor

// Constants for the ACS712 current sensor
const float SENSOR_ZERO = 512.0;  // Midpoint voltage (assuming 5V power supply)
const float MV_PER_AMP = 66.0;   // Sensitivity (66mV per Amp for ACS712-30A)
const float VOLTAGE_REF = 5.0;   // Reference voltage for ADC (5V Arduino)

// State Variables
bool motorForward = false;
bool motorActive = false;
int motorSpeed = 0;         // Current speed of the motor (0-255)

Encoder myEnc(2, 5);

void setup() {
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(CURRENT_SENSOR_PIN, INPUT);

  // Initialize Serial Communication
  Serial.begin(115200);
  Serial.println("Enter 'd' to activate the motor, '+' to increase speed, or '-' to decrease speed.");
}

long encCount = 0;
unsigned long lastSampleTime = 0;
unsigned long lastPrintTime = 0;
float accumulatedCurrent = 0;
float sampleCount = 0;

void loop() {
  long newEnc;
  // newEnc = myEnc.read();
  // if (newEnc != encCount) {
  //   Serial.println(newEnc);
  //   encCount = newEnc;
  // }

  // Sample current every 10ms
  if (millis() - lastSampleTime >= 3) {
    accumulatedCurrent += readCurrent();
    sampleCount++;
    lastSampleTime = millis();
  }

  // Print averaged current every 500ms
  // if (millis() - lastPrintTime >= 1500) {
  if (sampleCount > 150) {
      float averageCurrent = accumulatedCurrent / sampleCount;
      float AcsValueF = (2.5 - (averageCurrent * (5.0 / 1024.0)) )/0.066;
      Serial.print("Average Current (A): ");
      Serial.println(AcsValueF, 2);
      accumulatedCurrent = 0;
      sampleCount = 0;
    }
    // lastPrintTime = millis();
 // }

  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'd' && !motorActive) {
      // Activate the actuator
      Serial.println("Activating the motor...");
      if (!motorForward) {
        digitalWrite(MOTOR_DIR, LOW); // Extend
        Serial.println(" Forward");
        motorForward = true;
      } else {
        digitalWrite(MOTOR_DIR, HIGH);  // Retract
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
      }
    } else if (command == '-') {
      // Decrease motor speed
      if (motorActive) {
        motorSpeed = constrain(motorSpeed - 5, 0, 255);
        Serial.print("Motor speed decreased to: ");
        Serial.println(motorSpeed);
      }
    } else if (command == '0') {
      // Reset encoder count
      newEnc = myEnc.readAndReset();
    } else {
      // Stop both the actuator and the motor
      Serial.println("Stopping all motors...");
      analogWrite(MOTOR_PWM, 0);        // Stop motor
      motorActive = false;
    }
  }

  // If motor is active, set its speed
  if (motorActive) {
    analogWrite(MOTOR_PWM, motorSpeed);
  }
}

float readCurrent() {
  // Read the analog value from the current sensor
  int sensorValue = analogRead(CURRENT_SENSOR_PIN);

  // // Convert the analog value to a voltage
  // float voltage = (sensorValue / 1023.0) * VOLTAGE_REF;

  // // Calculate the current in Amps
  // float current = (voltage - (SENSOR_ZERO / 1023.0) * VOLTAGE_REF) / (MV_PER_AMP / 1000.0);

  return sensorValue;
}
