#pragma once

#include <IRremote.h>

namespace dhl {
namespace smart_ir {

class SmartIR {

  public:
    SmartIR(byte pin, unsigned int long_press_delay_ms = 250);
  public:
    // This has to be run in setup stage, not in globals
    void initialize();
    // Return received data or 0 if there is none of it.
    unsigned long recv();

  private:
    IRrecv *m_receiver;
    decode_results m_results;
    unsigned int m_lpo;
    unsigned long m_last_val;
    unsigned long m_press_ts;
};

}} // end of namespace