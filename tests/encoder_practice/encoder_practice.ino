

// const byte LED = 13;
// const byte BUTTON = 2;

// // Interrupt Service Routine (ISR)
// void switchPressed ()
// {
//   if (digitalRead (BUTTON) == HIGH)
//     digitalWrite (LED, HIGH);
//   else
//     digitalWrite (LED, LOW);
// }  // end of switchPressed

// void setup ()
// {
//   pinMode (LED, OUTPUT);  // so we can update the LED
//   digitalWrite (BUTTON, HIGH);  // internal pull-up resistor
//   attachInterrupt (digitalPinToInterrupt (BUTTON), switchPressed, CHANGE);  // attach interrupt handler
// }  // end of setup

// void loop ()
// {
//   // loop doing nothing 
// } 






int a_clk = 4;
int b_dt = 7;
// int led = 13;
// int button = 12;

// void switch_pressed() {
//   if (digitalRead(button) == HIGH) {
//     digitalWrite(led, HIGH);
//   }
//   else {
//     digitalWrite(led, LOW);
//   }
// }

void setup() {
  // put your setup code here, to run once:
  pinMode(a_clk, INPUT);
  pinMode(b_dt, INPUT);
  // pinMode(led, OUTPUT);
  // digitalWrite(button, HIGH); // internal pull up resistor
  // attachInterrupt(digitalPinToInterrupt(button), switch_pressed, CHANGE);
  Serial.begin(9600);
}

int lastVal = (digitalRead(a_clk) << 1) |  digitalRead(b_dt);
int lastA = digitalRead(a_clk);
int lastB = digitalRead(b_dt);
int printCount = 0;

long cw_count = 0;
long ccw_count = 0;

void loop() {
  //put your main code here, to run repeatedly:
  // int currVal = (digitalRead(a_clk) << 1) | digitalRead(b_dt);
  
  // if (lastVal != currVal) {
  //   switch(lastVal) {
  //     case 0b00:
  //       if (currVal == 0b10) {
  //         cw_count += 1;
  //       }
  //       else if (currVal == 0b01) {
  //         ccw_count += 1;
  //       }
  //       break;
      
  //     case 0b01:
  //       if (currVal == 0b00) {
  //         cw_count += 1;
  //       }
  //       else if (currVal == 0b11) {
  //         ccw_count += 1;
  //       }
  //       break;
      
  //     case 0b10:
  //       if (currVal == 0b11) {
  //         cw_count += 1;
  //       }
  //       else if (currVal == 0b00) {
  //         ccw_count += 1;
  //       }
  //       break;
      
  //     case 0b11:
  //       if (currVal == 0b01) {
  //         cw_count += 1;
  //       }
  //       else if (currVal == 0b10) {
  //         ccw_count += 1;
  //       }
  //       break;
      
  //     default:
  //       Serial.println("Unexpected transition");
  //       break;
  //   }

  //   lastVal = currVal;
  //   Serial.print("CW: ");
  //   Serial.print(cw_count);
  //   Serial.print("\t");
  //   Serial.print("CCW: ");
  //   Serial.println(ccw_count);
  // }

    
  int currA = digitalRead(a_clk);
  int currB = digitalRead(b_dt);
    

  if ((lastA != currA) || (lastB != currB)){
    Serial.print(currA);
    Serial.print("\t");
    Serial.println(currB);

    lastA = currA;
    lastB = currB;
    printCount += 1;
  }

  if (printCount == 4) {
    printCount = 0;
    Serial.println();
  }

}
