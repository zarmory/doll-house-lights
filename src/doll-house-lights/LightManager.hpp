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

    // I can't define the callback C-style like
    //   void act(const Room r, void (*room_cb)(const Room r));
    // since I want to use capturing lambdas as callback functions and
    // they do not play together with C functions:
    // https://stackoverflow.com/questions/55395717/passing-lambdas-as-callbacks-to-c-functions
    //
    // Template approach is taken from here:
    // https://stackoverflow.com/a/44639290/360390
    // As far as I understand, due to captured variables, each lambda is in fact
    // a different function, hence the only way to generalize act() method is to use
    // template - it's a bit sad, since actually a new version of act() will be produced
    // for each new lambda hence it's equivalient to code duplication to a certain extent.
    // Hoefully compiler will provide a good optimization for it.
    //
    // This function is used to apply a callback either to concrete room
    // or all rooms if passed room == Room::All.
    // This saves logic durplication for applyling similar operations on either
    // particular or all rooms.
    template <typename RoomCallback>
    void act(const Room r, RoomCallback&& room_cb) {
      const auto start = (r == Room::All) ? 0 : r;
      const auto end = (r == Room::All) ? (m_size - 1) : r;
      for (auto i = start; i <= end; i++) {
        room_cb(static_cast<roomie::Room>(i));
      }
    }

};

}} // end of namespace
