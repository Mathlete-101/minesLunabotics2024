#include "DiggingBelt.hpp"


// use initalize function in sketch to confirm initialization

DiggingBelt digBelt;


void setup() {
  // put your setup code here, to run once:
  // Initialize Serial Communication
  Serial.begin(115200);

}

void loop() {
 static bool remoteControl = false;


  if (!remoteControl) {
    digBelt.update();
  }
  // assumes constant inputs, must send stop command to stop motors, the motors will be left hanging otherwise
  if (remoteControl) {
    digBelt.remoteControl(false);
  }
  // put your main code here, to run repeatedly:
  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'w') {
      // read actuator pwm
      Serial.print("PWM: ");
      Serial.println(digBelt.getPWM());          
    }
    else if (command == 'p') {
      digBelt.setPWM(10);
    }
    else if (command == 'l') {
      digBelt.start();
    }
    else if (command == 's') {
      digBelt.stop();
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
    else if (command == 'd') {
      Serial.print("<Digging Belt>\t");
      Serial.print("Speed: ");
      Serial.print(digBelt.getCurrentSpeedRPM());
      Serial.print("\tState: ");
      Serial.println(digBelt.getState());
    }

  }
}
