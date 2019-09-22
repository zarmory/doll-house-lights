#include "LightManager.hpp"

namespace lightmanager {

using ledstrip::LedStrip;
using ledstrip::UpDown;


LightManager::LightManager(ledstrip::LedStrip strips[], const uint8_t size) : m_strips(strips), m_size(size) {
  m_strip_state = new bool[m_size];
  for (auto i = 0; i < m_size; i++) {
    m_strip_state[i] = m_strips[i].is_on();
  }
}


void LightManager::initialize() {
  for (auto i = 0; i < m_size; i++) {
    m_strips[i].initialize();
  }
}

void LightManager::adjust_strip_hsv(const StripIndex si, const UpDown h_dir, const UpDown s_dir, const UpDown v_dir) {
  if (!this->is_on()) {
    this->reset_state();
  }
  // FIXME: We repease this non-trivial line 6 times. Consider refactoring
  for (auto i = (si.is_all() ? 0 : si.index); i <= (si.is_all() ? (m_size - 1) : si.index); i++) {
    m_strips[i].adjust_hsv(h_dir, s_dir, v_dir);
    m_strip_state[i] = true;
  }
}

void LightManager::set_strip_color(const StripIndex si, const rainbow::ColorHSV &color){
  if (!this->is_on()) {
    this->reset_state();
  }
  for (auto i = (si.is_all() ? 0 : si.index); i <= (si.is_all() ? (m_size - 1) : si.index); i++) {
    m_strips[i].set_color(color);
    m_strip_state[i] = true;
  }
}

void LightManager::on() {
  for (auto i = 0; i < m_size; i++) {
    if (m_strip_state[i]) {
      m_strips[i].on();
    }
  }
}

void LightManager::off() {
  for (auto i = 0; i < m_size; i++) {
    m_strips[i].off();
  }
}

bool LightManager::is_on() {
  bool res = false;
  for (auto i = 0; i < m_size; i++) {
    res |= m_strips[i].is_on();
  }
  return res;
}

void LightManager::strip_on(const StripIndex si) {
  if (!this->is_on()) {
    this->reset_state();
  }
  for (auto i = (si.is_all() ? 0 : si.index); i <= (si.is_all() ? (m_size - 1) : si.index); i++) {
    m_strips[i].on();
    m_strip_state[i] = true;
  }
}

void LightManager::strip_off(const StripIndex si) {
  for (auto i = (si.is_all() ? 0 : si.index); i <= (si.is_all() ? (m_size - 1) : si.index); i++) {
    m_strips[i].off();
    m_strip_state[i] = false;
  }
}

bool LightManager::is_strip_on(const StripIndex si) {
  if (si.is_all()) {
    return this->is_on();
  } else {
    return m_strips[si.index].is_on();
  }
}

rainbow::ColorHSV LightManager::get_strip_color(const StripIndex si) {
  if (si.is_all()) {
    // get_strip_color for all strips does not really makes sense logic-wise
    // but can happen if a user wishes so.
    // Returning first led's color as a sane default - in case when all strips are of the
    // same color it actually makes sense.
    return m_strips[0].get_color();
  } else {
    return m_strips[si.index].get_color();
  }
}

void LightManager::reset_state() {
  for (auto i = 0; i < m_size; i++) {
    m_strip_state[i] = false;
  }
}


} // end of namespace
