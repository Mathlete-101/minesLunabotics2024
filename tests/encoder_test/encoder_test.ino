/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 5);

//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(115200);
}

long encCount  = -999;


void loop() {
  
  newEnc = myEnc.read();
  if (newEnc != encCount) {
    Serial.println(newEnc);
    encCount = newEnc;
  }
  if (newEnc > 100) {
    Serial.println("Fininshed");
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    // Serial.println("long newEnc;Reset both knobs to zero");
    myEnc.write(0);
  }
}