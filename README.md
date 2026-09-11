# EPS32C3-Environment-Monitoring

A standalone offline-capable environmental monitoring system built on ESP32-C3. It reads ambient temperature and humidity from a DHT11 sensor, displays live readings and status on an SSD1306 OLED screen, and provides visual humidity feedback via three status LEDs. The system operates fully without WiFi, and can host a simple web dashboard when connected to a 2.4GHz network.

---

## Project Overview
This project turns an ESP32-C3 microcontroller into a compact, self-contained sensor node with both local and remote monitoring capabilities. All core functions — sensor reading, OLED display and LED indicators — work completely offline, with a clear `NO SIG` indicator on screen when WiFi is unavailable. When connected to a 2.4GHz hotspot, the ESP32 runs a built-in web server that serves a responsive status page viewable from any device on the same network.

## Key Features
- DHT11 digital sensor for ambient temperature (°C) and relative humidity (%)
- 0.96" SSD1306 I2C OLED display (128×64) with live readings and status alerts
- On-screen status messages: `TOO HOT`, `TOO COLD`, `TOO WET`, `NORMAL`, `SENSOR ERROR`
- Three LED humidity indicators:
  - Red (blinking): humidity > 70% (warning / too wet)
  - Yellow (steady): humidity 40–70% (normal / okay)
  - Blue (steady): humidity < 40% (good / dry)
- Full offline operation: all local features function without WiFi connection
- WiFi connection timeout (10s) — firmware never hangs waiting for a network
- Non-blocking timer: sensor, OLED and LED status refresh every 5 seconds
- NaN-safe sensor handling: detects disconnected or faulty sensor and displays error state
- Built-in web server on port 80 with responsive status page for remote viewing
- PlatformIO project structure, built on Arduino framework for ESP32-C3

---

## Hardware Wiring
| Component | Pin | ESP32-C3 GPIO |
|---|---|---|
| DHT11 | DATA (S) | GPIO 4 |
| SSD1306 OLED | SDA | GPIO 0 |
| SSD1306 OLED | SCL | GPIO 1 |
| Red LED (+) | Anode (via 220Ω resistor) | GPIO 7 |
| Yellow LED (+) | Anode (via 220Ω resistor) | GPIO 8 |
| Blue LED (+) | Anode (via 220Ω resistor) | GPIO 9 |
| All components | VCC | 3.3V |
| All components | GND | GND |

> Note: Use a 220Ω current-limiting resistor in series with each LED to protect ESP32 I/O pins. All devices must share a common ground.

---

## Build & Upload with PlatformIO
### Prerequisites
- VS Code with PlatformIO IDE extension
- ESP32-C3 development board
- DHT11 sensor module
- SSD1306 I2C OLED display
- USB-C data cable (charge-only cables will not work)

### Steps
1. Clone or download this repository
2. Open the project folder in VS Code / PlatformIO
3. Configure WiFi credentials in `src/main.cpp`:
   ```cpp
   const char* WIFI_SSID = "YOUR_2.4G_WIFI_SSID";
   const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
   ```
   > ⚠️ ESP32-C3 only supports 2.4GHz WiFi. 5GHz hotspots will not connect.
4. Connect your ESP32-C3 board to your computer via USB
5. Click the **Upload** (→) button in the PlatformIO toolbar, or run `pio run --target upload`
6. Wait for the upload to finish successfully, then press the reset button on the board

### View Debug Output
Open the Serial Monitor (115200 baud) to see boot logs, WiFi status and the ESP32 IP address.

---

## Operation Guide
1. Power on the ESP32-C3 — OLED displays `Starting...`
2. The device attempts to connect to the configured WiFi for up to 10 seconds
   - Success: OLED shows `WIFI:OK` and web server starts
   - Failure: OLED shows `NO SIG` and continues running in offline mode
3. Every 5 seconds: sensor readings are updated, OLED refreshes, LED status is adjusted
4. If the DHT11 is disconnected or faulty, the screen displays `SENSOR ERROR`

---

## Customization
You can adjust these parameters in code to fit your use case:
- Temperature thresholds for `TOO HOT` / `TOO COLD` alerts
- Humidity threshold for `TOO WET` warning
- Sensor refresh interval (default: 5 seconds)
- Red LED blink speed
- WiFi connection timeout duration

---

## Project Structure
```
EPS32C3-Environment-Monitoring/
├── src/
│   ├── main.cpp        # Main firmware: sensor reading, WiFi, web server, LED logic
│   ├── oled_ui.h       # OLED display rendering functions
│   └── web.h           # Built-in web page HTML
├── platformio.ini      # PlatformIO build configuration & dependencies
├── .gitignore
└── README.md
```

---

## License
For educational and self-study purposes. Feel free to use, modify and build upon this project.
