
#include "Pins.hpp"
#include "SmartIR.hpp"
#include "Keypad.hpp"
#include "LedStrip.hpp"
#include "LightManager.hpp"
#include "Rainbow.hpp"
#include "Events.hpp"

using namespace dhl;

using ledstrip::LedStrip;
using events::EventType;

// FIXME: Apply constexpr, const, noexcept where needed

// ##### Config #####

// Where IR receiver data pin in connected
const uint8_t ir_pin = PIN_A0;

// Clock pin is shared by all led strips
const uint8_t clock_pin = PIN_D9;

LedStrip led_strips[] = {
  // id, # of pixels, data pin, clock pin
  LedStrip(0, 4, PIN_D4, clock_pin),
  LedStrip(1, 4, PIN_D5, clock_pin),
  LedStrip(2, 4, PIN_D6, clock_pin),
  LedStrip(3, 4, PIN_D7, clock_pin),
  LedStrip(4, 4, PIN_A5, clock_pin),
  LedStrip(5, 4, PIN_A4, clock_pin),
  LedStrip(6, 40, PIN_A3, clock_pin),
  // LedStrip(7, 4, PIN_A2, clock_pin), // spare channel
};


// ##### Globals #####
smart_ir::SmartIR ir(ir_pin);
lightmanager::LightManager lightman(led_strips, sizeof(led_strips)/sizeof(LedStrip));


void setup() {
  Serial.begin(115200);
  ir.initialize();
  lightman.initialize();
}


void loop() {
  static struct {
    roomie::Room current_room;
    bool lights_on = false;
    rainbow::ColorHSV clip_color;
  } state;

  // FIXME: No need for keycode - use keypad::Keys
  unsigned long keycode = 0;

  if (keycode=ir.recv()) {

    Serial.print(String("Received key ") + String(keycode, HEX) + ". ");

    // Handlers should not mutate the state!

    auto event  = events::keycode_to_event(static_cast<keypad::Keys>(keycode));

    if (event->get_type() == EventType::ColorSet) {
      Serial.println("It's a color event");
      lightman.set_room_color(state.current_room, *(static_cast<const events::ColorSetEvent* const>(event)->color));
      state.lights_on = true;

    } else if (event->get_type() == EventType::HSVChange) {
      Serial.println("It's a HSV event");
      lightman.adjust_room_hsv(state.current_room, *(static_cast<const events::HSVChangeEvent* const>(event)->shift));

    } else if (event->get_type() == EventType::Power) {
      Serial.println("It's a power event");
      lightman.is_on() ? lightman.off() : lightman.on();

    } else if (event->get_type() == EventType::ColorCopy) {
      Serial.println("It's a color copy event");
      if (lightman.is_room_on(state.current_room)) {
        state.clip_color = lightman.get_room_color(state.current_room);
        Serial.println(String("Copied color: ") + state.clip_color.to_String());
      } else {
        Serial.println("The strip " + String(static_cast<int8_t>(state.current_room)) + " is off. Nothing to copy");
      }

    } else if (event->get_type() == EventType::ColorPaste) {
      Serial.println("It's a color paste event");
      if (state.clip_color) {
        Serial.println(String("Pasting color: ") + state.clip_color.to_String());
        lightman.set_room_color(state.current_room, state.clip_color);
      } else {
        Serial.println("No Color to paste");
      }

    } else if (event->get_type() == EventType::RoomSelect) {
      Serial.println("It's a room selection event");
      state.current_room = *(static_cast<const events::RoomSelectEvent* const>(event)->room);

    } else if (event->get_type() == EventType::RoomOn) {
      Serial.println("It's a room on key");
      lightman.room_on(state.current_room);

    } else if (event->get_type() == EventType::RoomOff) {
      Serial.println("It's a room off key");
      lightman.room_off(state.current_room);

    } else {
      Serial.println("Non relevant key");
    }

    delete event;
  }
}