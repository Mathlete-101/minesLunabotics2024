#include "Actuator.hpp"


// use initalize function in sketch to confirm initialization
Actuator actuator;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  static bool remoteControl = false;

  if (!remoteControl) {
    actuator.update();
  }

  if (remoteControl) {
    actuator.remoteControl(false, false);
  }
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'w') {
      // read actuator pwm
      Serial.print("PWM: ");
      Serial.println(actuator.getPWM());          
    }
    else if (command == 'p') {
      actuator.setPWM(50);
    }
    else if (command == 'e') {
      actuator.extendForTimeMS(1500);
    }
    else if (command == 'r') {
      actuator.retractForTimeMS(1500);
    }
    else if (command == 's') {
      actuator.stop();
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
    else if (command == 'a') {
      Serial.print("<Actuator>\t");
      Serial.print("State: ");
      Serial.println(actuator.getState());
    }
  } 
}
