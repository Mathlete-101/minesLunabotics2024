#include "DumpingBelt.hpp"


// use initalize function in sketch to confirm initialization
DumpingBelt dumpBelt;
// power is backwards at motor, fix in code

void setup() {
  // put your setup code here, to run once:
  // Initialize Serial Communication
  Serial.begin(115200);

}

void loop() {
 static bool remoteControl = false;


  if (!remoteControl) {
    dumpBelt.update();
  }
  // assumes constant inputs, must send stop command to stop motors, the motors will be left hanging otherwise
  if (remoteControl) {
    dumpBelt.remoteControl(true);
  }
  // put your main code here, to run repeatedly:
  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'w') {
      Serial.print("PWM: ");
      Serial.println(dumpBelt.getPWM());     
    }
    else if (command == 'p') {
      dumpBelt.setPWM(75);
    }
    else if (command == 'd') {
      dumpBelt.dump();
    }
    else if (command == 'r') {
      dumpBelt.rotateForLoading();
    }
    else if (command == 'l') {
      dumpBelt.rotateForLoadingMS(500);
    }
    else if (command == 's') {
      dumpBelt.stop();
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
    else if (command == 'v') {
      Serial.print("<Dumping Belt>\t");
      Serial.print("State: ");
      Serial.print(dumpBelt.getState());
      Serial.print("\tNum Loads: ");
      Serial.print(dumpBelt.getNumLoadsOnBelt());
      Serial.print("\tFull: ");
      Serial.println(dumpBelt.isFull());
    }
  }
}
