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
  this->act(r, [&shift, this](const Room r) {
    this->m_rooms[r].adjust_hsv(shift);
    this->m_active_rooms[r] = true;
  });
}

void LightManager::set_room_color(const Room r, const rainbow::ColorHSV &color){
  if (!this->is_on()) {
    this->reset_state();
  }
  this->act(r, [&color, this](const Room r) {
    this->m_rooms[r].set_color(color);
    this->m_active_rooms[r] = true;
  });
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
  this->act(r, [this](const Room r) {
    this->m_rooms[r].on();
    this->m_active_rooms[r] = true;
  });
}

void LightManager::room_off(const Room r) {
  this->act(r, [this](const Room r) {
    this->m_rooms[r].off();
    this->m_active_rooms[r] = false;
  });
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
