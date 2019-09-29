
#include "SmartIR.hpp"
#include "Keypad.hpp"
#include "LedStrip.hpp"
#include "LightManager.hpp"
#include "Rainbow.hpp"
#include "Events.hpp"

using namespace dhl;

using ledstrip::LedStrip;
using lightmanager::StripIndex;
using events::EventType;

// FIXME: Apply constexpr, const, noexcept where needed

// ##### Config #####

// Where IR receiver data pin in connected
const uint8_t ir_pin = 4;

// Clock pin is shared by all led strips
const uint8_t clock_pin = 13;

LedStrip led_strips[] = {
  // id, # of pixels, data pin, clock pin
  LedStrip(0, 4, 5, clock_pin),
  LedStrip(1, 4, 6, clock_pin),
  LedStrip(2, 4, 7, clock_pin),
  LedStrip(3, 4, 8, clock_pin),
  LedStrip(4, 4, 9, clock_pin),
  LedStrip(5, 4, 10, clock_pin),
  LedStrip(6, 40, 11, clock_pin),
  // LedStrip(7, 4, 12, clock_pin), // spare channel
};


// ##### Globals #####
smart_ir::SmartIR ir(ir_pin);
lightmanager::LightManager lightman(led_strips, sizeof(led_strips)/sizeof(LedStrip));


StripIndex get_room_index(const keypad::KeyCode room_keycode);
void handle_hsv_event(const StripIndex si, const keypad::KeyCode hsv_keycode);
bool handle_room_onoff_event(const StripIndex si, const keypad::KeyCode on_off_keycode, const bool lights_on);

void setup() {
  Serial.begin(115200);
  ir.initialize();
  lightman.initialize();
}


void loop() {
  static struct {
    StripIndex current_room;
    bool lights_on = false;
    rainbow::ColorHSV clip_color;
  } state;

  // FIXME: No need for keycode - use keypad::Keys
  keypad::KeyCode keycode = 0;

  if (keycode=ir.recv()) {

    Serial.print(String("Received key ") + String(keycode, HEX) + ". ");

    // Handlers should not mutate the state!

    const events::Event* const event  = events::keycode_to_event(static_cast<keypad::Keys>(keycode));

    if (event->get_type() == EventType::ColorSet) {
      Serial.println("It's a color event");
      lightman.set_strip_color(state.current_room, *(static_cast<const events::ColorSetEvent* const>(event)->color));
      state.lights_on = true;

    } else if (event->get_type() == EventType::HSVChange) {
      Serial.println("It's a HSV event");
      lightman.adjust_strip_hsv(state.current_room, *(static_cast<const events::HSVChangeEvent* const>(event)->shift));

    } else if (event->get_type() == EventType::Power) {
      Serial.println("It's a power event");
      lightman.is_on() ? lightman.off() : lightman.on();

    } else if (event->get_type() == EventType::ColorCopy) {
      Serial.println("It's a color copy event");
      if (lightman.is_strip_on(state.current_room)) {
        state.clip_color = lightman.get_strip_color(state.current_room);
        Serial.println(String("Copied color: ") + state.clip_color.to_String());
      } else {
        Serial.println("The strip " + String(static_cast<int8_t>(state.current_room)) + " is off. Nothing to copy");
      }

    } else if (event->get_type() == EventType::ColorPaste) {
      Serial.println("It's a color paste event");
      if (state.clip_color) {
        Serial.println(String("Pasting color: ") + state.clip_color.to_String());
        lightman.set_strip_color(state.current_room, state.clip_color);
      } else {
        Serial.println("No Color to paste");
      }

    } else if (keypad::isRoomOnOff(keycode)) {
      Serial.println("It's a room on/off key");
      state.lights_on = handle_room_onoff_event(state.current_room, keycode);

    } else if (keypad::isRoomSelection(keycode)) {
      Serial.println("It's a Room selection key");
      state.current_room = get_room_index(keycode);

    } else {
      Serial.println("Non relevant key");
    }

    delete event;
  }
}

StripIndex get_room_index(const keypad::KeyCode room_keycode) {
  switch (room_keycode) {
    case keypad::RoomBL:
      return StripIndex(0);
      break;
    case keypad::RoomBR:
      return StripIndex(1);
      break;
    case keypad::RoomML:
      return StripIndex(2);
      break;
    case keypad::RoomMR:
      return StripIndex(3);
      break;
    case keypad::RoomTL:
      return StripIndex(4);
      break;
    case keypad::RoomTR:
      return StripIndex(5);
      break;
    case keypad::RoomEave:
      return StripIndex(6);
      break;
    case keypad::RoomAll:
      return StripIndex(StripIndex::AllStrips);
      break;
    default:
      return StripIndex(StripIndex::AllStrips);
      break;
  }
}

bool handle_room_onoff_event(const StripIndex roomi, const keypad::KeyCode on_off_keycode) {
  if (on_off_keycode == keypad::RoomOn) {
    lightman.strip_on(roomi);
    return true;
  } else if (on_off_keycode == keypad::RoomOff) {
    lightman.strip_off(roomi);
    return lightman.is_on();
  }
  Serial.println(String("Unsupported room on/off key") + String(on_off_keycode, HEX));
  return lightman.is_on();
}
