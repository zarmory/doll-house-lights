#pragma once

namespace rainbow {

class ColorHSV {
  public:
    ColorHSV(uint16_t h, uint8_t s, uint8_t v) : h(h), s(s), v(v) {}
  public:
    const uint16_t h;
    const uint8_t s;
    const uint8_t v;

    operator String() const {
      return String("ColorHSV(h=") + h + " s=" + s + " v=" + v + ")";
    }
};
  
} // end of namespace
