
#include "SmartIR.hpp"
#include "Keypad.hpp"
#include "LedStrip.hpp"
#include "LightManager.hpp"
#include "Rainbow.hpp"

using ledstrip::LedStrip;
using ledstrip::UpDown;
using lightmanager::StripIndex;

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
SmartIR ir(ir_pin);
lightmanager::LightManager lightman(led_strips, sizeof(led_strips)/sizeof(LedStrip));


StripIndex get_room_index(const keypad::KeyCode room_keycode);
void handle_hsv_event(const StripIndex si, const keypad::KeyCode hsv_keycode);
bool handle_power_event(bool lights_on);
void handle_color_event(const StripIndex si, const keypad::KeyCode color_keycode);
bool handle_room_onoff_event(const StripIndex si, const keypad::KeyCode on_off_keycode, const bool lights_on);
rainbow::ColorHSV handle_copy_paste_event(const StripIndex si, const keypad::KeyCode cp_keycode, const rainbow::ColorHSV &clip_color);

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

  keypad::KeyCode keycode = 0;

  if (keycode=ir.recv()) {

    Serial.print(String("Received key ") + String(keycode, HEX) + ". ");

    // Handlers should not mutate the state!

    if (keypad::isRoomOnOff(keycode)) {
      Serial.println("It's a room on/off key");
      state.lights_on = handle_room_onoff_event(state.current_room, keycode);

    } else if (keypad::isColor(keycode)) {
      Serial.println("It's a color key");
      handle_color_event(state.current_room, keycode);
      state.lights_on = true;

    } else if (keypad::isHSV(keycode)) {
      Serial.println("It's a HSV key");
      handle_hsv_event(state.current_room, keycode);
      state.lights_on = true;

    } else if (keypad::isRoomSelection(keycode)) {
      Serial.println("It's a Room selection key");
      state.current_room = get_room_index(keycode);

    } else if (keypad::isCopyPaste(keycode)) {
      Serial.println("It's a copy/paste key");
      state.clip_color = handle_copy_paste_event(state.current_room, keycode, state.clip_color);

    } else {
      switch (keycode) {
        case keypad::Power:
          Serial.println("It's a power key");
          state.lights_on = handle_power_event(state.lights_on);
          break;
        default:
          Serial.println("Non relevant key");
          break;
      }
    }
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

void handle_hsv_event(const StripIndex roomi, const keypad::KeyCode hsv_keycode) {
  UpDown h_dir = UpDown::None, s_dir = UpDown::None, v_dir = UpDown::None;

  switch (hsv_keycode) {
    case keypad::HSVHueUp:
      h_dir = UpDown::Up;
      break;
    case keypad::HSVHueDown:
      h_dir = UpDown::Down;
      break;
    case keypad::HSVSatUp:
      s_dir = UpDown::Up;
      break;
    case keypad::HSVSatDown:
      s_dir = UpDown::Down;
      break;
    case keypad::HSVValUp:
      v_dir = UpDown::Up;
      break;
    case keypad::HSVValDown:
      v_dir = UpDown::Down;
      break;
    default:
      Serial.print(String("Unsupported HSV key: ") + String(hsv_keycode, HEX));
      return;
  }

  Serial.println(String("Adjusting HSV for room ") + static_cast<int8_t>(roomi) +
                 " h_dir=" + static_cast<int8_t>(h_dir) +
                 " s_dir=" + static_cast<int8_t>(s_dir) +
                 " v_dir=" + static_cast<int8_t>(v_dir));

  lightman.adjust_strip_hsv(roomi, h_dir, s_dir, v_dir);

}

bool handle_power_event(bool lights_on) {
  lights_on = !lights_on;
  Serial.println(String("Lights ") + (lights_on ? "on!" : "off!"));
  lights_on ? lightman.on() : lightman.off();
  return lights_on;
}

void handle_color_event(const StripIndex roomi, const keypad::KeyCode color_keycode) {
  rainbow::ColorHSV color;
  const auto max_hue = 65535;
  const auto max_sat = 255;

  switch (color_keycode) {
    case keypad::Color51:  // Red
      color = rainbow::ColorHSV(0, max_sat, 255);
      break;
    case keypad::Color41:  // Orange-ish red
      color = rainbow::ColorHSV(1000, max_sat, 220);
      break;
    case keypad::Color31:  // Dark orange
      color = rainbow::ColorHSV(2000, max_sat, 190);
      break;
    case keypad::Color21:  // Light orange
      color = rainbow::ColorHSV(5000, max_sat, 160);
      break;
    case keypad::Color11:  // Yellow
      color = rainbow::ColorHSV(max_hue/6, max_sat, 110);
      break;

    case keypad::Color52:  // Green
      color = rainbow::ColorHSV(max_hue/3, max_sat, 150);
      break;
    case keypad::Color42:  // Light Green
      color = rainbow::ColorHSV(max_hue/6+9000, max_sat, 150);
      break;
    case keypad::Color32:  // Bright green
      color = rainbow::ColorHSV(max_hue/6+7000, max_sat, 150);
      break;
    case keypad::Color22:  // Yellow green
      color = rainbow::ColorHSV(max_hue/6+5000, max_sat, 150);
      break;
    case keypad::Color12:  // Cold yellow
      color = rainbow::ColorHSV(max_hue/6+2500, max_sat, 150);
      break;

    case keypad::Color53:  // Blue
      color = rainbow::ColorHSV(max_hue/3*2, max_sat, 255);
      break;
    case keypad::Color43:  // Light blue
      color = rainbow::ColorHSV(max_hue/3*2-2000, max_sat, 200);
      break;
    case keypad::Color33:  // Sky blue
      color = rainbow::ColorHSV(max_hue/3*2-5000, max_sat, 140);
      break;
    case keypad::Color23:  // Cyan
      color = rainbow::ColorHSV(max_hue/3*2-8000, max_sat, 120);
      break;
    case keypad::Color13:  // Cold cyan
      color = rainbow::ColorHSV(max_hue/3*2-13000, max_sat, 100);
      break;

    case keypad::Color54:  // White
      color = rainbow::ColorHSV(0, 0, 255);
      break;
    case keypad::Color44:  // Pink
      color = rainbow::ColorHSV(max_hue/6*5+9000, max_sat, 250);
      break;
    case keypad::Color34:  // Cold ping
      color = rainbow::ColorHSV(max_hue/6*5+5000, max_sat, 215);
      break;
    case keypad::Color24:  // Lilac
      color = rainbow::ColorHSV(max_hue/6*5, max_sat, 150);
      break;
    case keypad::Color14:  // Light violet
      color = rainbow::ColorHSV(max_hue/6*5-5000, max_sat, 170);
      break;

    default:
      Serial.print(String("Unsupported color key: ") + String(color_keycode, HEX));
      return;
  }

  lightman.set_strip_color(roomi, color);
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

rainbow::ColorHSV handle_copy_paste_event(const StripIndex roomi, const keypad::KeyCode cp_keycode, const rainbow::ColorHSV &clip_color) {
   if (cp_keycode == keypad::CopyColor) {
     if (!lightman.is_strip_on(roomi)) {
       Serial.println("The strip " + String(static_cast<int8_t>(roomi)) + " is not on");
       return clip_color;
     }
     Serial.println(String("Copied color: ") + clip_color.to_String());
     return lightman.get_strip_color(roomi);

   } else if (cp_keycode == keypad::PasteColor) {
      lightman.set_strip_color(roomi, clip_color);
      Serial.println(String("Pasted color: ") + clip_color.to_String());
      return clip_color;
   }

  Serial.println(String("Unsupported copy/paste key") + String(cp_keycode, HEX));
  return clip_color;
}
