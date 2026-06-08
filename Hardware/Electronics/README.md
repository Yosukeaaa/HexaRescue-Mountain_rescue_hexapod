HexaRescue is powered by a 2S Li‑Po battery. From this battery, three separate power rails are derived:

    - Servo power rail — The SSC‑32U board is connected directly to the Li‑Po and supplies power to the 20 servomotors 
    (18 for the legs, 2 for the camera arm).

    - 5V rail — A 5V buck converter steps down the battery voltage to power the Jetson Nano and the ESP32.

    - 3.3V rail — A 3.3V buck converter provides a stable voltage for all sensors.

A main power switch is placed right after the battery to ensure safety during startup and maintenance.

The following diagram shows the overall electrical architecture of the robot.

<img width="1000" height="450" alt="image" src="https://github.com/user-attachments/assets/e97e9baf-44a6-40de-bf62-36bf73839d79" />
