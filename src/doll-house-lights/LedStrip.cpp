#include <Arduino.h>
#include <Adafruit_DotStar.h>

#include "LedStrip.hpp"
#include "Rainbow.hpp"

namespace dhl {
namespace ledstrip {

const auto HSV_MAX_S = 255;
const auto HSV_MAX_V = 255;

// Adjustment coefficients for H/S/V values
const auto h_step = 200;
const auto s_rel_step = 0.15;
const auto s_min_step = 2;
const auto v_rel_step = 0.2;
const auto v_min_step = 2;


LedStrip::LedStrip(const uint8_t id, const uint8_t n_pixels, const uint8_t data_pin, const uint8_t clock_pin) : id(id) {
  m_dotstar = new Adafruit_DotStar(n_pixels, data_pin, clock_pin, DOTSTAR_BGR);
}

void LedStrip::initialize() {
  m_dotstar->begin();
  m_dotstar->show(); // Turn off all pixels
}

void LedStrip::adjust_hsv(const rainbow::ShiftHSV &shift) {
  int8_t s_diff, v_diff;

  m_c.h += static_cast<int8_t>(shift.h) * h_step; // Let it roll over around the HSV wheel

  // Making Saturation to change slow around high values and fast around low values.
  // Similar approach with Value, but the other way around.
  // This improves user experience since perception wise, color changes are not uniform
  // accross the range.
  s_diff = static_cast<int8_t>(shift.s) * max(s_rel_step * (HSV_MAX_S - m_c.s), s_min_step);
  v_diff = static_cast<int8_t>(shift.v) * max(v_rel_step * m_c.v, v_min_step);

  // Clamp S/V values to 0-255 range.
  // FIXME: Looks inefficient - probably some bit magic will do better?
  m_c.s = (s_diff > 0) ? min(m_c.s + s_diff, HSV_MAX_S) : max(m_c.s + s_diff, 0);
  m_c.v = (v_diff > 0) ? min(m_c.v + v_diff, HSV_MAX_V) : max(m_c.v + v_diff, 0);

  // FIXME: For smoother change effect consider applying diffs in several iterations
  // i.e. if diff is 10, call fill_color() 10 times each time with diff of 1.
  this->fill_color();

  m_is_on = true;
}

void LedStrip::set_color(const rainbow::ColorHSV &color) {
  m_c = color;
  this->fill_color();
  m_is_on = true;
}

void LedStrip::off() {
  m_dotstar->fill(0, 0, m_dotstar->numPixels());
  m_dotstar->show();
  m_is_on = false;
  }

void LedStrip::on() {
  this->fill_color();
  m_is_on = true;
}

bool LedStrip::is_on() {
  return m_is_on;
}

const rainbow::ColorHSV LedStrip::get_color() {
  return m_c;
}

}} // end of namespace
