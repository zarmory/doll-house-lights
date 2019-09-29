#pragma once

#include "Keypad.hpp"
#include "Rainbow.hpp"

namespace dhl {
namespace events {

using keypad::Keys;

enum class EventType : uint8_t {
    Power,
    ColorCopy,
    ColorPaste,
    ColorSet,
    HSVChange,
    RoomSelect,
    RoomOnOff,
    Unknown,
};

class Event {
    public:
        virtual const EventType get_type() const;
};

class PowerEvent : public Event {
    public:
        const EventType get_type() const { return EventType::Power; };
};

class ColorCopyEvent : public Event {
    public:
        const EventType get_type() const { return EventType::ColorCopy; };
};

class ColorPasteEvent : public Event {
    public:
        const EventType get_type() const { return EventType::ColorPaste; };
};

class ColorSetEvent : public Event {
    public:
        ColorSetEvent(const rainbow::ColorHSV *color) : color(color) {}
    public:
        const EventType get_type() const { return EventType::ColorSet; };
        const rainbow::ColorHSV* const color;
};

class HSVChangeEvent : public Event {
    public:
        HSVChangeEvent(const rainbow::ShiftHSV *shift) : shift(shift) {}
    public:
        const EventType get_type() const { return EventType::HSVChange; };
        const rainbow::ShiftHSV* const shift;
};

class UnknownEvent : public Event {
    public:
        const EventType get_type() const { return EventType::Unknown; };
};

const Event* keycode_to_event(const Keys keycode) {
    const rainbow::ColorHSV* const ev_color = rainbow::color_map.value(keycode);
    const rainbow::ShiftHSV* const ev_shift = rainbow::shift_map.value(keycode);

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

    } else {
        return new UnknownEvent();
    }
}


}}  // end of namespace