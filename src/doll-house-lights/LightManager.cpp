#include "LightManager.hpp"

namespace lightmanager {

using ledstrip::LedStrip;
using ledstrip::UpDown;


void LightManager::initialize() {
  for (auto i = 0; i < m_size; i++) {
    m_strips[i].initialize();
  }
}

void LightManager::adjust_strip_hsv(uint8_t strip_index, UpDown h_dir, UpDown s_dir, UpDown v_dir) {
  m_strips[strip_index].adjust_hsv(h_dir, s_dir, v_dir);
}

void LightManager::set_strip_color(uint8_t strip_index, rainbow::ColorHSV &color){
    m_strips[strip_index].set_color(color);
}

void LightManager::on() {
  for (auto i = 0; i < m_size; i++) {
    m_strips[i].on();
  }
}

void LightManager::off() {
  for (auto i = 0; i < m_size; i++) {
    m_strips[i].off();
  }
}

void LightManager::strip_on(uint8_t strip_index) {
  m_strips[strip_index].on();
}

void LightManager::strip_off(uint8_t strip_index) {
  m_strips[strip_index].off();
}

} // end of namespace
