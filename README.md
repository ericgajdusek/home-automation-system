# Smart IoT Home Automation System

This project is a distributed IoT system for controlling lighting and motorized blinds using ESP32 microcontrollers and a local server. It features a custom communication protocol with acknowledgment-based messaging, centralized state management, and a modular architecture that supports real-time control, automation, and remote access.

## Features

- Touchscreen controller interface for real-time system control  
- Wireless control of lighting and motorized blinds  
- Sensor-based automation (light + motion detection)  
- Centralized server for state management and coordination  
- Scheduling system (in progress)  
- To-do list and calendar integration (in progress)  
- Web dashboard for monitoring and control (in progress)

## Architecture

This system uses a hub-and-spoke architecture centered around a local server. The touchscreen controller acts as the main user interface, while separate ESP32 nodes handle dedicated hardware tasks such as lighting control and blinds actuation. Rather than sending commands directly between nodes, the system routes communication through the server so that state management, logging, and command coordination are handled in one place.

### Components
##### Server
The server acts as the source of truth for all system data. It stores the latest known node states, such as light brightness and blinds position, and is also responsible for managing higher-level features such as scheduling, sensor-driven automation, to-do list data, calendar events, and web app access. Keeping system state centralized on the server makes the design more reliable because the controller interface and web app both read from the same authoritative state.

##### ESP32 Nodes
The system is composed of three primary types of ESP32 nodes: **controller**, **sensor**, and **actuator** nodes. Each type is responsible for a distinct role within the system.

- **Controller Nodes**  
    Provide the user interface, accepting input and sending commands to the server for execution.
- **Sensor Nodes**  
    Collect environmental data used for automation. The current implementation includes an ambient sensor node that monitors light levels and motion to support night-light style automation.
- **Actuator Nodes**  
    Execute physical actions in response to system commands. This includes:
    - **Lighting Nodes** for controlling LED strips throughout the room
    - **Blinds Node** for driving a stepper motor that adjusts window blinds

See [Components](./docs/components.md) for full breakdown, diagrams, and documentation.
### Communication
The primary communication flow is:
Controller → Server → Actuator Node  
Actuator Node → Server → Controller

The system uses acknowledgment-based messaging with message and transaction IDs to ensure reliable communication and track command completion across nodes.

See [Communication Architecture](./docs/communication_architecture.md) for full breakdown.

### Separation of Concerns
This architecture separates user interaction, system coordination, and hardware control into different components. That separation makes the system easier to debug, extend, and document. It also supports future features such as remote web control, voice commands processed through the server, and more advanced automation logic without requiring major changes to the hardware nodes.

## Security Measures

Security features are currently in development. Planned measures include encrypted communication, secure device authentication, and VPN-based remote access.

## Current Status

- Wireless device communication implemented  
- ESP32 nodes operational (lights, blinds)  
- Touchscreen controller functional  
- Server infrastructure set up  
- Messaging protocol designed and partially implemented  
- Web app, scheduling, and user data features in progress

## Tech Stack

- ESP32 (Embedded C/C++)  
- MQTT / ESP-NOW  
- Ubuntu Server (Linux)  
- Python (backend)  
- SQLite (database)  
- Web frontend (in progress)
