#pragma once

#include <Arduino.h>
#include <IRremote.h>

class SmartIR {
   
  public:
    SmartIR(byte pin, unsigned int long_press_delay_ms = 250);
  public:
    // This has to be run in setup stage, not in globals
    void start();
    // Return received data or 0 if there is none of it.
    unsigned long recv();
    
  private:
    IRrecv *m_receiver;
    decode_results m_results;
    unsigned int m_lpo;
    unsigned long m_last_val;
    unsigned long m_press_ts;
};
