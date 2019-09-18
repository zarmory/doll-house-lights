#pragma once

#include "LedStrip.hpp"

namespace lightmanager {

class LightManager {
  public:
    LightManager(ledstrip::LedStrip strips[], uint8_t size) : m_strips(strips), m_size(size) {};

  public:
    void initialize();
    
    // FIXME: Use dedicated data types for H, S, and V
    void adjust_strip_hsv(uint8_t index, int16_t h_diff, int8_t s_diff, int8_t v_diff);

  private:
    ledstrip::LedStrip *m_strips; 
    uint8_t m_size;
};

} // end of namespace
