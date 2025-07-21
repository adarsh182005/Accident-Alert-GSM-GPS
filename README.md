# Accident-Alert-GSM-GPS
Arduino-based accident detection using piezo sensor, GPS (NEO-6M), and GSM (SIM900A) to send real-time SMS alerts.
## 🔌 Hardware Connections

### 1. Piezoelectric Sensor
- Signal → A0 (Arduino)
- VCC → 5V
- GND → GND
- **Optional:**
  - 10kΩ resistor between signal and GND
  - 0.1µF capacitor between A0 and GND

### 2. GPS Module (NEO-6M)
- TX → RX (Arduino Pin 4) (with voltage divider: 1kΩ + 2kΩ)
- RX → TX (Arduino Pin 3)
- VCC → 5V
- GND → GND

### 3. GSM Module (SIM900/SIM800)
- TX → RX (Arduino Pin 7)
- RX → TX (Arduino Pin 8) (with voltage divider: 1kΩ + 2kΩ)
- VCC → 5V
- GND → GND
