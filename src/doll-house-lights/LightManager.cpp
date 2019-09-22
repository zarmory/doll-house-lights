#include "LightManager.hpp"

namespace lightmanager {

using ledstrip::LedStrip;
using ledstrip::UpDown;

LightManager::LightManager(ledstrip::LedStrip strips[], uint8_t size) : m_strips(strips), m_size(size) {
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

void LightManager::adjust_strip_hsv(uint8_t strip_index, UpDown h_dir, UpDown s_dir, UpDown v_dir) {
  if (!this->is_on()) {
    this->reset_state();
  }
  m_strips[strip_index].adjust_hsv(h_dir, s_dir, v_dir);
  m_strip_state[strip_index] = true;
}

void LightManager::set_strip_color(uint8_t strip_index, rainbow::ColorHSV &color){
  if (!this->is_on()) {
    this->reset_state();
  }
  m_strips[strip_index].set_color(color);
  m_strip_state[strip_index] = true;
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

void LightManager::strip_on(uint8_t strip_index) {
  if (!this->is_on()) {
    this->reset_state();
  }
  m_strips[strip_index].on();
  m_strip_state[strip_index] = true;
}

void LightManager::strip_off(uint8_t strip_index) {
  m_strips[strip_index].off();
  m_strip_state[strip_index] = false;
}

bool LightManager::is_strip_on(uint8_t strip_index) {
  return m_strips[strip_index].is_on();
}

rainbow::ColorHSV LightManager::get_strip_color(uint8_t strip_index) {
  return m_strips[strip_index].get_color();
}

void LightManager::reset_state() {
  for (auto i = 0; i < m_size; i++) {
    m_strip_state[i] = false;
  }
}


} // end of namespace
