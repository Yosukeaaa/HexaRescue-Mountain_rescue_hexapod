#ifndef LEG_H
#define LEG_H

#include <Arduino.h>


class Leg {
public:
  int id;
  int coxaPin, femurPin, tibiaPin;
  float rotation;
  float alignGroundAngle;

  Leg(int id, float rotation, float alignGroundAngle, int coxaPin, int femurPin, int tibiaPin);
};

#endif
