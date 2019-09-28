#pragma once

#include "Keypad.hpp"
#include "KVMap.hpp"
#include "Rainbow.hpp"

namespace dhl {
namespace events {

using rainbow::ColorHSV;
using rainbow::max_hue;
using rainbow::max_sat;

using keypad::Keys;
using keypad::KeyCode;

using ColorMap = KVMap<KeyCode, ColorHSV>;

const ColorMap::Entry _cmap[] = {
    // First column
    {Keys::Color51, ColorHSV(0, max_sat, 255)}, // Red
    {Keys::Color41, ColorHSV(1000, max_sat, 220)}, // Orange-ish
    {Keys::Color31, ColorHSV(2000, max_sat, 190)}, // Dark orange
    {Keys::Color21, ColorHSV(5000, max_sat, 160)},// Light orange
    {Keys::Color11, ColorHSV(max_hue/6, max_sat, 110)}, // Yellow

    // Second column
    {Keys::Color52, ColorHSV(max_hue/3, max_sat, 150)}, // Green
    {Keys::Color42, ColorHSV(max_hue/6+9000, max_sat, 150)}, // Light Green
    {Keys::Color32, ColorHSV(max_hue/6+7000, max_sat, 150)}, // Bright green
    {Keys::Color22, ColorHSV(max_hue/6+5000, max_sat, 150)}, // Yellow green
    {Keys::Color12, ColorHSV(max_hue/6+2500, max_sat, 150)}, // Cold yellow

    // Third column
    {Keys::Color53, ColorHSV(max_hue/3*2, max_sat, 255)},  // Blue
    {Keys::Color43, ColorHSV(max_hue/3*2-2000, max_sat, 200)},  // Light blue
    {Keys::Color33, ColorHSV(max_hue/3*2-5000, max_sat, 140)},  // Sky blue
    {Keys::Color23, ColorHSV(max_hue/3*2-8000, max_sat, 120)},  // Cyan color
    {Keys::Color13, ColorHSV(max_hue/3*2-13000, max_sat, 100)},  // Cold cyan

    // Fouth column
    {Keys::Color54, ColorHSV(0, 0, 255)},  // White
    {Keys::Color44, ColorHSV(max_hue/6*5+9000, max_sat, 250)},  // Pink
    {Keys::Color34, ColorHSV(max_hue/6*5+5000, max_sat, 215)},  // Cold pink
    {Keys::Color24, ColorHSV(max_hue/6*5, max_sat, 150)},  // Lilac
    {Keys::Color14, ColorHSV(max_hue/6*5-5000, max_sat, 170)},  // Light violet
};
const ColorMap color_map(_cmap, sizeof(_cmap) / sizeof(_cmap[0]));


}}  // end of namespace