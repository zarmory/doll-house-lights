#pragma once

namespace keypad {

typedef uint32_t KeyCode;

enum Keys {
  Power = 0xFF02FD,

  CopyColor  = 0xFFD02F,
  PasteColor = 0xFFE01F,

  RoomOn = 0xFF3AC5,
  RoomOff = 0XFFBA45,

  // Colors
  // Numbers mean x/y button index from bottom left side
  // of the keypad color block
  Color11 = 0xFF18E7,
  Color12 = 0xFF9867,
  Color13 = 0xFF58A7,
  Color14 = 0xFFD827,

  Color21 = 0xFF38C7,
  Color22 = 0xFFB847,
  Color23 = 0xFF7887,
  Color24 = 0xFFF807,

  Color31 = 0xFF0AF5,
  Color32 = 0xFF8A75,
  Color33 = 0xFFB24D,
  Color34 = 0xFF32CD,

  Color41 = 0xFF2AD5,
  Color42 = 0xFFAA55,
  Color43 = 0xFF926D,
  Color44 = 0xFF12ED,

  Color51 = 0xFF1AE5,
  Color52 = 0xFF9A65,
  Color53 = 0xFFA25D,
  Color54 = 0xFF22DD,

  // HSV
  HSVHueUp   = 0xFF28D7,
  HSVHueDown = 0xFF08F7,
  HSVSatUp   = 0xFFA857,
  HSVSatDown = 0xFF8877,
  HSVValUp   = 0xFF6897,
  HSVValDown = 0xFF48B7,

  // Room selection
  RoomBL   = 0xFF20DF,
  RoomBR   = 0xFFA05F,
  RoomML   = 0xFF10EF,
  RoomMR   = 0xFF906F,
  RoomTL   = 0xFF30CF,
  RoomTR   = 0xFFB04F,
  RoomEave = 0xFF708F,
  RoomAll  = 0xFF609F,
};

bool isCopyPaste(KeyCode keycode) {
  switch (keycode) {
    case CopyColor:
    case PasteColor:
      return true;
      break;
    default:
      return false;
      break;
  }
}

bool isRoomOnOff(KeyCode keycode) {
  switch (keycode) {
    case RoomOn:
    case RoomOff:
      return true;
      break;
    default:
      return false;
      break;
  }
}

bool isHSV(KeyCode keycode) {
  switch (keycode) {
    case HSVHueUp:
    case HSVHueDown:
    case HSVSatUp:
    case HSVSatDown:
    case HSVValUp:
    case HSVValDown:
      return true;
      break;
    default:
      return false;
      break;
  }
}

bool isRoomSelection(KeyCode keycode) {
  switch (keycode) {
    case RoomBL:
    case RoomBR:
    case RoomML:
    case RoomMR:
    case RoomTL:
    case RoomTR:
    case RoomEave:
    case RoomAll:
      return true;
      break;
    default:
      return false;
      break;
  }
}

}  // end of namespace
