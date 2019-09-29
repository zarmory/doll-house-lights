#include "LightManager.hpp"

namespace dhl {
namespace lightmanager {

using ledstrip::LedStrip;
using roomie::Room;

LightManager::LightManager(ledstrip::LedStrip rooms[], const uint8_t size) : m_rooms(rooms), m_size(size) {
  m_active_rooms = new bool[m_size];
  for (auto i = 0; i < m_size; i++) {
    m_active_rooms[i] = m_rooms[i].is_on();
  }
}

void LightManager::initialize() {
  for (auto i = 0; i < m_size; i++) {
    m_rooms[i].initialize();
  }
}

void LightManager::adjust_room_hsv(const Room r, const rainbow::ShiftHSV &shift) {
  if (!this->is_on()) {
    this->reset_state();
  }
  // FIXME: We repease this non-trivial line 6 times. Consider refactoring
  for (auto i = ((r == Room::All) ? 0 : r); i <= ((r == Room::All) ? (m_size - 1) : r); i++) {
    m_rooms[i].adjust_hsv(shift);
    m_active_rooms[i] = true;
  }
}

void LightManager::set_room_color(const Room r, const rainbow::ColorHSV &color){
  if (!this->is_on()) {
    this->reset_state();
  }
  for (auto i = ((r == Room::All) ? 0 : r); i <= ((r == Room::All) ? (m_size - 1) : r); i++) {
    m_rooms[i].set_color(color);
    m_active_rooms[i] = true;
  }
}

void LightManager::on() {
  Serial.println("Lights on!");
  for (auto i = 0; i < m_size; i++) {
    if (m_active_rooms[i]) {
      m_rooms[i].on();
    }
  }
}

void LightManager::off() {
  Serial.println("Lights off!");
  for (auto i = 0; i < m_size; i++) {
    m_rooms[i].off();
  }
}

bool LightManager::is_on() {
  bool res = false;
  for (auto i = 0; i < m_size; i++) {
    res |= m_rooms[i].is_on();
  }
  return res;
}

void LightManager::room_on(const Room r) {
  if (!this->is_on()) {
    this->reset_state();
  }
  for (auto i = ((r == Room::All) ? 0 : r); i <= ((r == Room::All) ? (m_size - 1) : r); i++) {
    m_rooms[i].on();
    m_active_rooms[i] = true;
  }
}

void LightManager::room_off(const Room r) {
  for (auto i = ((r == Room::All) ? 0 : r); i <= ((r == Room::All) ? (m_size - 1) : r); i++) {
    m_rooms[i].off();
    m_active_rooms[i] = false;
  }
}

bool LightManager::is_room_on(const Room r) {
  if (r == Room::All) {
    return this->is_on();
  } else {
    return m_rooms[r].is_on();
  }
}

rainbow::ColorHSV LightManager::get_room_color(const Room r) {
  if (r == Room::All) {
    // get_strip_color for all strips does not really makes sense logic-wise
    // but can happen if a user wishes so.
    // Returning first led's color as a sane default - in case when all strips are of the
    // same color it actually makes sense.
    return m_rooms[0].get_color();
  } else {
    return m_rooms[r].get_color();
  }
}

void LightManager::reset_state() {
  for (auto i = 0; i < m_size; i++) {
    m_active_rooms[i] = false;
  }
}

}} // end of namespace
