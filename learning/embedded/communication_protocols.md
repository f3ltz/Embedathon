# Communication Protocols: SPI, I2C, and UART

## 1. Serial Peripheral Interface (SPI)

### Overview
- High-speed, full-duplex communication protocol.
- Typically used for short-distance communication between a microcontroller and peripherals (e.g., SD cards, displays).

### Key Features
- **Master-Slave Configuration:** One master controls one or more slaves.
- **4 Communication Lines:**
  - **MOSI (Master Out, Slave In):** Data from master to slave.
  - **MISO (Master In, Slave Out):** Data from slave to master.
  - **SCLK (Serial Clock):** Synchronizes data transfer.
  - **SS (Slave Select):** Selects the active slave device.

### Advantages
- High data transfer speed.
- Full-duplex communication.
- Simple hardware implementation.

### Disadvantages
- Requires more pins than I2C.
- Additional SS pin for each slave device.

### ESP32 SPI Pins (Default)
| Pin Name | GPIO  |
|----------|-------|
| MOSI     | 23    |
| MISO     | 19    |
| SCLK     | 18    |
| SS       | 5     |

### Example Code
```cpp
#include <SPI.h>

void setup() {
    SPI.begin();             // Initialize SPI
    pinMode(5, OUTPUT);      // SS pin as output
    digitalWrite(5, HIGH);   // Set SS high (inactive)
}

void loop() {
    digitalWrite(5, LOW);    // Activate slave
    SPI.transfer(0xFF);      // Send data
    digitalWrite(5, HIGH);   // Deactivate slave
    delay(1000);
}
```

---

## 2. Inter-Integrated Circuit (I2C)

### Overview
- Multi-device communication protocol using only two wires.
- Ideal for connecting sensors, EEPROMs, and displays.

### Key Features
- **Master-Slave Configuration:** Master controls multiple slaves using unique addresses.
- **2 Communication Lines:**
  - **SDA (Serial Data Line):** Transmits data.
  - **SCL (Serial Clock Line):** Synchronizes data transfer.

### Advantages
- Uses only two pins for communication.
- Supports multiple devices (up to 127) on a single bus.

### Disadvantages
- Slower than SPI.
- Half-duplex communication (data sent or received at a time).

### ESP32 I2C Pins (Default)
| Pin Name | GPIO  |
|----------|-------|
| SDA      | 21    |
| SCL      | 22    |

### Example Code
```cpp
#include <Wire.h>

void setup() {
    Wire.begin();                  // Initialize I2C
    Serial.begin(115200);
}

void loop() {
    Wire.beginTransmission(0x3C);  // Communicate with device at address 0x3C
    Wire.write("Hello I2C");       // Send data
    Wire.endTransmission();        // End communication
    delay(1000);
}
```

---

## 3. Universal Asynchronous Receiver-Transmitter (UART)

### Overview
- Asynchronous communication protocol.
- Often used for serial communication between a microcontroller and peripherals (e.g., GPS modules, serial monitors).

### Key Features
- **Full-Duplex Communication:** Simultaneous data transmission and reception.
- Requires only 2 communication lines:
  - **TX (Transmit):** Sends data.
  - **RX (Receive):** Receives data.
- Baud rate (e.g., 9600 bps) must match between devices.

### Advantages
- Simple and widely supported.
- Minimal pin usage.

### Disadvantages
- Only supports communication between two devices.
- Requires accurate timing and matching baud rates.

### ESP32 UART Ports
| UART  | TX Pin | RX Pin |
|-------|--------|--------|
| UART0 | 1      | 3      |
| UART1 | 10     | 9      |
| UART2 | 17     | 16     |

### Example Code
```cpp
void setup() {
    Serial.begin(115200);  // Initialize UART
}

void loop() {
    Serial.println("Hello UART!");  // Send data
    delay(1000);
}
```

---

## Comparison Table

| Feature              | SPI                 | I2C               | UART              |
|----------------------|---------------------|-------------------|-------------------|
| **Pins Required**    | 4+                 | 2                 | 2                 |
| **Speed**            | Fast               | Slower            | Medium            |
| **Communication**    | Full-Duplex        | Half-Duplex       | Full-Duplex       |
| **Devices Supported**| Limited by SS pins | Up to 127         | 2                 |
| **Use Cases**        | Displays, SD cards | Sensors, EEPROMs  | GPS, Serial Monitor |

---

## Conclusion
- **SPI:** Use when speed is critical and pin count is not a concern.
- **I2C:** Use when multiple devices share the same bus and fewer pins are available.
- **UART:** Use for serial communication between two devices.
```

Copy-paste this Markdown content into your Markdown editor or viewer, and it will render properly with tables, headers, and code blocks. Let me know if you need further adjustments! 😊