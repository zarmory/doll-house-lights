#pragma once

#include <Arduino.h>
#include <Adafruit_DotStar.h>

#include "Rainbow.hpp"

namespace dhl {
namespace ledstrip {

class LedStrip {
  public:
    LedStrip(const uint8_t id, const uint8_t n_pixels, const uint8_t data_pin, const uint8_t clock_pin);

    void initialize();

    // Adjust HSV value of the channel
    // ShiftHSV just defines directions and actuall valies
    // are determined by function's heuristics since just linear
    // changes to values result in non-linear color/light changes
    // LEDs, which is suboptimal from user-experience perspective.
    void adjust_hsv(const rainbow::ShiftHSV &shift);

    void set_color(const rainbow::ColorHSV &color);

    void off();

    void on();

    bool is_on();

    const rainbow::ColorHSV get_color();

    const uint8_t id; // For debug messages

  private:
    Adafruit_DotStar *m_dotstar;

    bool m_is_on = false;
    rainbow::ColorHSV m_c = rainbow::ColorHSV(0, 0, 0); // no light

    void fill_color() {
      Serial.println(String("Strip ") + id + " setting color to " + m_c.to_String());
      m_dotstar->fill(m_dotstar->ColorHSV(m_c.h, m_c.s, m_c.v), 0, m_dotstar->numPixels());
      m_dotstar->show();
    }
};

}} // end of namespace
