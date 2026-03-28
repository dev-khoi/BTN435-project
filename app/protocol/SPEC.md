# Smart Home Network Automation System

## Application Protocol Specification (SPEC.md)

---

## 1. Overview

This document defines the application-layer protocol used for communication between the client and the server in the Smart Home Network Automation System.

The protocol is HTTP-like and uses simple text-based commands over TCP.

---

## 2. Connection Details

* Protocol: TCP
* Default Port: 8080
* Communication Type: Request–Response
* Encoding: Plain text

---

## 3. Request Format

Each request follows this format:

GET /<device>/<action>/<value>

Where:

* `<device>` = light | thermostat | camera | network
* `<action>` = command to execute
* `<value>` = optional parameter (used for thermostat)

---

## 4. Supported Commands

### Light Control

* `GET /light/on` → Turn light ON
* `GET /light/off` → Turn light OFF
* `GET /light/status` → Get light status

### Thermostat Control

* `GET /thermostat/set/<temperature>` → Set temperature (e.g., 22)
* `GET /thermostat/status` → Get current temperature

### Camera Control

* `GET /camera/status` → Get camera status

### Network Diagnostics

* `GET /network/arp` → Get ARP table (IP to MAC mapping)

---

## 5. Response Format

Each response consists of:

<status_code> <status_message>
<response_body>

### Status Codes:

* `200 OK` → Success
* `400 ERROR` → Invalid request
* `404 NOT FOUND` → Unknown device
* `500 SERVER ERROR` → Internal error

---

## 6. Example Responses

### Success:

200 OK
Light turned ON

### Thermostat:

200 OK
Temperature set to 22°C

### Error:

400 ERROR
Invalid command format

### ARP Table:

200 OK
ARP Table:
IP Address       MAC Address          Interface   Status
192.168.1.10     AA:BB:CC:DD:EE:01   eth0        Resolved
192.168.2.5      AA:BB:CC:DD:EE:02   eth1        Resolved

---

## 7. Connection Lifecycle

1. Client connects to server via TCP
2. Client sends a request
3. Server processes the request
4. Server sends response
5. Connection may remain open or close after response

---

## 8. Error Handling

* Invalid commands → 400 ERROR
* Unknown device → 404 NOT FOUND
* Server issues → 500 SERVER ERROR

---

## 9. Notes

* Commands are case-insensitive
* Only one request per line
* Multiple clients can connect simultaneously
