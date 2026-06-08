#pragma once
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class SSC32 {
public:
  SSC32(HardwareSerial& serialPort, const int* pins, int nbServos);
  void begin(long baudrate);
  void sendAllServos(const int pulses[], int travelTime);
  HardwareSerial& port();

  // Mutex partagé — initialisé dans setup(), utilisé ici et dans sendCameraServos()
  static SemaphoreHandle_t serial2Mutex;

private:
  HardwareSerial& _serial;
  const int*      _pins;
  int             _nbServos;
};