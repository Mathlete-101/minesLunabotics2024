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
  // Initialize Serial Communication
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

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
    else if (command == 't') {
      if (remoteControl) {
        remoteControl = false;
      }
      else {
        remoteControl = true;
      }
      Serial.print("Remote Control: ");
      Serial.println(remoteControl);
    }
    else if (command == 'p') {
      actuator.setPWM(50);
      dumpBelt.setPWM(75);
      digBelt.setPWM(10);
    }
    else if (command == 'k') {
      driveTrain.setKp(5.5);
      driveTrain.setKi(0.5);
    }
    else if (command == 'e') {
      actuator.extendForTimeMS(1000);
    }
    else if (command == 'u') {
      actuator.retractForTimeMS(1000);
    }
    else if (command == 'd') {
      dumpBelt.dump();
    }
    else if (command == 'l') {
      dumpBelt.rotateForLoading();
    }
    else if (command == 'b') {
      digBelt.start();
    }
    else if (command == 'f') {
      driveTrain.setSpeedMS(0.3, 0, 1000);
    }
    else if (command == 'v') {
      driveTrain.setSpeedMS(-0.3, 0, 1000);
    }
    // turn right??
    else if (command == 'r') {
      driveTrain.setSpeedMS(0, 0.2, 1000);
    }
    // turn left???
    else if (command == 'c') {
      driveTrain.setSpeedMS(0, -0.2, 1000);
    }
    else if (command == '1') {
      Serial.print("<Actuator>\t");
      Serial.print("State: ");
      Serial.println(actuator.getState());
    }
    else if (command == '2') {
      Serial.print("<Dumping Belt>\t");
      Serial.print("State: ");
      Serial.print(dumpBelt.getState());
      Serial.print("\tNum Loads: ");
      Serial.print(dumpBelt.getNumLoadsOnBelt());
      Serial.print("\tFull: ");
      Serial.println(dumpBelt.isFull());
    }
    else if (command == '3') {
      Serial.print("<Digging Belt>\t");
      Serial.print("Speed: ");
      Serial.print(digBelt.getCurrentSpeedRPM());
      Serial.print("\tState: ");
      Serial.println(digBelt.getState());
    }
    else if (command == '4') {
      Serial.print("<Drive Train>\t");
      Serial.print("Set FWD: ");
      Serial.print(driveTrain.getSetFWDSpeedMPS());
      Serial.print("\tActual FWD: ");
      Serial.print(driveTrain.getActualFWDSpeedMPS());
      Serial.print("\tSet ANG: ");
      Serial.print(driveTrain.getSetANGSpeedRPS());
      Serial.print("\tActual ANG: ");
      Serial.print(driveTrain.getActualANGSpeedRPS());
      Serial.print("\tState: ");
      Serial.println(driveTrain.getState());

    }
    else {
      actuator.stop();
      dumpBelt.stop();
      digBelt.stop();
      driveTrain.stop();
    }
  }
}












