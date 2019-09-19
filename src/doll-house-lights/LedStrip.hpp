#pragma once

#include <Arduino.h>
#include <Adafruit_DotStar.h>

#include "Rainbow.hpp"

namespace ledstrip {

// FIXME: Implementation to cpp file

const auto HSV_MAX_S = 255;
const auto HSV_MAX_V = 255;

// Adjustment coefficients for H/S/V values
const auto h_step = 500;
const auto s_rel_step = 0.15;
const auto s_min_step = 2;
const auto v_rel_step = 0.2;
const auto v_min_step = 2;

enum class UpDown : int8_t {
  Down = -1,
  None = 0,
  Up = +1,
};


class LedStrip {
  public:
    LedStrip(uint8_t n_pixels, uint8_t data_pin, uint8_t clock_pin) {
      m_dotstar = new Adafruit_DotStar(n_pixels, data_pin, clock_pin, DOTSTAR_BGR);
    }

    void initialize() {
      m_dotstar->begin();
      m_dotstar->show(); // Turn off all pixels
    }

    /// Adjust HSV value of the channel
    /// For Saturation/Value you just pas the direction since the actual
    /// value diff will be calculated through a specific forumual to make
    /// sure adjustment is not too slow preception-wise.
    ///
    /// @param h_diff Hue value delta
    /// @param s_up_down Saturation value - either +1 or -1
    /// @param v_up_down Value value - either +1 or -1
    void adjust_hsv(UpDown h_dir, UpDown s_dir, UpDown v_dir) {
      int8_t s_diff, v_diff;

      m_h += static_cast<int8_t>(h_dir) * h_step; // Let it roll over around the HSV wheel

      // Making Saturation to change slow around high values and fast around low values.
      // Similar approach with Value, but the other way around.
      // This improves user experience since perception wise, color changes are not uniform
      // accross the range.
      s_diff = static_cast<int8_t>(s_dir) * max(s_rel_step * (HSV_MAX_S - m_s), s_min_step);
      v_diff = static_cast<int8_t>(v_dir) * max(v_rel_step * m_v, v_min_step);

      Serial.println(String("m_v=") + m_v + " v_diff=" + v_diff);
      Serial.println(String("m_s=") + m_s + " s_diff=" + s_diff);

      // Clamp S/V values to 0-255 range.
      // FIXME: Looks inefficient - probably some bit magic will do better?
      m_s = (s_diff > 0) ? min(m_s + s_diff, HSV_MAX_S) : max(m_s + s_diff, 0);
      m_v = (v_diff > 0) ? min(m_v + v_diff, HSV_MAX_V) : max(m_v + v_diff, 0);

      // FIXME: For smoother change effect consider applying diffs in several iterations
      // i.e. if diff is 10, call fill_color() 10 times each time with diff of 1.
      this->fill_color();
    }

    void set_color(rainbow::ColorHSV *color) {
      m_h = color->h;
      m_s = color->s;
      m_v = color->v;
      this->fill_color();
    }

    void off() {
      m_dotstar->fill(0, 0, m_dotstar->numPixels());
      m_dotstar->show();
    }

    void on() {
      this->fill_color();
    }

  private:
    Adafruit_DotStar *m_dotstar;

    // white
    uint16_t m_h = 0;
    uint8_t m_s = 0;
    uint8_t m_v = 255;

    void fill_color() {
      Serial.println(String("Setting color h=") + m_h + " s=" + m_s + " v=" + m_v);
      m_dotstar->fill(m_dotstar->ColorHSV(m_h, m_s, m_v), 0, m_dotstar->numPixels());
      m_dotstar->show();
    }
};

} // end of namespace
