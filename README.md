# STM32-Based Smart Soil Irrigation and Environmental Monitoring System

Automated soil moisture monitoring with temperature, humidity sensing and ESP8266 Wi-Fi communication.

## Academic Details & Identification

**Course:** 23CSE304 – Embedded Systems  
**Mentor:** Dr. A. Baskar  
**Department:** Department of Computer Science and Engineering  
**Institution:** School of Computing, Amrita Vishwa Vidyapeetham, Coimbatore, India

### Team Members

| Student Name | Registration Number |
| :--- | :--- |
| Eswar Suraj Reddy | [CB.SC.U4CSE24549] |
| Uddhav Krishna | [CB.SC.U4CSE24553] |
| Siddharth Sreekanth | [CB.SC.U4CSE24566] |
| Sai Kumar | [CB.SC.U4CSE24563] |

---

## Video Demo Link:
https://drive.google.com/file/d/1xj9iB1caGT8WEM6Aj6zGXLpLBuKXrS8L/view?usp=sharing

---

## 1. Problem Statement

Plants require an appropriate amount of water for healthy growth. Traditional irrigation methods depend on manual monitoring and watering, which can lead to overwatering, underwatering and unnecessary water consumption.

This project proposes an STM32-based smart soil irrigation and environmental monitoring system that continuously monitors soil moisture, temperature and humidity. The system detects dry and wet soil conditions, provides local alerts and transmits sensor information to a mobile device through an ESP8266 Wi-Fi module.

## 2. Objectives

- Develop an embedded soil irrigation and monitoring system using the STM32F401CCU6 (Black Pill) microcontroller.
- Measure soil moisture using an analog soil moisture sensor.
- Interface the soil moisture sensor with the STM32 ADC.
- Monitor environmental temperature and humidity using the DHT11 sensor.
- Detect dry and wet soil conditions using a predefined moisture threshold.
- Provide visual and audio indications using LEDs and an active buzzer.
- Establish Wi-Fi communication using the ESP8266 module.
- Transmit sensor readings and system status for remote monitoring.
- Reduce unnecessary water consumption through moisture-based irrigation control.

## 3. Proposed Solution

The STM32F401CCU6 acts as the main processing unit of the system.

The soil moisture sensor measures the moisture level in the soil and provides an analog output to the STM32 through pin PA0. The ADC converts this analog voltage into a digital value.

The controller compares the measured value against a predefined threshold:
- **Dry Soil:** The system identifies insufficient soil moisture and activates the dry-condition indication and audio buzzer.
- **Wet Soil:** The system identifies sufficient soil moisture and provides the corresponding wet-condition indication while turning off the alert buzzer.

A DHT11 sensor connected to PB8 measures environmental temperature and humidity.

An active buzzer connected to PB9 provides an audio alert. The ESP8266 Wi-Fi module communicates with the STM32 through UART, allowing sensor information and system status to be transmitted for remote monitoring.

## 4. System Workflow & Working

### Six-Step Workflow

- **Step 1 — Power On & Initialization:**  
  The STM32F401CCU6 initializes GPIO, ADC, SysTick timer and UART communication peripherals required for sensor monitoring and Wi-Fi communication.

- **Step 2 — Soil Moisture Detection:**  
  The soil moisture sensor continuously measures the moisture content and sends its analog output to PA0 of the STM32.

- **Step 3 — Analog-to-Digital Conversion:**  
  The STM32 ADC converts the analog sensor voltage into a digital value. The measured value is compared against the predefined soil moisture threshold.

- **Step 4 — Temperature & Humidity Monitoring:**  
  The DHT11 sensor connected to PB8 measures the surrounding temperature and humidity. The STM32 reads the sensor data for environmental monitoring.

- **Step 5 — Soil Condition Identification:**  
  The controller determines whether the soil is dry or wet:
  - **Dry Condition:** Dry-soil indication is activated, and the buzzer provides an alert.
  - **Wet Condition:** Wet-soil indication is activated, and the dry-condition alert is stopped.

- **Step 6 — Wi-Fi Communication:**  
  The ESP8266 Wi-Fi module communicates with the STM32 through UART. Sensor readings and system status are transmitted to a mobile device through the Wi-Fi monitoring interface.

### System Workflow Diagram

```text
              START
                │
                ▼
       Initialize STM32
                │
                ▼
       Read Soil Moisture
                │
                ▼
         ADC Conversion
                │
                ▼
       Compare Threshold
                │
          ┌─────┴─────┐
          │           │
          ▼           ▼
       DRY SOIL    WET SOIL
          │           │
          ▼           ▼
      Dry Status   Wet Status
      Indication   Indication
          │           │
          ▼           ▼
      Buzzer Alert  Buzzer OFF
          │           │
          └─────┬─────┘
                │
                ▼
       Read DHT11 Sensor
                │
                ▼
     Temperature & Humidity
                │
                ▼
       ESP8266 Wi-Fi
                │
                ▼
      Transmit Sensor Data
                │
                ▼
          Repeat Process
```

## 5. Hardware Pin Mapping

| Pin | Function | Peripheral | Connects To |
| :--- | :--- | :--- | :--- |
| **PA0** | ADC1_IN0 | ADC1 | Soil Moisture Sensor Analog Output (AO) |
| **PB8** | GPIO | GPIO | DHT11 Data Pin |
| **PB9** | GPIO Output | GPIO | Active Buzzer IN |
| **PA2** | USART2_RX | USART2 | ESP8266 TX |
| **3.3V** | Power Supply | STM32 3V3 | Soil Sensor, DHT11, and ESP8266 VCC |
| **GND** | Ground | Ground | Common Ground Rail |

### ESP8266 Pin Connections

| ESP8266 Pin | Connection |
| :--- | :--- |
| **TX** | STM32 PA2 (USART2_RX) |
| **RX** | Not Connected (TBD) |
| **VCC** | 3.3V Power Rail |
| **GND** | Common Ground |
| **CH_PD / EN** | 3.3V (Pull-up / Enabled) |

> **Note:** The current circuit uses the ESP8266 transmit line to send data to the STM32. The ESP8266 receive line is not connected in the documented wiring.

## 6. Project Circuit Diagram

The following circuit diagram illustrates the hardware connections between the STM32F401CCU6, soil moisture sensor, DHT11 temperature sensor, active buzzer, and ESP8266 Wi-Fi module:

<p align="center">
  <img src="docs/images/circuit_diagram.png" alt="Soil Irrigation System Circuit Diagram" width="900"/>
  <br/>
  <b>Figure 1: Circuit Diagram of STM32-Based Soil Irrigation System</b>
</p>

## 7. Hardware Prototype

The complete prototype is assembled using a breadboard, STM32 controller, ESP8266 Wi-Fi module, soil moisture sensor, DHT11 sensor and buzzer.

<p align="center">
  <img src="docs/images/prototype.png" alt="Soil Irrigation System Prototype" width="850"/>
  <br/>
  <b>Figure 2: Physical Prototype of Soil Irrigation and Environmental Monitoring System</b>
</p>

## 8. Hardware Components Required

| Component | Purpose |
| :--- | :--- |
| **STM32F401CCU6 (Black Pill)** | Main ARM Cortex-M4 microcontroller |
| **Soil Moisture Sensor** | Measures volumetric soil moisture via analog output |
| **DHT11 Sensor** | Measures ambient temperature and relative humidity |
| **ESP8266 Wi-Fi Module** | Acts as an Access Point and Web Server for wireless monitoring |
| **Active Buzzer** | Audio alert on dry soil detection |
| **Status LEDs** | Visual indication for dry/wet soil status |
| **Breadboard** | Hardware circuit prototyping |
| **Jumper Wires** | Interconnecting modules |
| **Micro-USB / 3.3V Power Source** | System power and programming |

## 9. Software & Technologies Used

- **Programming Language:** Embedded C (STM32), C++ / Wiring (ESP8266)
- **Microcontroller:** STM32F401CCU6 (ARM Cortex-M4)
- **Development Environment:** Keil µVision / STM32CubeIDE, Arduino IDE (ESP8266)
- **Communication Protocol:** UART (USART2 @ 9600 baud)
- **Analog Interface:** 12-bit ADC (ADC1 Channel 0)
- **Wireless Communication:** ESP8266 Wi-Fi (SoftAP Web Server)
- **Timing:** SysTick Timer & Delay routines
- **Sensors:** Analog Soil Moisture Sensor, DHT11 Temperature & Humidity Sensor

## 10. Repository Structure

```text
Soil-Irrigation-system./
├── docs/
│   └── images/
│       ├── circuit_diagram.png                # Circuit schematic diagram
│       └── prototype.png                      # Physical prototype photo (optional)
├── Soil Irrigation System/
│   ├── Arduino code/
│   │   └── sketch_nov02a/
│   │       └── sketch_nov02a.ino              # ESP8266 SoftAP Web Server dashboard
│   ├── keil code/
│   │   ├── soil.c                             # STM32 C firmware source code
│   │   ├── keil code.uvprojx                  # Keil µVision project file
│   │   └── RTE/                               # Keil Run-Time Environment CMSIS files
│   ├── docs/
│   │   └── images/
│   │       └── circuit_diagram.png
│   └── README.md                              # Subfolder documentation copy
└── README.md                                  # Main project documentation
```

## 11. How to Run the Project

### Part 1: Hardware Connections

Connect the components according to the hardware pin mapping table:
1. Connect the soil moisture sensor analog output (AO) to STM32 pin `PA0`.
2. Connect the DHT11 data pin to STM32 pin `PB8`.
3. Connect the active buzzer signal pin (`IN`) to STM32 pin `PB9`.
4. Connect the ESP8266 `TX` pin to STM32 pin `PA2`.
5. Connect all module `VCC` pins to the regulated `3.3V` rail.
6. Connect all module `GND` pins to the common `GND` rail.
7. Ensure that the ESP8266 receives a stable, regulated 3.3V power supply capable of delivering sufficient peak current during Wi-Fi transmission.

### Part 2: Programming the STM32

1. Open the Keil µVision project (`Soil Irrigation System/keil code/keil code.uvprojx`).
2. Verify that `soil.c` is included in the project workspace target.
3. Configure ADC1 on channel 0 (`PA0`) for continuous or single-conversion soil moisture measurement.
4. Configure GPIO pins for the active buzzer (`PB9`) and status LEDs.
5. Configure USART2 (`PA2`) for serial communication with the ESP8266 at 9600 baud.
6. Build and compile the project (`F7` in Keil).
7. Connect the ST-LINK V2 programmer to the SWD pins (`SWCLK`, `SWDIO`, `3.3V`, `GND`) of the STM32F401.
8. Flash the compiled firmware onto the microcontroller.
9. Reset the board and verify sensor readouts.

### Part 3: ESP8266 Wi-Fi Dashboard Setup

1. Open `Soil Irrigation System/Arduino code/sketch_nov02a/sketch_nov02a.ino` in Arduino IDE.
2. In Arduino IDE, configure the board settings:
   - **Board:** Generic ESP8266 Module (or NodeMCU 1.0)
   - **Upload Speed:** 115200
3. Connect the ESP8266 to your computer using a USB-to-UART converter.
4. Upload `sketch_nov02a.ino` to the ESP8266.
5. Once uploaded, reconnect the ESP8266 to the STM32 circuit.
6. Power on the system. The ESP8266 starts a Wi-Fi Access Point:
   - **SSID:** `STM32-Sensors`
   - **Password:** `sensor123`
7. Connect your smartphone or PC to the `STM32-Sensors` Wi-Fi network.
8. Open your browser and go to `http://192.168.4.1`.
9. The responsive dashboard will display live readings for Soil Moisture ADC, Temperature, and Humidity, updating automatically every 3 seconds.

## 12. Testing Plan

- **Soil Moisture Testing:** Test sensor response across varying soil conditions (completely dry, optimal moisture, saturated/wet).
- **ADC Testing:** Verify digital output variation corresponding to the sensor's analog voltage.
- **Threshold Calibration:** Verify that dry/wet switching thresholds trigger the buzzer and LED indications accurately.
- **Temperature & Humidity Testing:** Validate DHT11 sensor readings against ambient room conditions.
- **Buzzer Testing:** Verify that the audio alarm is asserted when dry soil is detected and silenced when moisture is restored.
- **Wi-Fi Communication Testing:** Confirm UART packet reception (`SOIL:<val>,TEMP:<val>,HUM:<val>`) and real-time dashboard rendering on connected mobile devices.
- **Continuous Monitoring:** Confirm sustained stability of continuous sensor sampling and Web Server responsiveness over extended run periods.

## 13. Limitations & Future Work

### Limitations

- **Sensor Corrosion:** Resistive soil moisture probes degrade over time due to electrolysis; capacitive probes provide better longevity.
- **Fixed Threshold:** A static threshold does not account for species-specific plant water requirements or diverse soil compositions.
- **Environmental Sensor Precision:** The DHT11 has moderate accuracy ($\pm 2^\circ\text{C}$, $\pm 5\%\text{ RH}$) compared to industrial sensors like DHT22 or BME280.
- **Wi-Fi Range:** Operates over a local SoftAP access point without direct internet gateway connectivity.
- **Actuation:** The documented setup demonstrates sensing and alert logic but does not include a motor driver/relay for automated water pumping.

### Future Work

- **Automated Pumping Mechanism:** Integrate a 5V/12V submersible DC pump controlled via a relay module or N-channel power MOSFET.
- **Cloud IoT Connectivity:** Upgrade the ESP8266 firmware to connect to cloud IoT platforms (Blynk, ThingSpeak, MQTT, AWS IoT Core) for worldwide remote monitoring.
- **Capacitive Soil Moisture Sensor:** Switch to capacitive soil sensing to prevent probe degradation.
- **Smart Irrigation Algorithms:** Implement closed-loop PID or fuzzy logic irrigation control taking historical weather and evapotranspiration data into account.
- **Water Reservoir Level Monitoring:** Add an ultrasonic sensor (HC-SR04) or float switch to monitor irrigation water tank levels.
- **Battery & Solar Power Management:** Implement low-power sleep modes on the STM32 and integrate a solar charging module (TP4056 + 18650 Li-ion battery).

## 14. Acknowledgement

We express our sincere gratitude to our mentor, **Dr. A. Baskar**, for his valuable academic guidance, constructive feedback, and continuous support throughout the development of this project under the course **23CSE304 – Embedded Systems**, Department of Computer Science and Engineering, School of Computing, Amrita Vishwa Vidyapeetham, Coimbatore, India.

---

**Project:** STM32-Based Smart Soil Irrigation and Environmental Monitoring System  
**Course:** 23CSE304 – Embedded Systems  
**Institution:** Amrita Vishwa Vidyapeetham, Coimbatore
