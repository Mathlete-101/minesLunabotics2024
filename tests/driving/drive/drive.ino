/**
 * @file find-and-seek-marker.ino
 * @author Blake Billharz and Ben Sprik
 * @brief Scan until a marker has been detected. On detection, process telemetry, go to marker, and do a circle around it.
 * @version 0.1
 * @date 2024-04-07
 * 
 */

#include <Arduino.h>
#include "utils/Robot.hpp"
#include <Streaming.h>
#include <Wire.h>


void setup() {
    
    // setup
    Serial.begin(115200);


    // main routine

    Robot rob;

    Serial << "Scanning for marker" << endl;
    rob.goForwardF(1.0);
    Serial << "Marker found" << endl;

  

}

void loop() {
    //
}