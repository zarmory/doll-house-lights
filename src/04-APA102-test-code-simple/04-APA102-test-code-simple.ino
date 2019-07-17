#include <Adafruit_DotStar.h>
#define NUMPIXELS 1 // Number of LEDs in strip
 
// Here's how to control the LEDs from any two pins:
#define DATAPIN    6
#define CLOCKPIN   7
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello");

  strip.begin();
  strip.show();
  strip.setPixelColor(0, 255, 255, 255);
  strip.show();

}

void loop() {
  // put your main code here, to run repeatedly:
  strip.show();
  delay(50);
}
