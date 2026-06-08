#include "SSC32.h"

// Définition du membre statique
SemaphoreHandle_t SSC32::serial2Mutex = nullptr;

SSC32::SSC32(HardwareSerial& serialPort, const int* pins, int nbServos)
  : _serial(serialPort), _pins(pins), _nbServos(nbServos)
{}

void SSC32::begin(long baudrate) {
  _serial.begin(baudrate);
}

HardwareSerial& SSC32::port() {
  return _serial;
}

void SSC32::sendAllServos(const int pulses[], int travelTime) {
  // Mutex pris ET relâché dans la même fonction → deadlock impossible
  if (serial2Mutex && xSemaphoreTake(serial2Mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    for (int i = 0; i < _nbServos; i++) {
      _serial.print("#");
      _serial.print(_pins[i]);
      _serial.print(" P");
      _serial.print(pulses[i]);
      _serial.print(" ");
    }
    _serial.print("T");
    _serial.println(travelTime);
    xSemaphoreGive(serial2Mutex);
  }
}