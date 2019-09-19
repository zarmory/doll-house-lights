#pragma once

#include "LedStrip.hpp"
#include "Rainbow.hpp"

namespace lightmanager {

using ledstrip::UpDown;

class LightManager {
  public:
    LightManager(ledstrip::LedStrip strips[], uint8_t size) : m_strips(strips), m_size(size) {};

  public:
    void initialize();
    
    void adjust_strip_hsv(uint8_t strip_index, UpDown h_dir, UpDown s_dir, UpDown v_dir);
    void set_strip_color(uint8_t strip_index, rainbow::ColorHSV &color);

    // Turn all strip On/Off to their last saved values
    void on();
    void off();

  private:
    ledstrip::LedStrip *m_strips; 
    uint8_t m_size;
};

} // end of namespace
