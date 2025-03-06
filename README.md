# Antenna Deployment Mechanism

## Project Overview
This project is part of the **Nanosatellite Design and Electronics (MPA-NDE 24/25L)** course at **Brno University of Technology (BUT), Faculty of Electrical Engineering and Communication (FEEC)**. Our team is developing an **Antenna Deployment Mechanism** for a CubeSat-class nanosatellite.

## Purpose
The Antenna Deployment Mechanism (ADM) is designed to stow and deploy a flexible metal tape antenna reliably once the satellite reaches its operational orbit. The mechanism ensures the antenna remains secured during launch and deploys upon receiving an activation signal from the microcontroller unit (MCU).

## Key Features
- **Compact and Lightweight Design** – Optimized for CubeSat integration.
- **Reliable Deployment** – Ensures consistent antenna extension in space conditions.
- **Locking Mechanism** – Holds the antenna in place until deployment is required.
- **MCU Controlled Activation** – Deployment triggered via command.

## Components
- **Flexible Metal Tape Antenna**
- **Holding Mechanism (Damping System, Hinges, and Latch)**
- **Microcontroller (MCU) and Activation Circuitry**
- **Sensors for Deployment Verification**

## Block diagram
<img width="712" alt="blokove_schema_navrhu" src="https://github.com/user-attachments/assets/2b6d8c76-4798-46ee-b662-08b46337790c" />
## SW (Dodělat)

```c
   DE2-PROJECT          
   ├── include         // Included file(s)
   │   └── timer.h
   ├── lib             // Libraries
   │   ├── gpio        // Your GPIO library
   │   │   ├── gpio.c
   │   │   └── gpio.h
   │   ├── oled        
   │   │   ├── oled.c
   │   │   ├── oled.h
   │   │   └── font.h
   │   ├── uart       
   │   │   ├── uart.c
   │   │   └── uart.h
   │   ├── twi       
   │   │   ├── twi.c
   │   │   └── twi.h
   │   ├── soil       
   │   │   ├── soil.c
   │   │   └── soil.h
   │   ├── controls       
   │   │   ├── controls.c
   │   │   └── controls.
   │   ├── HumTempSensor       
   │   │   ├── HumTempSensor.c
   │   │   └── HumTempSensor.h
   │   ├── rtc       
   │   │   ├── rtc.c
   │   │   └── rtc.h
   │   ├── display       
   │   │   ├── display.c
   │   │   └── display.h
   │   ├── buttons       
   │   │   ├── buttons.c
   │   │   └── buttons.h
   │   ├── pot       
   │   │   ├── pot.c
   │   │   └── pot.h
   │   └── light       
   │       ├── light.c
   │       └── light.h
   ├── src             // Source file(s)
   │   └── main.c
   ├── test            // No need this
   └── platformio.ini  // Project Configuration File
   ```



## Team
This project is developed by students of **MPA-NDE 24/25L** at BUT FEEC as part of a nanosatellite technology research initiative.

## Future Work
- Prototype development and testing.
- Thermal and mechanical stress analysis.
- Integration with satellite subsystems.

## License
This project is for academic and research purposes under the Nanosatellite Design and Electronics course at BUT.


