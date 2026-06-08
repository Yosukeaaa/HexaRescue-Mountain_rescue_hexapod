<img width="273" height="210" alt="image" src="https://github.com/user-attachments/assets/44d3e111-2353-4680-84ad-40aa97803237" /># Web Application Overview
The HexaRescue web application is divided into four main sections, each designed to give the operator a clear and efficient control interface.

  ## Status & Telemetry Panel
  This section displays the robot’s real‑time state through colored indicators.
  Each LED‑style indicator reflects the health of a subsystem (robot status, sensors, communication, ...).
  The panel also shows the robot’s live position by GPS coordinates.

  ## Multi‑View Camera Interface
  The operator can switch between three different video feeds:
  - RGB view — standard camera feed  
  - Thermal view — temperature‑based visualization  
  - Fusion view — combined RGB + thermal overlay for enhanced detection

<img width="250" height="200" alt="image" src="https://github.com/user-attachments/assets/673a13ae-728d-47d3-a4d2-d64a6360c886" /> --> <img width="277" height="235" alt="image" src="https://github.com/user-attachments/assets/2b8f6580-e4eb-4d9c-b739-dd770a24a2a9" /> --> <img width="312" height="245" alt="image" src="https://github.com/user-attachments/assets/38620d12-16c2-47ad-bbd6-6549fbbe7e06" />




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

  
 <img width="244" height="737" alt="image" src="https://github.com/user-attachments/assets/585c06db-2f8c-4df0-b30c-5867f45fd5da" />

