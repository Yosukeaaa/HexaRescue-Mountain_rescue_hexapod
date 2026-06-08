#include <Arduino.h>
#include <math.h>

// UART matériel pour SSC-32U
// RX2 = GPIO16
// TX2 = GPIO17
HardwareSerial leg(2);

// Longueur des liens
const float F = 110;  // Femur
const float T = 154;  // Tibia
const float C = 29;   // Coxa

// Canaux SSC-32U
const int COXA_PIN_1 = 1;
const int FEMUR_PIN_1 = 2;
const int TIBIA_PIN_1 = 3;

const int COXA_PIN_2 = 5;
const int FEMUR_PIN_2 = 6;
const int TIBIA_PIN_2 = 7;

const int COXA_PIN_3 = 12;
const int FEMUR_PIN_3 = 13;
const int TIBIA_PIN_3 = 14;

// Patte 4
const int COXA_PIN_4 = 19;
const int FEMUR_PIN_4 = 18;
const int TIBIA_PIN_4 = 17;

// Patte 5
const int COXA_PIN_5 = 27;
const int FEMUR_PIN_5 = 26;
const int TIBIA_PIN_5 = 25;

// Patte 6
const int COXA_PIN_6 = 31;
const int FEMUR_PIN_6 = 30;
const int TIBIA_PIN_6 = 29;

void setup() {
  Serial.begin(115200);

  // UART vers SSC-32U
  leg.begin(9600, SERIAL_8N1, 16, 17);

  delay(1000);
  Serial.println("Entrez coordonnées X Y Z :");
}

void loop() {
  if (Serial.available()) {
    float x = Serial.parseFloat();
    float y = Serial.parseFloat();
    float z = Serial.parseFloat();
    delay(10);
    while (Serial.available()) Serial.read();

    // Cinématique inverse
    float d = sqrt(x * x + y * y);
    float b = d - C;
    float h = sqrt(z * z + b * b);

    float theta1 = 180.0 - atan2(y, x) * 180.0 / PI;
    float theta2 = 180.0 - atan2(b, -z) * 180.0 / PI
                   - acos((F * F + h * h - T * T) / (2 * F * h)) * 180.0 / PI;
    float theta3 = acos((F * F + T * T - h * h) / (2 * F * T)) * 180.0 / PI;

    // Affichage
    Serial.print("Theta1 (coxa): ");
    Serial.println(theta1);
    Serial.print("Theta2 (femur): ");
    Serial.println(theta2);
    Serial.print("Theta3 (tibia): ");
    Serial.println(theta3);
    Serial.println("-------------");

    // Conversion angle -> pulse (µs)
    int pulseTheta1 = map(constrain(theta1, 0, 180), 0, 180, 500, 2500);
    int pulseTheta2 = map(constrain(theta2, 0, 180), 0, 180, 500, 2500);
    int pulseTheta3 = map(constrain(theta3, 0, 180), 0, 180, 500, 2500);

    // Envoi vers SSC-32U
    leg.print("#");
    leg.print(COXA_PIN_1);
    leg.print(" P");
    leg.print(pulseTheta1);

    leg.print(" #");
    leg.print(FEMUR_PIN_1);
    leg.print(" P");
    leg.print(pulseTheta2);

    leg.print(" #");
    leg.print(TIBIA_PIN_1);
    leg.print(" P");
    leg.print(pulseTheta3);

    leg.println(" T500");

    // Envoi vers SSC-32U
    leg.print("#");
    leg.print(COXA_PIN_2);
    leg.print(" P");
    leg.print(pulseTheta1);

    leg.print(" #");
    leg.print(FEMUR_PIN_2);
    leg.print(" P");
    leg.print(pulseTheta2);

    leg.print(" #");
    leg.print(TIBIA_PIN_2);
    leg.print(" P");
    leg.print(pulseTheta3);

    leg.println(" T500");

    // Envoi vers SSC-32U
    leg.print("#");
    leg.print(COXA_PIN_3);
    leg.print(" P");
    leg.print(pulseTheta1);

    leg.print(" #");
    leg.print(FEMUR_PIN_3);
    leg.print(" P");
    leg.print(pulseTheta2);

    leg.print(" #");
    leg.print(TIBIA_PIN_3);
    leg.print(" P");
    leg.print(pulseTheta3);

    leg.println(" T500");

    // Envoi vers SSC-32U
    leg.print("#");
    leg.print(COXA_PIN_4);
    leg.print(" P");
    leg.print(pulseTheta1);

    leg.print(" #");
    leg.print(FEMUR_PIN_4);
    leg.print(" P");
    leg.print(pulseTheta2);

    leg.print(" #");
    leg.print(TIBIA_PIN_4);
    leg.print(" P");
    leg.print(pulseTheta3);

    leg.println(" T500");

    // Envoi vers SSC-32U
    leg.print("#");
    leg.print(COXA_PIN_5);
    leg.print(" P");
    leg.print(pulseTheta1);

    leg.print(" #");
    leg.print(FEMUR_PIN_5);
    leg.print(" P");
    leg.print(pulseTheta2);

    leg.print(" #");
    leg.print(TIBIA_PIN_5);
    leg.print(" P");
    leg.print(pulseTheta3);

    leg.println(" T500");

    // Envoi vers SSC-32U
    leg.print("#");
    leg.print(COXA_PIN_6);
    leg.print(" P");
    leg.print(pulseTheta1);

    leg.print(" #");
    leg.print(FEMUR_PIN_6);
    leg.print(" P");
    leg.print(pulseTheta2);

    leg.print(" #");
    leg.print(TIBIA_PIN_6);
    leg.print(" P");
    leg.print(pulseTheta3);

    leg.println(" T500");
  }
}