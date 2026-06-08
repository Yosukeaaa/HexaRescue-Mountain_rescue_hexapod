HexaRescue is a mountain‑rescue hexapod robot designed to navigate rough terrain, detect potential victims, and transmit reliable information to rescue teams.
The robot combines robust mechanical design, modular electronics, and onboard AI to operate in challenging environments.

<img width="687" height="363" alt="image" src="https://github.com/user-attachments/assets/93250311-cde1-4d51-b309-9fdc1b974096" />


## Mechanical Design
HexaRescue is a 50 cm hexapod with six legs, each offering three degrees of freedom for stability and obstacle‑crossing.

<img width="195" height="196" alt="image" src="https://github.com/user-attachments/assets/f91edd37-6fcf-461c-b4e5-75afffef7092" />

The robot is built on three stacked layers:

  - Top layer — Jetson Nano + ESP32

  - Middle layer — 2S Li‑Po battery (centered for stability)

  - Bottom layer — SSC‑32U servo controller + power converters

Most parts are 3D‑printed in PETG, while the chassis is metal for rigidity.
The feet use TPE pads for improved grip on uneven terrain.

## Electrical Architecture
Power comes from a 2S Li‑Po battery, split into three rails:

  - Direct Li‑Po → SSC‑32U powering 20 servos
  - 5 V buck converter → Jetson Nano + ESP32
  - 3.3 V buck converter → sensors

## Sensor Placement
Sensors are positioned to maximize reliability:

  - GPS at the front, away from EMI sources
  - RGB + thermal cameras on a 2‑DOF camera arm
  - Distance sensor at the bottom for ground‑level obstacle detection

## Software Architecture & AI
The system uses a simple but effective communication scheme:

The ESP32 hosts a Wi‑Fi network and serves a web interface
The smartphone connects simultaneously to:

  - ESP32 → commands + telemetry
  - Jetson Nano → video stream

<img width="1077" height="468" alt="image" src="https://github.com/user-attachments/assets/72a0d7c5-f6c6-48a5-a0ae-97422053a5a4" />


The Jetson Nano runs YOLOv5 to detect people.

When a detection occurs, the ESP32 cross‑checks it with thermal data from the GridEYE sensor. 

<img width="231" height="178" alt="image" src="https://github.com/user-attachments/assets/608b7a97-187c-4ed2-996c-63f660a11b36" /><img width="236" height="197" alt="image" src="https://github.com/user-attachments/assets/e75a75bf-14fa-4ead-84fb-fdbef59615e7" /> 

If both confirm a human presence, the operator receives an alert popup with timestamp and GPS coordinates. <img width="300" height="200" alt="image" src="https://github.com/user-attachments/assets/dba4bd86-cb95-4c18-a382-d7869a8ddf94" />


