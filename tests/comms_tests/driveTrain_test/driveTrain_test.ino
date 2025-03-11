
#include "DriveTrain.hpp"

// use initalize function in sketch to confirm initialization

DriveTrain driveTrain;

void setup() {
  // put your setup code here, to run once:
  // Initialize Serial Communication
  Serial.begin(115200);

}

void loop() {
  static bool remoteControl = false;


  // always call update for control systems
  
  driveTrain.update();
  
  // assumes constant inputs, must send stop command to stop motors, the motors will be left hanging otherwise
  if (remoteControl) {
    driveTrain.remoteControl(-0.3, 0.1);
  }
   // put your main code here, to run repeatedly:
  // Check if there is input from the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the input character

    if (command == 'w') {
      // read actuator pwm
      Serial.print("Kp: ");
      Serial.print(driveTrain.getKp());
      Serial.print("\tKi: ");
      Serial.println(driveTrain.getKi());      
    }
    else if (command == '0') {
      driveTrain.setSpeed(0,0);
    }
    else if (command == 'p') {
      driveTrain.setKp(5.5);
      driveTrain.setKi(0.5);
    }
    // front right wheel spins opposite direction that is expected, switched polarity of motors to fix
    else if (command == 'l') {
      driveTrain.setSpeedMS(-0.3, 0.0, 2500);
    }
    else if (command == 'k') {
      driveTrain.setSpeedMS(0.3, 0.0, 2500);
    }
    else if (command == 'r') {
      driveTrain.setSpeed(0.0, 0.6);
    }
    else if (command == 'o') {
      driveTrain.setSpeed(0.0, -0.6);
    }
    else if (command == 's') {
      driveTrain.stop();
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
      Serial.print("<Drive Train>\t");
      Serial.print("Set FWD: ");
      Serial.print(driveTrain.getSetFWDSpeedMPS());
      Serial.print("\tActual FWD: ");
      Serial.print(driveTrain.getActualFWDSpeedMPS()); // does not work
      Serial.print("\tSet ANG: ");
      Serial.print(driveTrain.getSetANGSpeedRPS());
      Serial.print("\tActual ANG: ");
      Serial.print(driveTrain.getActualANGSpeedRPS()); // does not work, returns nan
      Serial.print("\tState: ");
      Serial.println(driveTrain.getState());

    }

  }
}
