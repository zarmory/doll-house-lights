#pragma once

#include <Arduino.h>
#include <Adafruit_DotStar.h>


namespace ledstrip {

// FIXME: Implementation to cpp file

class LedStrip {
  public:
    LedStrip(uint8_t n_pixels, uint8_t data_pin, uint8_t clock_pin) {
      m_dotstar = new Adafruit_DotStar(n_pixels, data_pin, clock_pin, DOTSTAR_BGR);
    }

    void initialize() {
      m_dotstar->begin();
      this->fill_color();
    }
    
    void adjust_hsv(int16_t h_diff, int8_t s_diff, int8_t v_diff) {
      m_h += h_diff;
      m_s += s_diff;
      m_v += v_diff;
      this->fill_color();
    }

  private:
    Adafruit_DotStar *m_dotstar;

    // We keep last strip color because DotStar lib does not have RBP to HSV conversion function
    // and it's not trival to implement properly - a bit of a RAM waste, but let's not optimize prematurely.
    uint16_t m_h = 0;  // red
    uint8_t m_s = 0;
    uint8_t m_v = 255;

    void fill_color() {
      m_dotstar->fill(m_dotstar->ColorHSV(m_h, m_s, m_v), 0, m_dotstar->numPixels());
      m_dotstar->show();
      Serial.println(String("Setting color h=") + m_h + " s=" + m_s + " v=" + m_v);
    }
};

} // end of namespace
