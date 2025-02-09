const int switchPin = 48;  // Pin connected to the limit switch
int lastState = HIGH;      // Stores the last known state of the switch
int currentState;          // Stores the current state

void setup() {
    pinMode(switchPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    Serial.begin(9600);                // Initialize serial communication
}

void loop() {
    currentState = digitalRead(switchPin);  // Read the switch state

    // Check if the state has changed
    if (currentState != lastState) {
        if (currentState == LOW) {
            Serial.println("Limit switch PRESSED!");
        } else {
            Serial.println("Limit switch RELEASED!");
        }
        lastState = currentState;  // Update the last state
    }

    delay(50);  // Small debounce delay to prevent false triggers
}
