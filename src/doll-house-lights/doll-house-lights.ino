
#include <Adafruit_DotStar.h>

#include "SmartIR.hpp"


// ##### Config #####
const byte IR_PIN = 4;


SmartIR ir(IR_PIN);

void setup() {
  Serial.begin(9600);
  ir.start();
}

void loop() {
  unsigned long ir_code = 0;
  if (ir_code=ir.recv()) {
    Serial.println(ir_code, HEX);
  }
}
