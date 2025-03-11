#include "Actuator.hpp"
#include "DumpingBelt.hpp"
#include "DiggingBelt.hpp"
#include "DriveTrain.hpp"

// use initalize function in sketch to confirm initialization
Actuator actuator;
DumpingBelt dumpBelt;
DiggingBelt digBelt;
DriveTrain driveTrain;

void setup() {
  // put your setup code here, to run once:
  // Initialize Serial Communication
  Serial.begin(115200);

}

void loop() {
 static bool remoteControl = false;

  driveTrain.update();
  if (!remoteControl) {
    actuator.update();
    dumpBelt.update();
    digBelt.update();
  }
  // assumes constant inputs, must send stop command to stop motors, the motors will be left hanging otherwise
  if (remoteControl) {
    actuator.remoteControl(false, false);
    dumpBelt.remoteControl(false);
    digBelt.remoteControl(false);
    driveTrain.remoteControl(0, 0);
  }
  // put your main code here, to run repeatedly:
  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'w') {
      // read actuator pwm
      Serial.println(actuator.getPWM()); 
      Serial.println(dumpBelt.getPWM()); 
      Serial.println(digBelt.getPWM());
      Serial.println(driveTrain.getKp());
      Serial.println(driveTrain.getKi());          
    }
    else if (command == 'p') {
      actuator.setPWM(50);
      dumpBelt.setPWM(75);
      digBelt.setPWM(10);
      driveTrain.setKP(5.5);
      driveTrain.setKi(0.5);
    }
    else if (command == 'l') {
      actuator.extendForTimeMS(2500);
      dumpBelt.rotateForLoadingMS(500);
      digBelt.start();
    }
    else if (command == 's') {
      actuator.stop();
      dumpBelt.stop();
      digBelt.stop();
    }
    else if (command == 'r') {
      remoteControl = true;
    }
    else if (command == 'c') {
      remoteControl = false;
    }
    else if (command == 'v') {
      Serial.print("<Dumping Belt>\t");
      Serial.print("State: ");
      Serial.print(dumpBelt.getState());
      Serial.print("\tNum Loads: ");
      Serial.print(dumpBelt.getNumLoadsOnBelt());
      Serial.print("\tFull: ");
      Serial.println(dumpBelt.isFull());
    }
    else if (command == 'a') {
      Serial.print("<Actuator>\t");
      Serial.print("State: ");
      Serial.println(actuator.getState());
    }
    else if (command == 'd') {
      Serial.print("<Digging Belt>\t");
      Serial.print("Speed: ");
      Serial.print(digBelt.getCurrentSpeedRPM());
      Serial.print("\tState: ");
      Serial.println(digBelt.getState());
    }

  }
}
