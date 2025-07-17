# FINAL_PROJECT

## 🚘 Real-Time Vehicle Status Monitoring System using CAN Protocol (LPC2129)

A microcontroller-based vehicle monitoring solution built on the **CAN (Controller Area Network) protocol**, designed to enhance vehicle safety, diagnostics, and modular communication between embedded nodes. Developed using the **LPC2129 ARM7 microcontroller**, this system simulates real-time interaction between distributed nodes for **indicators**, **fuel sensing**, and **central status display**.

> 💡 Designed for embedded system enthusiasts and automotive R&D projects.

---

## 🔍 What is This Project?

A distributed embedded system that mimics how modern vehicles use **CAN bus** for seamless data exchange between various control units (like indicators, fuel sensors, etc.). This project demonstrates:

- **Multi-node communication over CAN**
- Real-time monitoring and status display
- Modular embedded software architecture
- Practical use of ARM7 microcontroller (LPC2129)

---

## 🧩 System Architecture

### 📦 Nodes Involved:
- **Indicator Node**
  - Detects switch inputs (left/right)
  - Sends indicator direction over CAN

- **Fuel Node**
  - Reads analog fuel level via ADC
  - Transmits fuel data via CAN

- **Main Node**
  - Receives data from all other nodes
  - Displays status on a 16x2 LCD (fuel + indicators)

### 🔁 Communication Protocol:
- All nodes communicate using the **CAN protocol**
- CAN frames carry 8-byte payloads (fuel level, direction, status)
- **MCP2551** transceiver handles physical layer

---

## 🔧 Hardware Used

| Component        | Description                                        |
|------------------|----------------------------------------------------|
| **LPC2129**      | ARM7 microcontroller — core of each node           |
| **MCP2551**      | CAN transceiver IC                                  |
| **MMA7660**      | 3-axis accelerometer (for advanced control, optional)|
| **16x2 LCD**     | Displays status info at Main Node                  |
| **ADC Input**    | Reads analog fuel sensor values                    |
| **Switches & LEDs** | For manual indicator simulation and alerts     |
| **Power Supply** | Regulated 5V DC                                    |

---

## 🛠 Software & Tools

- **Keil µVision** – Code development & compilation
- **Flash Magic** – Flashing firmware to LPC2129
- **Embedded C** – Low-level code for microcontroller
- **Proteus** *(Optional)* – Circuit simulation
- **GitHub** – Version control and project tracking

---
