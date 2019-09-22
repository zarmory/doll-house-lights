#pragma once

#include "LedStrip.hpp"
#include "Rainbow.hpp"

namespace lightmanager {

using ledstrip::UpDown;

class StripIndex {
  public:
    enum {
      AllStrips = -1,
    };

    int index;
    StripIndex(int i = 0) : index(i) {}
    bool is_all() { return (index == AllStrips); }

    operator int() const { return index; }
};

class LightManager {
  public:
    LightManager(ledstrip::LedStrip strips[], uint8_t size);

  public:
    void initialize();
    
    void adjust_strip_hsv(StripIndex si, UpDown h_dir, UpDown s_dir, UpDown v_dir);
    void set_strip_color(StripIndex si, rainbow::ColorHSV &color);

    // Turn all strips Off / On to their last saved color
    // "all" means all those that were powered on before
    void on();
    void off();

    bool is_on();

    // Turn a strip Off / On to their last saved color
    void strip_on(StripIndex si);
    void strip_off(StripIndex si);

    bool is_strip_on(StripIndex si);

    rainbow::ColorHSV get_strip_color(StripIndex si);

  private:
    ledstrip::LedStrip *m_strips; 
    uint8_t m_size;
    bool *m_strip_state;
    bool m_is_on = false;

    void reset_state();
};

} // end of namespace
