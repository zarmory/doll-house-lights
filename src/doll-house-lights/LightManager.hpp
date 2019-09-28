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

    int8_t index;
    StripIndex(int8_t i = 0) : index(i) {}
    bool is_all() const { return (index == AllStrips); }

    operator int8_t() const { return index; }
};

class LightManager {
  public:
    LightManager(ledstrip::LedStrip strips[], uint8_t size);

  public:
    void initialize();

    void adjust_strip_hsv(const StripIndex si, const UpDown h_dir, const UpDown s_dir, const UpDown v_dir);

    void set_strip_color(const StripIndex si, const rainbow::ColorHSV &color);
    rainbow::ColorHSV get_strip_color(const StripIndex si);

    // Turn all strips Off / On to their last saved color
    // "all" means all those that were powered on before
    void on();
    void off();

    bool is_on();

    // Turn a strip Off / On to their last saved color
    void strip_on(const StripIndex si);
    void strip_off(const StripIndex si);

    bool is_strip_on(const StripIndex si);





  private:
    ledstrip::LedStrip *m_strips;
    const uint8_t m_size;
    bool *m_active_strips;
    bool m_is_on = false;

    void reset_state();
};

} // end of namespace
