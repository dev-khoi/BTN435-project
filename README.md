# BTN 415 – Data Communications Programming

## Project (15%)

## Project Title: Smart Home Network Automation System

---

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

| Criteria                                        | Points | Weight   |
| ----------------------------------------------- | ------ | -------- |
| Network Design (Subnetting and Routing)         | 5      | 10%      |
| Multithreaded Client-Server Application         | 10     | 20%      |
| Application Layer Protocol (HTTP-like)          | 5      | 10%      |
| Multithreading for Concurrent Device Management | 5      | 10%      |
| Routing and Network Communication               | 5      | 10%      |
| In-Class Demonstration                          | 5      | 10%      |
| Report Quality and Completeness                 | 5      | 10%      |
| Project Presentation                            | 5      | 10%      |
| "Above and Beyond" Features                     | 5      | 10%      |
| **Total**                                       | **50** | **100%** |

---

## "Above and Beyond"

Students can earn up to 5 points for implementing additional features that go beyond the core requirements. Possible bonus features include:

- Implementing Dynamic Routing Protocols (e.g., RIP or OSPF).
- Creating a web-based control panel using HTTP.
- Adding remote access to the smart home system via the internet or an external device.
- Implementing advanced security features, such as encryption or authentication.

---

## Group Work and Collaboration

- This is a group project (3–5 students per group).
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
