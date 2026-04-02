# BTN 415 – Data Communications Programming

## Project (15%)


## Project Title: Smart Home Network Automation System
## PROJECT STRUCTURE
--
## Repository Structure

```
BTN435-project/
├── network/                  # Khoi — subnetting, routing, ARP
│   ├── topology.md           # Network diagram + subnet table
│   ├── routing_table.txt     # Static routing entries
│   └── src/
│       ├── server.cpp        # Multithreaded C++ server
│       ├── server.h
│       └── arp_handler.cpp   # ARP resolution logic
│
├── app/                      # Zlata — client + protocol
│   ├── protocol/
│   │   └── SPEC.md           # ← SHARED CONTRACT (read before coding)
│   └── client/
│       ├── client.cpp        # TCP socket client
│       └── client.h
│
├── devices/                  # Bay — device simulation + tests
│   ├── simulation/
│   │   ├── light.cpp
│   │   ├── thermostat.cpp
│   │   └── camera.cpp
│   └── tests/
│       ├── unit/
│       └── integration/
│
├── report/                   # Shared — each member adds their sections. Reports can be done in google Docs, ignore for now.
│   ├── 00_overview.md        # Written together at kickoff
│   ├── 01_network_architecture.md     # Khoi
│   ├── 02_protocol_implementation.md  # Zlata
│   ├── 03_testing_challenges.md       # Bay
│   └── 04_code_walkthrough.md         # All three contribute
│
└── README.md
```


### Khoi — Network & Server Lead

**Own:** `network/`

**Responsibilities:**

- Design the subnet topology using VLSM (minimum 3 subnets: lighting, thermostat, security)
- Document all subnets, IP ranges, and subnet masks in `network/topology.md`
- Implement static routing between subnets
- Implement ARP resolution in `arp_handler.cpp`
- Build the core multithreaded C++ server in `server.cpp`
  - Accept multiple simultaneous TCP client connections
  - Dispatch incoming commands to the correct device handler thread
  - Use mutexes to protect shared device state

**Do not modify:** `app/`, `devices/` — coordinate via the protocol spec instead.

**Definition of done:**

- Server compiles and binds to a configurable port
- At least 3 subnets documented with correct VLSM masks
- ARP table populates on startup and resolves IP → MAC
- Server handles at least 3 concurrent clients without crashing

---

### Zlata — Client & Protocol Lead

**Own:** `app/`

**Responsibilities:**

- Define the HTTP-like application protocol in `app/protocol/SPEC.md` — **do this first, in week 1**
  - Document all supported commands: `GET /light/on`, `GET /thermostat/set/22`, `GET /camera/status`, etc.
  - Document response format: status code, message body
- Implement the TCP socket client in `client.cpp`
  - Connect to the server using a configurable IP + port
  - Accept text commands from the user via stdin
  - Display server responses clearly
- Write `report/02_protocol_implementation.md`

**Do not modify:** `network/src/`, `devices/simulation/` — use the protocol spec as the boundary.

**Definition of done:**

- `SPEC.md` reviewed and acknowledged by Khoi and Bay before any coding begins
- Client connects, sends a command, and prints the server response
- All commands in the spec are implemented on the client side

---

### Bay — Devices & Testing Lead

**Own:** `devices/`

**Responsibilities:**

- Implement one source file per device: `light.cpp`, `thermostat.cpp`, `camera.cpp`
  - Each device runs in its own thread
  - Device state is protected with a mutex
  - Devices respond to commands defined in `app/protocol/SPEC.md`
- Write unit tests for each device (on/off, set value, status query)
- Write integration tests:
  - Single client controlling multiple devices
  - Multiple clients sending simultaneous commands
  - Device failure / unknown command handling
- Write `report/03_testing_challenges.md` including test results

**Do not modify:** `network/src/`, `app/client/` — wire devices into the server only via the agreed interface in `SPEC.md`.

**Definition of done:**

- All 3 devices compile, run in threads, and respond to commands correctly
- Unit tests pass for all devices
- Integration test demonstrates 3 simultaneous clients without race conditions

---

## Shared Responsibilities (All Three)

| Task | Details |
| --- | --- |
| Kickoff meeting | Agree on `SPEC.md` together before anyone writes device or client code. |
| Report intro | Write `report/00_overview.md` together (objectives, design choices, tech stack). |
| Code walkthrough | Each member contributes their section to `report/04_code_walkthrough.md`. |
| In-class demo | All three members present; each explains their own module. |
| Final recording | Each member records 2-3 minutes covering their contribution. |
| Peer evaluation | Complete honestly and on time. |

---



---
## INSTRUCTIONS
--
## Project Overview

This project involves the design and implementation of a smart home network automation system using C++ programming and various networking protocols. The system will allow users to control smart devices (e.g., lights, thermostat, security systems) within a home using networked communication.

---

## Learning Outcomes

- This project will require understanding and application of concepts including subnetting, routing, socket programming, multithreading, and network protocols to create a fully functional and scalable application.
- This project encourages collaboration, critical thinking, and problem-solving in a group environment.

---

## Core Requirements

- **Network Design:** Design a network topology using subnetting and VLSM to segment the smart home devices. Implement routing to enable communication across subnets.
- **Socket Programming:** Develop a multithreaded client-server application in C++ that enables communication between the client (user interface) and the server (controlling smart devices).
- **Protocols:** Implement various network protocols, including TCP/IP, HTTP-like application protocol, ARP, ICMP, and routing protocols.
- **Device Control:** Simulate the control of devices such as lights, thermostat, and security cameras. The system must allow users to turn devices on/off, check statuses, and modify settings.
- **Multithreading:** Use multithreading to handle concurrent client requests and manage different devices simultaneously.
- **Report:** Provide a comprehensive project report detailing your design, implementation, testing, and challenges faced during the development process.

---

## Project Description

### Network Design

- **Subnetting:** Design a network that consists of multiple subnets (e.g., separate subnets for lighting, thermostat, security systems, etc.).
- Use equal-size subnetting or Variable Length Subnet Masking (VLSM) to efficiently allocate IP addresses to each subnet.
- **Routing:** Implement static routing or default routing in the network to facilitate communication between devices on different subnets.
- **ARP:** Integrate ARP protocol to resolve IP addresses to MAC addresses and ensure devices can communicate over the local area network.

### Client-Server Application

- **Server:** Develop a multithreaded server in C++ that manages the state of devices and responds to client requests.
  - The server should be able to handle multiple clients simultaneously, each controlling different devices.
  - The server will interact with devices, sending commands like "Turn on light", "Set thermostat to 22°C", and "Check security system status".

- **Client:** Develop a client application that sends commands to the server to control devices. The client should use TCP sockets to communicate with the server.
  - The client will interact with the user to allow them to send commands to control devices.
  - Use simple text-based commands (e.g., `GET /light/on`, `GET /thermostat/set/22`, etc.).

### Application Layer Protocol

- Implement a simple HTTP-like protocol to send commands from the client to the server and receive responses.
  - Clients will send commands to control devices (e.g., `GET /light/on` or `GET /thermostat/set/22`).
  - The server will respond with the status of the device or a confirmation of the action performed.

### Multithreading and Concurrent Device Management

- Use multithreading to allow the server to handle multiple requests from clients simultaneously.
  - Each device (e.g., light, thermostat, camera) will be represented by a separate thread.
  - Ensure proper synchronization and thread safety to prevent race conditions when accessing shared device data.

### Routing and Network Communication

- Implement static or dynamic routing between different subnets, ensuring that the server can communicate with devices located on different subnets.
  - Handle routing tables and resolve IP addresses for devices on different network segments.
  - Use ARP to resolve IP addresses to MAC addresses for communication across the local area network.

### Critical Thinking and Integration

- Integrate all components into a coherent and functional system that allows the smart home devices to be controlled by users through the network.
- Address challenges such as network congestion, concurrent device control, error handling, and reliable data transmission.
- Implement robust error handling and ensure the system can operate without failures under real-world conditions (e.g., multiple clients, device failures).

---

## Project Report Requirements (15–20 pages)

1. **Project Overview**
   - Describe the objectives of the project, the design choices made for the system architecture, and the technology stack used (C++, socket programming, multithreading, etc.).

2. **Network Architecture**
   - Detail the network design (including subnets, devices, and routing) and explain how the system supports multiple devices and clients.

3. **Protocol Implementations**
   - Provide explanations of the network protocols used in the project, including TCP/IP, ARP, routing, and the custom HTTP-like protocol.
   - Discuss how each protocol was implemented and tested.

4. **Code Walkthrough**
   - Include key code snippets with comments to explain the functionality and logic behind critical sections of the code (e.g., multithreading, socket handling, device control).

5. **Testing**
   - Describe the testing process, including unit tests and integration tests to ensure that the system works as expected.
   - Provide results of testing with different configurations (e.g., multiple devices, multiple clients).

6. **Challenges and Solutions**
   - Discuss any challenges faced during the development process (e.g., network issues, threading problems) and how they were addressed.

---

## Rubric

| Criteria | Points | Weight |
| --- | ---: | ---: |
| Network Design (Subnetting and Routing) | 5 | 10% |
| Multithreaded Client-Server Application | 10 | 20% |
| Application Layer Protocol (HTTP-like) | 5 | 10% |
| Multithreading for Concurrent Device Management | 5 | 10% |
| Routing and Network Communication | 5 | 10% |
| In-Class Demonstration | 5 | 10% |
| Report Quality and Completeness | 5 | 10% |
| Project Presentation | 5 | 10% |
| "Above and Beyond" Features | 5 | 10% |
| **Total** | **50** | **100%** |

---

## "Above and Beyond"

Students can earn up to 5 points for implementing additional features that go beyond the core requirements. Possible bonus features include:

- Implementing Dynamic Routing Protocols (e.g., RIP or OSPF).
- Creating a web-based control panel using HTTP.
- Adding remote access to the smart home system via the internet or an external device.
- Implementing advanced security features, such as encryption or authentication.

---

## Group Work and Collaboration

- This is a group project (3–5 students per group).l
- Every group member should contribute equally to the coding, testing, and writing of the report.
- Peer evaluations will be used to assess individual contributions.

---

## In-Class Demonstration

- Groups are required to present their progress during scheduled lab sessions.
- Each group is expected to show the progress made up to the week of the demonstration.
- Demonstration schedules for all groups will be announced in advance.

---

## Project Presentation

- A recording of the final presentation by all group members is required.
- Each member should record 2–3 minutes presenting their contribution.
- The final combined recording must be included in the submitted project.
- You can upload in YouTube or any platform (as private) and provide the link for submission.
