#include "SmartIR.hpp"

SmartIR::SmartIR(byte pin, unsigned int long_press_delay_ms) {
  m_receiver = new IRrecv(pin);
  m_lpo = long_press_delay_ms;
}

// This has to be run in setup stage, not in globals
void SmartIR::start() {
    m_receiver->enableIRIn();
}

unsigned long SmartIR::recv() {
  static const unsigned long IR_REPEAT_LAST_VAL = 0xFFFFFFFF;
  
  if (m_receiver->decode(&m_results)) {
    m_receiver->resume();

    if (m_results.value != IR_REPEAT_LAST_VAL) {
      m_last_val = m_results.value;
      m_press_ts = millis();
      return m_last_val;
    }
      
    // delay repeating to handle key debounce
    if (static_cast<int32_t>(millis() - m_press_ts) > m_lpo) {
      return m_last_val;
    }

  }
  return 0;
}
