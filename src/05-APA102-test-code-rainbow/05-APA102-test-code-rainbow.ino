#include <Adafruit_DotStar.h>
#define NUMPIXELS 50 // Number of LEDs in strip
 
// Here's how to control the LEDs from any two pins:
#define DATAPIN    6
#define CLOCKPIN   7
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

unsigned long cts = 0;
int update_interval_millis = 1;
unsigned short sat = 255;
unsigned short val = 150;
unsigned long hue_base = 0;

#define DOWN 0
#define UP 1
unsigned short val_dir = UP;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello");

  strip.begin();
  strip.show();
  cts = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned int i = 0;
  uint32_t color;
  unsigned int hue;

  if ( (millis() - cts) > update_interval_millis) {
    cts = millis();
    val = (val_dir == UP) ? (val + 1) : (val - 1);
    if (val > 250) {
      val_dir = DOWN;
    }
    if (val < 80) {
      val_dir = UP;
    }
    //Serial.println("Setting val to " + String(val));
  }

  //Serial.println("Setting color at");
  //Serial.println(millis());

  hue_base += 1;
  
  for (i=0; i<NUMPIXELS; i++) {
    hue = (hue_base * 400 + i*1000) % 65534;
    //Serial.println("Hue for led " + String(i) + " is " + hue);
    color = strip.gamma32(strip.ColorHSV(hue, sat, val));
    strip.setPixelColor(i, color);
  }
  strip.show();
}
