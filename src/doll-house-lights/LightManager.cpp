#include "LightManager.hpp"
#include "LedStrip.hpp"

namespace lightmanager {

using ledstrip::LedStrip;


void LightManager::initialize() {
  for (auto i = 0; i < m_size; i++) {
    m_strips[i].initialize();
  }
}

void LightManager::adjust_strip_hsv(uint8_t index, int16_t h_diff, int8_t s_diff, int8_t v_diff) {
  m_strips[index].adjust_hsv(h_diff, s_diff, v_diff);
}

} // end of namespace
