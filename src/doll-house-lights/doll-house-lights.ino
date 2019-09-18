
#include "SmartIR.hpp"
#include "Keypad.hpp"
#include "LedStrip.hpp"
#include "LightManager.hpp"

using ledstrip::LedStrip;

// ##### Config #####

// Where IR receiver data pin in connected
const uint8_t ir_pin = 4;

// Clock pin is shared by all led strips
const uint8_t clock_pin = 13;

LedStrip led_strips[] = {
  // # of pixels, data pin, clock pin
  LedStrip(4, 5, clock_pin),
  LedStrip(4, 6, clock_pin),
  LedStrip(4, 7, clock_pin),
  LedStrip(4, 8, clock_pin),
  LedStrip(4, 9, clock_pin),
  LedStrip(4, 10, clock_pin),
  LedStrip(4, 11, clock_pin),
  LedStrip(40, 12, clock_pin),
};

// HSV increments for each button press
const uint16_t hsv_h_step = 1000;
const uint8_t  hsv_s_step = 10;
const uint8_t  hsv_v_step = 10;


// ##### Globals #####
SmartIR ir(ir_pin);
lightmanager::LightManager lightman(led_strips, sizeof(led_strips)/sizeof(LedStrip));


void setup() {
  Serial.begin(115200);
  ir.initialize();
  lightman.initialize();
}

uint8_t room_index(keypad::KeyCode room_key_code);
void handle_hsv_event(uint8_t room_index, keypad::KeyCode hsv_keycode);

void loop() {
  static struct {
    uint8_t current_room_index = 0;
  } state;
  
  keypad::KeyCode keycode = 0;
  
  if (keycode=ir.recv()) {
  
    Serial.print("Received key: ");
    Serial.println(keycode, HEX);

    if (keypad::isBrightness(keycode)) {
      Serial.println("It's a brightness key");
    
    } else if (keypad::isColor(keycode)) {
      Serial.println("It's a color key");
    
    } else if (keypad::isHSV(keycode)) {
      Serial.println("It's a HSV key");
      handle_hsv_event(state.current_room_index, keycode);
    
    } else if (keypad::isRoom(keycode)) {
      Serial.println("It's a Room selection key");
      state.current_room_index = room_index(keycode);
    
    } else if (keypad::isCopyPaste(keycode)) {
      Serial.println("It's a copy/paste key");
    
    } else {
      switch (keycode) {
        case keypad::Power:
          Serial.println("It's a power key");
          break;
        default:
          Serial.println("Non relevant key");
          break;
      }
    }
  }
}

uint8_t room_index(keypad::KeyCode room_key_code) {
  switch (room_key_code) {
    case keypad::RoomBL:
      return 0;
      break;
    case keypad::RoomBR:
      return 1;
      break;
    case keypad::RoomML:
      return 2;
      break;
    case keypad::RoomMR:
      return 3;
      break;
    case keypad::RoomTL:
      return 4;
      break;
    case keypad::RoomTR:
      return 5;
      break;
    case keypad::RoomEave:
      return 6;
      break;
    default:
      return 0;  // FIXME: Suboptimal - needs proper error handling.
      break;
  }
}

void handle_hsv_event(uint8_t room_index, keypad::KeyCode hsv_keycode) {
  int16_t h = 0;
  int8_t s = 0, v = 0;
  
  switch (hsv_keycode) {
    case keypad::HSVHueUp:
      h = hsv_h_step;
      break;
    case keypad::HSVHueDown:
      h = -hsv_h_step;
      break;
    case keypad::HSVSatUp:
      s = hsv_s_step;
      break;
    case keypad::HSVSatDown:
      s = -hsv_s_step;
      break;
    case keypad::HSVValUp:
      v = hsv_v_step;
      break;
    case keypad::HSVValDown:
      v = -hsv_v_step;
      break;
    default:
      Serial.print("Unknown HSV key: ");
      Serial.println(hsv_keycode);
      return;
  }

  lightman.adjust_strip_hsv(room_index, h, s, v);
  Serial.println(String("Adjusting HSV for room ") + room_index + " h=" + h + " s=" + s + " v=" + v);

}
