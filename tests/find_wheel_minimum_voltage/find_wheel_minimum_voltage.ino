

// Test the smallest voltage that makes the motor move
// Slowly increment PWM until the motor moves. PWM - 1 is voltage that does nothing

#include <CytronMotorDriver.h>

#define MOTOR_PWM 4  // Front Right Motor PWM Pin
#define MOTOR_DIR 30  // Front Right Motor Direction Pin

#define BATTERY_VOLTAGE 13.2  // Update to the actual battery voltage

CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);

int PWM = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // Wait for Serial connection
  }
  Serial.println("Arduino Ready to Receive Data...");
}

void loop() {
  static int PWM = 0;
  float unusableVoltage;

  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == '+') {
      // Increase motor speed
      PWM = constrain(PWM + 1, 0, 255);
      Serial.print("PWM: ");
      Serial.println(PWM);
    } 
    else {
      // Stop the motor and print the last usable PWM
      PWM = PWM - 1;
      Serial.println("Stopping motor...");
      unusableVoltage = BATTERY_VOLTAGE - (255.0 - (float)PWM) / 255.0 * BATTERY_VOLTAGE;
      Serial.print("PWM: ");
      Serial.print(PWM);
      Serial.print("\tUnusable Voltage: ");
      Serial.println(unusableVoltage);
      PWM = 0;
    }
  }

  // Set the motor speed
  motor.setSpeed(PWM);
}


// // test what the smallest voltage applied to all wheels makes it go
// // slowly increment PWM until the rover moves. PWM - 1 is voltage that does nothing
// #include <CytronMotorDriver.h>



// #define RIGHT_F_PWM 7
// #define RIGHT_F_DIR 6   // spins opposite way, encoders inc with current forward

// #define LEFT_F_PWM 9
// #define LEFT_F_DIR 8

// #define RIGHT_B_PWM 5
// #define RIGHT_B_DIR 16   // spins opposite way, encoders inc with current forward

// #define LEFT_B_PWM 12
// #define LEFT_B_DIR 17

// #define BATTERY_VOLTAGE 13.2  // update to reading of voltage


// // Orgainzed so index 0 = Front Right Motor, 1 = Front Left Motor, 2 = Back Right Motor, 3 = Back Left Motor

// CytronMD motors[4] = {CytronMD(PWM_DIR, RIGHT_F_PWM, RIGHT_F_DIR), 
//                       CytronMD(PWM_DIR, LEFT_F_PWM, LEFT_F_DIR), 
//                       CytronMD(PWM_DIR, RIGHT_B_PWM, RIGHT_B_DIR), 
//                       CytronMD(PWM_DIR, LEFT_B_PWM, LEFT_B_DIR)};
                    
// int PWM;


// void setup() {
//   Serial.begin(115200);
//   while (!Serial) {
//         ;  // Wait for Serial connection
//     }
//   Serial.println("Arduino Ready to Receive Data...");
// }

// void loop() {
//   static int PWM = 0;
//   int i; 
//   float unusableVoltage;

//    // Check if there is input from the Serial Monitor
//   if (Serial.available() > 0) {
//     char command = Serial.read(); // Read the input character

//     // increment the PWM by 1
//     if (command == '+') {
//       // Increase motor speed
//         PWM = constrain(PWM + 1, 0, 255);
//         Serial.print("PWM: ");
//         Serial.println(PWM);
//       }
//       // stop the process if anything else is entered
//     else {
//       // motors, print the result (last PWM value is unusable)
//       PWM = PWM - 1;
//       Serial.println("Stopping all motors...");
//       unusableVoltage = BATTERY_VOLTAGE - (255.0 - (float)PWM) / 255.0 * BATTERY_VOLTAGE;
//       Serial.print("PWM: ");
//       Serial.print(PWM);
//       Serial.print("\tUnusable Voltage: ");
//       Serial.println(unusableVoltage);
//       PWM = 0;
//     }
//   }
 
//  // set the motor speed
//   for (i=0; i<4; i++) {
//       // set PWM for motors
//       if (i == 0 || i == 2) {
//         motors[i].setSpeed(-PWM);
//       }
//       else {
//         motors[i].setSpeed(PWM);
//       }
//   }
// }














