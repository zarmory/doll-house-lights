
#include <Adafruit_DotStar.h>

#include "SmartIR.hpp"
#include "Keypad.hpp"

// ##### Config #####
const byte IR_PIN = 4;

// ##### Main #####
SmartIR ir(IR_PIN);


void setup() {
  Serial.begin(115200);
  ir.start();
}


void loop() {
  unsigned long keycode = 0;
  if (keycode=ir.recv()) {
    Serial.print("Received key: ");
    Serial.println(keycode, HEX);

    if (keypad::isBrightness(keycode)) {
      Serial.println("It's a brightness key");
    } else if (keypad::isColor(keycode)) {
      Serial.println("It's a color key");
    } else if (keypad::isHSV(keycode)) {
      Serial.println("It's a HSV key");
    } else if (keypad::isRoom(keycode)) {
      Serial.println("It's a Room selection key");
    } else if (keypad::isCopyPaste(keycode)) {
      Serial.println("It's a copy/paste key");
    } else {
      switch(keycode) {
        case keypad::Power:
          Serial.println("It's a power key");
          break;
        default:
          Serial.println("Non relevant key");
          break;
      }
    }
  }
}
