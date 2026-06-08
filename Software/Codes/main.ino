#include "Leg.h"
#include "inverseKine.h"
#include "Gaits.h"
#include "SSC32.h"
#include "webapp.h"

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include "esp_wifi.h"

const char* AP_SSID = "HexaRescue";
const char* AP_PASSWORD = "hexapode42";

WebServer httpServer(80);
WebSocketsServer wsServer(81);

TinyGPSPlus gps;
HardwareSerial GPSserial(1);

float gpsLat = 0.0;
float gpsLng = 0.0;
float gpsAlt = 0.0;
bool gpsValid = false;

const int PINS[18] = {
  31, 30, 29,
  12, 13, 14,
  27, 26, 25,
  5, 6, 7,
  19, 18, 17,
  1, 2, 3
};

#define CAM_YAW_PIN 10
#define CAM_PITCH_PIN 11

Leg HexaRescue[6] = {
  Leg(1, -60.0, 0.0, PINS[0], PINS[1], PINS[2]),
  Leg(2, 55.0, 0.0, PINS[3], PINS[4], PINS[5]),
  Leg(3, 0.0, 0.0, PINS[6], PINS[7], PINS[8]),
  Leg(4, 0.0, 0.0, PINS[9], PINS[10], PINS[11]),
  Leg(5, 55.0, 0.0, PINS[12], PINS[13], PINS[14]),
  Leg(6, -60.0, 0.0, PINS[15], PINS[16], PINS[17])
};

Gaits gait;
SSC32 ssc(Serial2, PINS, 18);

char command = '\0';
bool robotEnabled = false;
bool robotNeutral = false;
bool distanceLock = false;
float groundHeight = -120.0;
float stepHeight = 80.0;
int velocity = 1500;

int camYawUs = 1500;
int camPitchUs = 1500;
bool camDirty = false;
bool camCenter = false;

void sendCameraServos() {
  if (!camDirty) return;
  int transition = camCenter ? 500 : 80;
  char cmd[64];
  snprintf(cmd, sizeof(cmd), "#%dP%d#%dP%dT%d\r",
           CAM_YAW_PIN, camYawUs, CAM_PITCH_PIN, camPitchUs, transition);
  if (SSC32::serial2Mutex && xSemaphoreTake(SSC32::serial2Mutex, pdMS_TO_TICKS(5)) == pdTRUE) {
    Serial2.print(cmd);
    xSemaphoreGive(SSC32::serial2Mutex);
    camDirty = false;
    camCenter = false;
  }
}

void centerCamera() {
  camYawUs = 1500;
  camPitchUs = 1500;
  camDirty = true;
}

#include <Wire.h>
#include <VL53L0X.h>
#include <SparkFun_GridEYE_Arduino_Library.h>

VL53L0X sensor;
GridEYE grideye;

volatile int distanceVL53 = -1;
volatile bool bodyDetected = false;
volatile bool yoloDetected = false;
const float THERMAL_DIFF_THRESHOLD = 2.5;  // °C de différence min pour confirmer

float lastTempMax = 0.0;
float lastTempAvg = 0.0;
float lastTempMin = 0.0;
bool gridEyeOK = false;
float lastGrid[64] = { 0 };

unsigned long lastTelemSend = 0;

void sendTelemetry() {
  if (millis() - lastTelemSend < 1000) return;
  lastTelemSend = millis();

  bool tempValid = gridEyeOK && (lastTempMax > -40.0) && (lastTempMax < 150.0);

  StaticJsonDocument<2048> doc;
  doc["type"] = "telem";
  doc["distance"] = distanceVL53;
  doc["tempMax"] = tempValid ? lastTempMax : 0.0;
  doc["tempAvg"] = tempValid ? lastTempAvg : 0.0;
  doc["tempValid"] = tempValid;
  doc["bodyDetected"] = bodyDetected;
  doc["yoloDetected"] = yoloDetected;
  doc["robotEnabled"] = robotEnabled;
  doc["robotNeutral"] = robotNeutral;
  doc["distanceLock"] = distanceLock;
  doc["gpsValid"] = gpsValid;
  doc["gpsLat"] = gpsLat;
  doc["gpsLng"] = gpsLng;
  doc["gpsAlt"] = gpsAlt;
  JsonArray grid = doc.createNestedArray("grid");
  for (int i = 0; i < 64; i++) grid.add(lastGrid[i]);

  char buf[2048];
  serializeJson(doc, buf);
  wsServer.broadcastTXT(buf);
}

enum EmoteType { EMOTE_NONE,
                 EMOTE_HELLO,
                 EMOTE_TWIST,
                 EMOTE_JUMP };
EmoteType currentEmote = EMOTE_NONE;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type != WStype_TEXT) return;

  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, payload)) return;

  const char* msgType = doc["type"] | "";

  if (strcmp(msgType, "ping") == 0) {
    wsServer.sendTXT(num, "{\"type\":\"pong\"}");
    return;
  }
  if (strcmp(msgType, "cmd") == 0) {
    const char* c = doc["cmd"] | "S";
    command = c[0];
    return;
  }
  if (strcmp(msgType, "param") == 0) {
    const char* key = doc["key"] | "";
    int value = doc["value"] | 0;
    if (strcmp(key, "H") == 0) groundHeight = -(float)value;
    if (strcmp(key, "U") == 0) stepHeight = (float)value;
    if (strcmp(key, "V") == 0) velocity = value;
    return;
  }
  if (strcmp(msgType, "cam") == 0) {
    int yaw = doc["yaw"] | 1500;
    int pitch = doc["pitch"] | 1500;
    bool center = doc["center"] | false;
    camYawUs = constrain(yaw, 1000, 2000);
    camPitchUs = constrain(pitch, 1000, 2000);
    camDirty = true;
    camCenter = center;
    return;
  }
  if (strcmp(msgType, "emote") == 0) {
    const char* name = doc["name"] | "";
    if (strcmp(name, "hello") == 0) {
      command = 'E';
      currentEmote = EMOTE_HELLO;
    }
    if (strcmp(name, "twist") == 0) {
      command = 'E';
      currentEmote = EMOTE_TWIST;
    }
    if (strcmp(name, "jump") == 0) {
      command = 'E';
      currentEmote = EMOTE_JUMP;
    }
    return;
  }
  if (strcmp(msgType, "yolo") == 0) {
    bool yoloSaw = doc["yoloDetected"] | false;
    if (yoloSaw) {
      // Confirmation thermique obligatoire
      bool thermalConfirmed = gridEyeOK && (lastTempMax > -40.0) && (lastTempMax - lastTempMin >= THERMAL_DIFF_THRESHOLD);
      yoloDetected = thermalConfirmed;
      Serial.printf("[YOLO] Detection YOLO=%s | TempMax=%.1f TempAvg=%.1f | Thermal=%s\n",
                    yoloSaw ? "OUI" : "NON",
                    lastTempMax, lastTempAvg,
                    thermalConfirmed ? "CONFIRMEE" : "NON CONFIRMEE");
    } else {
      yoloDetected = false;
    }
    return;
  }
}

void networkTask(void* pvParameters) {
  while (true) {
    httpServer.handleClient();
    wsServer.loop();
    sendTelemetry();
    sendCameraServos();
    vTaskDelay(1);
  }
}

void captorsTask(void* pvParameters) {
  unsigned long lastVL53 = 0;
  unsigned long lastGridEye = 0;

  while (true) {
    // GPS
    while (GPSserial.available()) {
      gps.encode(GPSserial.read());
      if (gps.location.isUpdated()) {
        gpsLat = gps.location.lat();
        gpsLng = gps.location.lng();
        gpsAlt = gps.altitude.meters();
        gpsValid = true;
      }
    }

    unsigned long now = millis();

    // VL53L0X
    if (now - lastVL53 >= 50) {
      lastVL53 = now;
      int d = sensor.readRangeContinuousMillimeters();
      if (!sensor.timeoutOccurred()) distanceVL53 = d;
      else distanceVL53 = -1;
    }

    // GridEYE
    if (now - lastGridEye >= 300) {
      lastGridEye = now;
      if (gridEyeOK) {
        float validPixels[64];
        int validCount = 0;
        bool readOK = true;

        for (int i = 0; i < 64; i++) {
          float t = grideye.getPixelTemperature(i);
          if (t < 0.0 || t > 80.0) continue;  // filtre bruit
          lastGrid[i] = t;
          validPixels[validCount++] = t;
        }

        if (validCount < 32) {  // moins de la moitié valide = lecture suspecte
          readOK = false;
        }

        if (readOK) {
          // Tri pour médiane
          for (int i = 0; i < validCount - 1; i++)
            for (int j = i + 1; j < validCount; j++)
              if (validPixels[j] < validPixels[i]) {
                float tmp = validPixels[i];
                validPixels[i] = validPixels[j];
                validPixels[j] = tmp;
              }

          float median = (validCount % 2 == 0)
                           ? (validPixels[validCount / 2 - 1] + validPixels[validCount / 2]) / 2.0
                           : validPixels[validCount / 2];

          float biggest = validPixels[validCount - 1];
          float smallest = validPixels[0];
          float sumTemp = 0;
          for (int i = 0; i < validCount; i++) sumTemp += validPixels[i];

          lastTempMax = biggest;
          lastTempAvg = sumTemp / validCount;
          lastTempMin = smallest;

          // Détection : au moins 1 pixel dépasse médiane + seuil
          const float MEDIAN_THRESHOLD = 3.0;
          bodyDetected = (biggest - median >= MEDIAN_THRESHOLD);

          Serial.printf("[THERMAL] median=%.1f max=%.1f diff=%.1f body=%s\n",
                        median, biggest, biggest - median, bodyDetected ? "OUI" : "NON");
        }
      }
    }

    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(115200);

  GPSserial.begin(9600, SERIAL_8N1, 34, -1);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  ssc.begin(9600);

  Wire.begin(21, 22);

  sensor.setTimeout(500);
  if (!sensor.init()) Serial.println("[VL53L0X] FAIL");
  else {
    sensor.startContinuous(50);
    Serial.println("[VL53L0X] OK");
  }

  grideye.begin();
  delay(100);
  float testTemp = grideye.getPixelTemperature(0);
  gridEyeOK = (testTemp > -40.0 && testTemp < 150.0);
  Serial.println(gridEyeOK ? "[GridEYE] OK" : "[GridEYE] FAIL");

  SSC32::serial2Mutex = xSemaphoreCreateMutex();
  centerCamera();

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.softAPIP());

  WiFi.softAP(AP_SSID, AP_PASSWORD, 6, 0, 4, false);

  httpServer.on("/", []() {
    httpServer.send_P(200, "text/html", INDEX_HTML);
  });
  httpServer.begin();
  Serial.println("[HTTP] pret");

  wsServer.begin();
  wsServer.onEvent(webSocketEvent);
  Serial.println("[WS] pret port 81");

  xTaskCreatePinnedToCore(networkTask, "networkTask", 12288, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(captorsTask, "captorsTask", 8192, NULL, 1, NULL, 1);
}

void loop() {

  if (command == '\0') return;
  
  if (command == 'S') {
    if (robotEnabled) gait.shutDown(HexaRescue, ssc);
    robotEnabled = false;
    robotNeutral = false;
    return;
  }
  if (command == 'O') {
    if (!robotEnabled) gait.turnOn(HexaRescue, ssc);
    robotEnabled = true;
    robotNeutral = false;
    return;
  }

  if (!robotEnabled) return;

  switch (command) {
    case 'z': gait.tripodGaitForward(HexaRescue, ssc); break;
    case 'q': gait.tripodGaitLeftTurn(HexaRescue, ssc); break;
    case 's': gait.tripodGaitBackward(HexaRescue, ssc); break;
    case 'd': gait.tripodGaitRightTurn(HexaRescue, ssc); break;
    case 'N':
      gait.neutralPosition(HexaRescue, ssc);
      robotNeutral = true;
      distanceLock = false;
      break;
  }

  if (command == 'E' && robotEnabled) {
    switch (currentEmote) {
      case EMOTE_HELLO: gait.hello(HexaRescue, ssc); break;
      case EMOTE_TWIST: gait.twist(HexaRescue, ssc); break;
      case EMOTE_JUMP: gait.jump(HexaRescue, ssc); break;
      default: break;
    }
    currentEmote = EMOTE_NONE;
    command = 'N';
    return;
  }
}
