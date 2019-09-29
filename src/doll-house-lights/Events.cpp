#include <Arduino.h>
#include "Events.hpp"

namespace dhl {
namespace events {

using keypad::Keys;

const Event* keycode_to_event(const Keys keycode) {
    const rainbow::ColorHSV* const ev_color = rainbow::color_map.value(keycode);
    const rainbow::ShiftHSV* const ev_shift = rainbow::shift_map.value(keycode);
    const roomie::Room* const ev_room = roomie::room_map.value(keycode);

    if (keycode == Keys::Power) {
        return new PowerEvent();

    } else if (keycode == Keys::ColorCopy) {
        return new ColorCopyEvent();

    } else if (keycode == Keys::ColorPaste) {
        return new ColorPasteEvent();

    } else if (ev_color != nullptr) {
        return new ColorSetEvent(ev_color);

    } else if (ev_shift != nullptr) {
        return new HSVChangeEvent(ev_shift);

    } else if (ev_room != nullptr) {
        return new RoomSelectEvent(ev_room);

    } else if (keycode == Keys::RoomOn) {
        return new RoomOnEvent();

    } else if (keycode == Keys::RoomOff) {
        return new RoomOffEvent();

    } else {
        return new UnknownEvent();
    }
}


}}  // end of namespace