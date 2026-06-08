#include "InverseKine.h"
#include "SSC32.h"

// Définition des constantes
const float F = 110;  // Femur
const float T = 154;  // Tibia
const float C = 29;   // Coxa

void inverseKine(float x, float y, float z, float rotation, float alignGroundAngle, int pulses[3]) {

  float d = sqrt(x * x + y * y);
  float b = d - C;
  float h = sqrt(z * z + b * b);

  float theta1 = 180.0 + rotation - atan2(y, x) * 180.0 / PI;
  float theta2 = 180.0 + alignGroundAngle - atan2(b, -z) * 180.0 / PI - acos((F * F + h * h - T * T) / (2 * F * h)) * 180.0 / PI;
  float theta3 = acos((F * F + T * T - h * h) / (2 * F * T)) * 180.0 / PI;

  int pulseTheta1 = map(constrain(theta1, 0, 180), 0, 180, 500, 2500);
  int pulseTheta2 = map(constrain(theta2, 0, 180), 0, 180, 500, 2500);
  int pulseTheta3 = map(constrain(theta3, 0, 180), 0, 180, 500, 2500);

  pulses[0] = pulseTheta1;
  pulses[1] = pulseTheta2;
  pulses[2] = pulseTheta3;
}
