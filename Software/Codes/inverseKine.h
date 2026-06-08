#ifndef INVERSEKINE_H
#define INVERSEKINE_H

#include <Arduino.h>
#include "Leg.h"
#include "SSC32.h"

// Longueurs des segments
extern const float F;  // Femur
extern const float T;  // Tibia
extern const float C;  // Coxa

// Fonction d'inverse cinématique
void inverseKine(float x, float y, float z, float rotation, float alignGroundAngle, int pulses[3]);

#endif
