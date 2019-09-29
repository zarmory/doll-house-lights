#pragma once

#include "Keypad.hpp"
#include "Rainbow.hpp"
#include "Roomie.hpp"

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
    RoomOn,
    RoomOff,
    Unknown,
};

// Base class for all events
class Event;
// Translates IR keycode to concrete event possible setting the relevant payload
const Event* keycode_to_event(const Keys keycode);

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

class RoomSelectEvent : public Event {
    public:
        RoomSelectEvent(const roomie::Room* const room) : room(room) {}
    public:
        const EventType get_type() const { return EventType::RoomSelect; };
        const roomie::Room* const room;
};

class RoomOnEvent : public Event {
    public:
        const EventType get_type() const { return EventType::RoomOn; };
};

class RoomOffEvent : public Event {
    public:
        const EventType get_type() const { return EventType::RoomOff; };
};

class UnknownEvent : public Event {
    public:
        const EventType get_type() const { return EventType::Unknown; };
};


}}  // end of namespace