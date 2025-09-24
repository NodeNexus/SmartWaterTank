# 💧 ESP32 Smart Water Tank Controller

## 📌 Overview
This project monitors **water level** in a tank using an ESP32 + ultrasonic sensor and controls a **pump**.  
It serves a **local web dashboard** to view water level and manually toggle the pump.

---

## 🛠️ Hardware Required
- ESP32 Dev Board  
- Ultrasonic Sensor (HC-SR04)  
- Water Pump + Relay Module / MOSFET  
- Jumper wires, breadboard  

---

## 🔌 Wiring
- **Ultrasonic Sensor**  
  - TRIG → GPIO5  
  - ECHO → GPIO18  
  - VCC → 5V  
  - GND → GND  

- **Pump**  
  - Controlled via GPIO19 through Relay/MOSFET  

---

## ▶️ Usage
1. Upload the sketch to ESP32.  
2. Connect to Wi-Fi AP `ESP32-AP` (password: `12345678`).  
3. Open browser → go to `192.168.4.1`.  
4. Dashboard shows **water level %** and **pump state**.  
5. Click “Toggle Pump” to manually control the pump.  

---

## 📂 Repo Structure
