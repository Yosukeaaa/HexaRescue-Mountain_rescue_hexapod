#include "Gaits.h"
#include "Leg.h"
#include "inverseKine.h"
#include "SSC32.h"

// Variables externes définies dans main
extern float groundHeight;
extern float stepHeight;
extern int velocity;
extern char command;  // mis à jour par WebSocket

int nbPts = 4;
float amplitude = 100.0;
int bodyDistance = 150;
int bodyDistanceCorners = 40;
float amplitude_turn = 50.0;

// Coordonnées pour le tripod gait forward
int coordinatestripodForwardFL[2] = { 175, 75 };
int coordinatestripodForwardFR[2] = { -175, -75 };
int coordinatestripodForwardML[2] = { 50, -50 };
int coordinatestripodForwardMR[2] = { -50, 50 };
int coordinatestripodForwardBL[2] = { -75, -175 };
int coordinatestripodForwardBR[2] = { 75, 175 };

// Coordonnées pour le tripod gait backward
int coordinatestripodBackwardFL[2] = { 75, 175 };
int coordinatestripodBackwardFR[2] = { -75, -175 };
int coordinatestripodBackwardML[2] = { -50, 50 };
int coordinatestripodBackwardMR[2] = { 50, -50 };
int coordinatestripodBackwardBL[2] = { -175, -75 };
int coordinatestripodBackwardBR[2] = { 175, 75 };

// Coordonnées pour le tripod gait left turn
int coordinatestripodLeftTurnFL[2] = { -35, 35 };
int coordinatestripodLeftTurnFR[2] = { -35, 35 };
int coordinatestripodLeftTurnML[2] = { -35, 35 };
int coordinatestripodLeftTurnMR[2] = { -35, 35 };
int coordinatestripodLeftTurnBL[2] = { -35, 35 };
int coordinatestripodLeftTurnBR[2] = { -35, 35 };

// Coordonnées pour le tripod gait right turn
int coordinatestripodRightTurnFL[2] = { 35, -35 };
int coordinatestripodRightTurnFR[2] = { 35, -35 };
int coordinatestripodRightTurnML[2] = { 35, -35 };
int coordinatestripodRightTurnMR[2] = { 35, -35 };
int coordinatestripodRightTurnBL[2] = { 35, -35 };
int coordinatestripodRightTurnBR[2] = { 35, -35 };


Gaits::Gaits() {}


#define CHECK_CMD(expectedCmd) \
  if (command != (expectedCmd)) { \
    neutralPosition(legs, ssc); \
    return; \
  }


// ------------------------------------------------------
// FORWARD
// ------------------------------------------------------
void Gaits::tripodGaitForward(Leg legs[6], SSC32& ssc) {

  Leg& FL = legs[0];
  Leg& FR = legs[1];
  Leg& ML = legs[2];
  Leg& MR_4 = legs[3];
  Leg& BL = legs[4];
  Leg& BR_6 = legs[5];

  int pulses[18];
  int p[3];

  // CYCLE 1
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('z');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FR, ML, BR
    float xFR = coordinatestripodForwardFR[0] + i * (amplitude / nbPts);
    float xML = coordinatestripodForwardML[0] - i * (amplitude / nbPts);
    float xBR = coordinatestripodForwardBR[0] + i * (amplitude / nbPts);

    inverseKine(xFR, bodyDistanceCorners, zSOL, FR.rotation, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zSOL, ML.rotation, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistanceCorners, zSOL, BR_6.rotation, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    // AIR : FL, MR, BL
    float xFL = coordinatestripodForwardFL[1] + i * (amplitude / nbPts);
    float xMR = coordinatestripodForwardMR[1] - i * (amplitude / nbPts);
    float xBL = coordinatestripodForwardBL[1] + i * (amplitude / nbPts);

    inverseKine(xFL, bodyDistanceCorners, zAIR, FL.rotation, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zAIR, MR_4.rotation, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistanceCorners, zAIR, BL.rotation, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }

  // CYCLE 2
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('z');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FL, MR, BL
    float xFL = coordinatestripodForwardFL[0] - i * (amplitude / nbPts);
    float xMR = coordinatestripodForwardMR[0] + i * (amplitude / nbPts);
    float xBL = coordinatestripodForwardBL[0] - i * (amplitude / nbPts);

    inverseKine(xFL, bodyDistanceCorners, zSOL, FL.rotation, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zSOL, MR_4.rotation, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistanceCorners, zSOL, BL.rotation, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    // AIR : FR, ML, BR
    float xFR = coordinatestripodForwardFR[1] - i * (amplitude / nbPts);
    float xML = coordinatestripodForwardML[1] + i * (amplitude / nbPts);
    float xBR = coordinatestripodForwardBR[1] - i * (amplitude / nbPts);

    inverseKine(xFR, bodyDistanceCorners, zAIR, FR.rotation, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zAIR, ML.rotation, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistanceCorners, zAIR, BR_6.rotation, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }
}


// ------------------------------------------------------
// BACKWARD
// ------------------------------------------------------
void Gaits::tripodGaitBackward(Leg legs[6], SSC32& ssc) {

  Leg& FL = legs[0];
  Leg& FR = legs[1];
  Leg& ML = legs[2];
  Leg& MR_4 = legs[3];
  Leg& BL = legs[4];
  Leg& BR_6 = legs[5];

  int pulses[18];
  int p[3];

  // CYCLE 1
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('s');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FR, ML, BR
    float xFR = coordinatestripodBackwardFR[0] - i * (amplitude / nbPts);
    float xML = coordinatestripodBackwardML[0] + i * (amplitude / nbPts);
    float xBR = coordinatestripodBackwardBR[0] - i * (amplitude / nbPts);

    inverseKine(xFR, bodyDistanceCorners, zSOL, FR.rotation, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zSOL, ML.rotation, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistanceCorners, zSOL, BR_6.rotation, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    // AIR : FL, MR, BL
    float xFL = coordinatestripodBackwardFL[1] - i * (amplitude / nbPts);
    float xMR = coordinatestripodBackwardMR[1] + i * (amplitude / nbPts);
    float xBL = coordinatestripodBackwardBL[1] - i * (amplitude / nbPts);

    inverseKine(xFL, bodyDistanceCorners, zAIR, FL.rotation, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zAIR, MR_4.rotation, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistanceCorners, zAIR, BL.rotation, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }

  // CYCLE 2
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('s');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FL, MR, BL
    float xFL = coordinatestripodBackwardFL[0] + i * (amplitude / nbPts);
    float xMR = coordinatestripodBackwardMR[0] - i * (amplitude / nbPts);
    float xBL = coordinatestripodBackwardBL[0] + i * (amplitude / nbPts);

    inverseKine(xFL, bodyDistanceCorners, zSOL, FL.rotation, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zSOL, MR_4.rotation, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistanceCorners, zSOL, BL.rotation, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    // AIR : FR, ML, BR
    float xFR = coordinatestripodBackwardFR[1] + i * (amplitude / nbPts);
    float xML = coordinatestripodBackwardML[1] - i * (amplitude / nbPts);
    float xBR = coordinatestripodBackwardBR[1] + i * (amplitude / nbPts);

    inverseKine(xFR, bodyDistanceCorners, zAIR, FR.rotation, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zAIR, ML.rotation, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistanceCorners, zAIR, BR_6.rotation, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }
}


// ------------------------------------------------------
// LEFT TURN
// ------------------------------------------------------
void Gaits::tripodGaitLeftTurn(Leg legs[6], SSC32& ssc) {

  Leg& FL = legs[0];
  Leg& FR = legs[1];
  Leg& ML = legs[2];
  Leg& MR_4 = legs[3];
  Leg& BL = legs[4];
  Leg& BR_6 = legs[5];

  int pulses[18];
  int p[3];

  // CYCLE 1
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('q');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FR, ML, BR
    float xFR = coordinatestripodLeftTurnFR[0] + i * (amplitude_turn / nbPts);
    float xML = coordinatestripodLeftTurnML[0] + i * (amplitude_turn / nbPts);
    float xBR = coordinatestripodLeftTurnBR[0] + i * (amplitude_turn / nbPts);

    inverseKine(xFR, bodyDistance, zSOL, 0.0, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zSOL, 0.0, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistance, zSOL, 0.0, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    // AIR : FL, MR, BL
    float xFL = coordinatestripodLeftTurnFL[1] - i * (amplitude_turn / nbPts);
    float xMR = coordinatestripodLeftTurnMR[1] - i * (amplitude_turn / nbPts);
    float xBL = coordinatestripodLeftTurnBL[1] - i * (amplitude_turn / nbPts);

    inverseKine(xFL, bodyDistance, zAIR, 0.0, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zAIR, 0.0, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistance, zAIR, 0.0, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }

  // CYCLE 2
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('q');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FL, MR, BL
    float xFL = coordinatestripodLeftTurnFL[0] + i * (amplitude_turn / nbPts);
    float xMR = coordinatestripodLeftTurnMR[0] + i * (amplitude_turn / nbPts);
    float xBL = coordinatestripodLeftTurnBL[0] + i * (amplitude_turn / nbPts);

    inverseKine(xFL, bodyDistance, zSOL, 0.0, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zSOL, 0.0, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistance, zSOL, 0.0, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    // AIR : FR, ML, BR
    float xFR = coordinatestripodLeftTurnFR[1] - i * (amplitude_turn / nbPts);
    float xML = coordinatestripodLeftTurnML[1] - i * (amplitude_turn / nbPts);
    float xBR = coordinatestripodLeftTurnBR[1] - i * (amplitude_turn / nbPts);

    inverseKine(xFR, bodyDistance, zAIR, 0.0, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zAIR, 0.0, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistance, zAIR, 0.0, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }
}


// ------------------------------------------------------
// RIGHT TURN
// ------------------------------------------------------
void Gaits::tripodGaitRightTurn(Leg legs[6], SSC32& ssc) {

  Leg& FL = legs[0];
  Leg& FR = legs[1];
  Leg& ML = legs[2];
  Leg& MR_4 = legs[3];
  Leg& BL = legs[4];
  Leg& BR_6 = legs[5];

  int pulses[18];
  int p[3];

  // CYCLE 1
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('d');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FR, ML, BR
    float xFR = coordinatestripodRightTurnFR[0] - i * (amplitude_turn / nbPts);
    float xML = coordinatestripodRightTurnML[0] - i * (amplitude_turn / nbPts);
    float xBR = coordinatestripodRightTurnBR[0] - i * (amplitude_turn / nbPts);

    inverseKine(xFR, bodyDistance, zSOL, 0.0, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zSOL, 0.0, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistance, zSOL, 0.0, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    // AIR : FL, MR, BL
    float xFL = coordinatestripodRightTurnFL[1] + i * (amplitude_turn / nbPts);
    float xMR = coordinatestripodRightTurnMR[1] + i * (amplitude_turn / nbPts);
    float xBL = coordinatestripodRightTurnBL[1] + i * (amplitude_turn / nbPts);

    inverseKine(xFL, bodyDistance, zAIR, 0.0, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zAIR, 0.0, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistance, zAIR, 0.0, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }

  // CYCLE 2
  for (int i = 0; i <= nbPts; i++) {
    CHECK_CMD('d');

    float zSOL = groundHeight;
    float t = (float)i / nbPts;
    float zAIR = groundHeight + stepHeight * sin(PI * t);

    // SOL : FL, MR, BL
    float xFL = coordinatestripodRightTurnFL[0] - i * (amplitude_turn / nbPts);
    float xMR = coordinatestripodRightTurnMR[0] - i * (amplitude_turn / nbPts);
    float xBL = coordinatestripodRightTurnBL[0] - i * (amplitude_turn / nbPts);

    inverseKine(xFL, bodyDistance, zSOL, 0.0, FL.alignGroundAngle, p);
    pulses[0] = p[0];
    pulses[1] = p[1];
    pulses[2] = p[2];
    inverseKine(xMR, bodyDistance, zSOL, 0.0, MR_4.alignGroundAngle, p);
    pulses[9] = p[0];
    pulses[10] = p[1];
    pulses[11] = p[2];
    inverseKine(xBL, bodyDistance, zSOL, 0.0, BL.alignGroundAngle, p);
    pulses[12] = p[0];
    pulses[13] = p[1];
    pulses[14] = p[2];

    // AIR : FR, ML, BR
    float xFR = coordinatestripodRightTurnFR[1] + i * (amplitude_turn / nbPts);
    float xML = coordinatestripodRightTurnML[1] + i * (amplitude_turn / nbPts);
    float xBR = coordinatestripodRightTurnBR[1] + i * (amplitude_turn / nbPts);

    inverseKine(xFR, bodyDistance, zAIR, 0.0, FR.alignGroundAngle, p);
    pulses[3] = p[0];
    pulses[4] = p[1];
    pulses[5] = p[2];
    inverseKine(xML, bodyDistance, zAIR, 0.0, ML.alignGroundAngle, p);
    pulses[6] = p[0];
    pulses[7] = p[1];
    pulses[8] = p[2];
    inverseKine(xBR, bodyDistance, zAIR, 0.0, BR_6.alignGroundAngle, p);
    pulses[15] = p[0];
    pulses[16] = p[1];
    pulses[17] = p[2];

    ssc.sendAllServos(pulses, velocity / nbPts);
  }
}


// ------------------------------------------------------
// NEUTRAL POSITION
// ------------------------------------------------------
void Gaits::neutralPosition(Leg legs[6], SSC32& ssc) {
  int pulses[18];
  int p[3];

  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 500);
}


// ------------------------------------------------------
// SHUTDOWN
// ------------------------------------------------------
void Gaits::shutDown(Leg legs[6], SSC32& ssc) {
  int pulses[18];
  int p[3];

  neutralPosition(legs, ssc);

  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, 175, 0, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 1500);
}



// ------------------------------------------------------
// TURN ON
// ------------------------------------------------------
void Gaits::turnOn(Leg legs[6], SSC32& ssc) {
  int pulses[18];
  int p[3];

  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, 175, 0, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 1500);
  delay(1500);

  // Phase 1 : position XY ready, z=0
  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, -30, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 1000);
  delay(1000);

  // Phase 2 : descente au sol
  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 2000);
}






// ------------------------------------------------------
// EMOTES
// ------------------------------------------------------

void Gaits::hello(Leg legs[6], SSC32& ssc) {
  int pulses[18];
  int p[3];

  neutralPosition(legs, ssc);

  Leg& L = legs[1];

  inverseKine(0, 200, 175, 0.0, L.alignGroundAngle, p);
  pulses[3 + 0] = p[0];
  pulses[3 + 1] = p[1];
  pulses[3 + 2] = p[2];

  ssc.sendAllServos(pulses, 500);
  delay(500);

  for (int i = 0; i < 3; i++) {
    inverseKine(0, 200, 175, -25.0, L.alignGroundAngle, p);
    pulses[3 + 0] = p[0];
    pulses[3 + 1] = p[1];
    pulses[3 + 2] = p[2];

    ssc.sendAllServos(pulses, 500);
    delay(500);

    inverseKine(0, 200, 175, 25.0, L.alignGroundAngle, p);
    pulses[3 + 0] = p[0];
    pulses[3 + 1] = p[1];
    pulses[3 + 2] = p[2];

    ssc.sendAllServos(pulses, 500);
    delay(500);
  }

  inverseKine(0, 200, 175, 0.0, L.alignGroundAngle, p);
  pulses[3 + 0] = p[0];
  pulses[3 + 1] = p[1];
  pulses[3 + 2] = p[2];

  ssc.sendAllServos(pulses, 500);
  delay(500);

  neutralPosition(legs, ssc);
}





void Gaits::twist(Leg legs[6], SSC32& ssc) {
  int pulses[18];
  int p[3];

  neutralPosition(legs, ssc);


  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, 30.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 750);
  delay(750);

  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, -30.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 1500);
  delay(1500);

  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, 30.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 1500);
  delay(1500);


  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 750);
  delay(750);
}



void Gaits::jump(Leg legs[6], SSC32& ssc) {
  int pulses[18];
  int p[3];

  neutralPosition(legs, ssc);

  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, -120, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 500);
  delay(500);


  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, -80, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 500);
  delay(500);


  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, -200, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 150);
  delay(150);


  for (int i = 0; i < 6; i++) {
    Leg& L = legs[i];
    inverseKine(0, bodyDistance, groundHeight, 0.0, L.alignGroundAngle, p);
    pulses[i * 3 + 0] = p[0];
    pulses[i * 3 + 1] = p[1];
    pulses[i * 3 + 2] = p[2];
  }
  ssc.sendAllServos(pulses, 200);
  delay(200);
}