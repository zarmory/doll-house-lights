#pragma once

#include "LedStrip.hpp"
#include "Rainbow.hpp"
#include "Roomie.hpp"

namespace dhl {
namespace lightmanager {

using roomie::Room;

class LightManager {
  public:
    LightManager(ledstrip::LedStrip rooms[], uint8_t size);

  public:
    void initialize();

    void adjust_room_hsv(const Room r, const rainbow::ShiftHSV &shift);

    void set_room_color(const Room r,  const rainbow::ColorHSV &color);
    rainbow::ColorHSV get_room_color(const Room r);

    // Turn all strips Off / On to their last saved color
    // "all" means all those that were powered on before
    void on();
    void off();

    bool is_on();

    // Turn a strip Off / On to their last saved color
    void room_on(const Room r);
    void room_off(const Room r);

    bool is_room_on(const Room r);

  private:
    ledstrip::LedStrip *m_rooms;
    const uint8_t m_size;
    bool *m_active_rooms;
    bool m_is_on = false;

    void reset_state();
};

}} // end of namespace
