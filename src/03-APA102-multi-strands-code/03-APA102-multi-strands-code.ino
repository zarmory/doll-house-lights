/**
 * Copyright 2019 Jim Sokoloff (jim.sokoloff@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * 
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *      
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// In Arduino 1.8.9 on MacOS, I had to remove the IRremoteTools.cpp file to compile

// Include following library to access IR sensor
#include <RobotIRremote.h>

// Connect the IR Receiver to these three pins (in order, D9, D8, D7)
int IR_GND_PIN = 9;    // GND for IR Receiver
int IR_VCC_PIN = 8;    // Vcc for IR Receiver
int IR_OUT_PIN = 7;    // Output of IR Receiver
IRrecv receiver(IR_OUT_PIN); 
decode_results results;     

#include <Adafruit_DotStar.h>

#define NUMPIXELS 50 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define CLOCKPIN1   2
#define DATAPIN1    3

#define CLOCKPIN2   2
#define DATAPIN2    4

Adafruit_DotStar strip1 = Adafruit_DotStar(NUMPIXELS, DATAPIN1, CLOCKPIN1, DOTSTAR_BGR);
Adafruit_DotStar strip2 = Adafruit_DotStar(NUMPIXELS, DATAPIN2, CLOCKPIN2, DOTSTAR_BGR);


void setup() {
  Serial.begin(115200);
  Serial.println("Hello");
  Serial.println("Use this test program to ensure your APA 102 lights are OK");
  Serial.println("Should display a Red, then Green, then Blue, then White LEDs on one strand");
  Serial.println("Should display a rainbox of LEDs on the second strand");
  Serial.println("If not, then change the DOTSTAR_XXX until it does");
  Serial.println("");
  

  pinMode(IR_VCC_PIN, OUTPUT);  // Provide power to the IR receiver
  pinMode(IR_GND_PIN, OUTPUT);
  digitalWrite(IR_VCC_PIN, LOW);
  digitalWrite(IR_GND_PIN, HIGH);
    
  receiver.enableIRIn();    //  Enable receiver so that it would start processing infrared signals

  strip1.begin(); // Initialize pins for output
  strip1.show();  // Turn all LEDs off ASAP
  strip2.begin(); // Initialize pins for output
  strip2.show();  // Turn all LEDs off ASAP

  int p = 0;
  strip1.setPixelColor(p++, 0xFF0000);  // R
  strip1.setPixelColor(p++, 0x007700);  // G - half power
  strip1.setPixelColor(p++, 0x0000FF);  // B
  strip1.setPixelColor(p++, 0x777777);  // W - half power
  strip1.setPixelColor(p++, 0x333333);  // Gray
  strip1.setPixelColor(p++, 0x222222);  // Gray
  strip1.setPixelColor(p++, 0x111111);  // Gray

  p = 0;
  strip2.setPixelColor(p++, 0xFF0000);  // R
  strip2.setPixelColor(p++, 0xFF3300);  // O
  strip2.setPixelColor(p++, 0xFFaa00);  // Y
  strip2.setPixelColor(p++, 0x007700);  // G - half power
  strip2.setPixelColor(p++, 0x0000FF);  // B
  strip2.setPixelColor(p++, 0x3b0082);  // V1
  strip2.setPixelColor(p++, 0x6422a3);  // V2
}

void loop() {
  if (receiver.decode(&results)) {            //  Decode the button code and put it in "results" variable
    if (results.value != 0xffffffff) {
      Serial.print(results.value, HEX);
      if (results.value > 0xffffff) {
        Serial.println(" (probably bad read)");
      } else {
        Serial.println(" (presumed good)");
      }
      if (results.value == 0xFFA05F) {          //  Key '1' is pressed
        Serial.println("Key 1");
      }
      if (results.value == 0xFF20DF) {          //  Key '2' is pressed
        Serial.println("Key 2");
      }
      if (results.value == 0xFF609F) {          //  Key '3' is pressed
        Serial.println("Key 3");
      }
      if (results.value == 0xFFE01F) {          //  Key '4' is pressed
         Serial.println("Key 4");
      }
    }
    receiver.resume();
  }
  strip1.show();                     // Refresh strip
  strip2.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)
}
