# Web Application Overview
The HexaRescue web application is divided into four main sections, each designed to give the operator a clear and efficient control interface.


<div style="display: flex; align-items: center;">
<img width="244" height="737" alt="image" src="https://github.com/user-attachments/assets/585c06db-2f8c-4df0-b30c-5867f45fd5da" style="margin-right: 20px;"/>
  <p>
    Ton texte ici. Tu peux écrire plusieurs lignes, mettre des titres, etc.
  </p>
</div>

## Status & Telemetry Panel
This section displays the robot’s real‑time state through colored indicators.
Each LED‑style indicator reflects the health of a subsystem (robot status, sensors, communication, ...).
The panel also shows the robot’s live position by GPS coordinates.

## Multi‑View Camera Interface
The operator can switch between three different video feeds:
  - RGB view — standard camera feed
  - Thermal view — temperature‑based visualization
  - Fusion view — combined RGB + thermal overlay for enhanced detection

This setup provides a complete understanding of the environment, even in low‑visibility conditions.

## Robot Control Panel
This section provides direct control over the robot’s movements:

  - Forward / backward
  - Rotation
  - Head movement (left/right and up/down)

Commands are sent instantly through the ESP32, ensuring responsive control during navigation.

## Detection Alerts & History
All confirmed detections appear in a dedicated alert feed.
Each entry includes:

  - Detection time
  - GPS coordinates
  - Thermal signature

This creates a detailed history of all potential victims detected during the mission.
