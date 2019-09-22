#pragma once

namespace rainbow {

class ColorHSV {
  public:
    ColorHSV(uint16_t h = 0 , uint8_t s = 0, uint8_t v = 0) : h(h), s(s), v(v) {}

  public:
    uint16_t h;
    uint8_t s;
    uint8_t v;

    void operator=(const ColorHSV &c) {
      h = c.h;
      s = c.s;
      v = c.v;
    }

    String to_String() {
      return String("ColorHSV(h=") + h + " s=" + s + " v=" + v + ")";
    }

    operator bool() const {
      return (h && s && v);
    }
};
  
} // end of namespace
