
#include <Encoder.h>

#define CNV_MTR_PWM 4       
#define CNV_MTR_DIR 30
#define CNV_ENC_A 18
#define CNV_ENC_B 47

// #define FWR_MTR_PWM 7        
// #define FWR_MTR_DIR 6 
// #define FWR_ENC_A 20
// #define FWR_ENC_B 27     


// State Variables
bool motorForward = false;
bool motorActive = false;
int motorSpeed = 0;         // Current speed of the motor (0-255)

Encoder myEnc1(CNV_ENC_A, CNV_ENC_B);
// Encoder myEnc2(FWR_ENC_A, FWR_ENC_B);

void setup() {

  pinMode(CNV_MTR_PWM, OUTPUT);
  pinMode(CNV_MTR_DIR, OUTPUT);
  // pinMode(FWR_MTR_PWM, OUTPUT);
  // pinMode(FWR_MTR_DIR, OUTPUT);

  // Initialize Serial Communication
  Serial.begin(115200);
  Serial.println("Enter 'd' to activate the motor, '+' to increase speed, or '-' to decrease speed.");
  digitalWrite(CNV_MTR_DIR, LOW); // Extend
  analogWrite(CNV_MTR_PWM, 0); // Extend
  // digitalWrite(FWR_MTR_DIR, LOW); // Extend
  // analogWrite(FWR_MTR_PWM, 0); // Extend
}

long encCount1  = 0;
long encCount2  = 0;

void loop() {
 long newEnc1;
//  long newEnc2;
  newEnc1 = myEnc1.read();
  // newEnc2 = myEnc2.read();
  if ((newEnc1 != encCount1)) {
    Serial.println(newEnc1);
    //Serial.print(", ");
    // Serial.println(newEnc2);
    encCount1 = newEnc1;
    // encCount2 = newEnc2;
  }
  
  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'd' && !motorActive) {
      // Activate the actuator
      Serial.println("Activating the motor...");
      if (!motorForward) {
        digitalWrite(CNV_MTR_DIR, LOW); // Extend
        // digitalWrite(FWR_MTR_DIR, LOW); // Extend
        Serial.println(" Forward");
        motorForward = true;
      
      } else {
        digitalWrite(CNV_MTR_DIR, HIGH);  // Retract
        // digitalWrite(FWR_MTR_DIR, HIGH);  // Retract
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
        // Increase motor speed
        newEnc1 = myEnc1.readAndReset();
        // newEnc2 = myEnc2.readAndReset();
    } else {
      // Stop both the actuator and the motor
      Serial.println("Stopping all motors...");
      analogWrite(CNV_MTR_PWM, 0);        // Stop motor
      // analogWrite(FWR_MTR_PWM, 0);  
      motorActive = false;
    }
  }
  

  // If motor is active, set its speed
  if (motorActive) {
    analogWrite(CNV_MTR_PWM, motorSpeed);
    // analogWrite(FWR_MTR_PWM, motorSpeed);
  }
}



