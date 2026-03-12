# ESP32 Marauder — Cheap Yellow Display

<p align="center">
  <img alt="Marauder logo" src="https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/blob/master/img/readme0.png" width="300">
</p>

<p align="center">
  <img src="https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool/actions/workflows/pages.yml/badge.svg" alt="GitHub Actions Badge" />
  <img src="https://img.shields.io/badge/version-1.4.3-000000?style=flat" alt="GitHub Release Version Badge" />
  <img src="https://img.shields.io/github/issues/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display?style=flat&color=2EA44F" alt="GitHub Issues" />
  <br>
  <img src="https://img.shields.io/github/commits-since/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/latest?style=flat&color=1F285E" />
  <img src="https://komarev.com/ghpvc/?username=Fr4nkFletcher&label=Views&color=333333&style=flat" alt="Profile Views" />
  <a href="https://twitter.com/Fr4nkFletcher">
    <img src="https://img.shields.io/badge/Follow-%231DA1F2.svg?style=flat&logo=x&logoColor=white&color=1F285E" alt="Follow me on X">
  </a>
</p>

The aim of this project is to port the ESP32-Marauder firmware to the Cheap Yellow Display (CYD), offering powerful WiFi and Bluetooth testing features on an affordable and accessible hardware platform.

> **This fork adds native support for the ESP32-S3-CAM N16R8 RE 1.3** — a non-standard S3 board with OPI PSRAM and a 2.8" ILI9341 display via FPC ribbon. See the [ESP32-S3-CAM N16R8 Port](#esp32-s3-cam-n16r8-re-13-port) section below.

---

## 🏴‍☠️ Latest Update Highlights — v1.4.3 (04/16/25) 🏴‍☠️

- Add security check to AP scanning
- Add WPS and Manufacturer check for AP scan
- Add more stats to Raw Capture
- Add WiFi analyzer
- Add quick names to channel analyzer graph
- Fix evil portal AP name character limit
- Add packet count function
- Add AP info
- Add Raw Capture
- Add single scan for AP and Stations
- Add generate random MACs for AP and Station WiFi interface
- Fixed sniffer output overlapping 
- Reduced memory usage
- Add index number to AP scan display
- Add BLE analyzer 
- Add support for ESP32-2432S024C 2.4" ILI9341/CST820 Capacitive Touch
- Add support for ESP32-3248S035C 3.5" ST7796/GT911 Capacitive Touch
- Add support for ESP32-2432S032C 3.2" ST7789/GT911 Capacitive Touch
- Add support for ESP32-2432S032R 3.2" ST7789/XPT2046 Resistive Touch 
- Add support for Guition ESP32-2432S024R 2.4" Resistive Touch
- Recalibrate touch for 3.5" 
- Fix bluetooth attack LED not turning off
- Fix status LED for bluetooth stuff
- Add support for ESP32-3248S035R 3.5" ST7796/XPT2046 Resistive Touch
- Add support for Adafruit MAX17048 battery monitor
- Update Save/Load Files menu to add for saving and loading AirTags
- Add logging to SD for Flipper/AirTag sniff
- Add Flipper Zero Sniff
- Airtag Sniffing/Spoofing
- Working Pwnagotchi Detect on all models
- Flipper BLE Spam
- Wardriving Menu added
- Added compatibility for ESP32-2432S024R 2.4" Resistive Touch USB Type-C Only
- [Guide for antenna modification](https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/blob/master/AntennaModNew.md) using ESP-WROOM-32U with built-in IPEX/U.FL
- [Evil Portal examples and setup](https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/blob/master/evilportal/)
- [How to add an external antenna](https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/blob/master/AntennaMod.md)

---

## Requirements

1. A compatible CYD module (see [Compatibility](#compatibility))
2. Chrome browser
3. Data-capable USB cable
4. *(Optional)* [GPS](https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display?tab=readme-ov-file#gps-functionality) module for enhanced functionality

---

## Installation Steps

### Web Flasher Method (Recommended)

1. Go to the [CYM Web Flasher](https://fr4nkfletcher.github.io/Adafruit_WebSerial_ESPTool/)
2. Hold BOOT on your device, click "Connect" and select
3. Choose the appropriate Model and Version
4. Click "Program" to start flashing

<p align="center">
  <img src="https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool/blob/main/assets/sc00000.jpg?raw=true" alt="CYM Web Flasher Screenshot" width="100%" style="max-width:800px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1);">
</p>

### Manual Web Flasher Method (for installing old releases)

1. Visit the [manual flasher](https://fr4nkfletcher.github.io/Adafruit_WebSerial_ESPTool/manual)
2. Hold BOOT, click Connect
3. Set memory offset and upload files
4. Program

<p align="center">
  <img src="https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool/blob/main/assets/scman1.png" alt="Manual Flashing">
</p>

| Offset | -> | Bin |
|--------|:--:|------|
| 0x1000  | -> | [Bootloader](https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool/raw/refs/heads/main/resources/STATIC/M/CYD/esp32_marauder.ino.bootloader.bin) |
| 0x8000  | -> | [Partitions](https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool/raw/refs/heads/main/resources/STATIC/M/CYD/esp32_marauder.ino.partitions.bin) |
| 0x10000 | -> | [Firmware](https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool/tree/main/resources/CURRENT)   |

#### Troubleshooting:

If issues arise, try the following steps:

1. Unplug and restart your CYD module
2. Hold `RST`, tap `BOOT`, release `RST` (the screen should go blank)
3. Refresh the Web Flasher page and click "Connect"

For further details, check out the [Web Flasher repository](https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool).

Alternatively, you can flash using [esptool.py](https://github.com/espressif/esptool) by:
```
cd ~
git clone https://github.com/Fr4nkFletcher/Adafruit_WebSerial_ESPTool
esptool.py --port /dev/YOURSERIALPORT write_flash 0x1000 ~/Adafruit_WebSerial_ESPTool/resources/STATIC/M/CYD/esp32_marauder.ino.bootloader.bin \
0x8000 ~/Adafruit_WebSerial_ESPTool/resources/STATIC/M/CYD/esp32_marauder.ino.partitions.bin \
0x10000 ~/Adafruit_WebSerial_ESPTool/resources/CURRENT/esp32_marauder_ofyourchoice.bin
```
---

### Manual Arduino IDE Method

1. Set up your Arduino environment following the [ESP32 Marauder Arduino IDE Setup Guide](https://github.com/justcallmekoko/ESP32Marauder/wiki/arduino-ide-setup)
2. Update your platform.txt
3. Add the necessary libraries to your Arduino libraries folder
4. Set the upload speed to `115200` in the Arduino IDE (tested on version 1.8.19)
5. Upload the firmware to your CYD module

For a step-by-step guide, refer to [Smoochiee's tutorial](https://github.com/smoochiee/MARAUDER-FOR-CYD---CHEAP-YELLOW-DISPLAY)

---

## ESP32-S3-CAM N16R8 RE 1.3 Port

This fork adds a `cyd28_s3` PlatformIO environment targeting the **ESP32-S3-CAM N16R8 RE 1.3** — a board with 16 MB Flash, 8 MB OPI PSRAM, and a 2.8" ILI9341 resistive touch display connected via FPC ribbon on the PCB back.

> **Important:** Pre-compiled CYD bins target the classic ESP32 (Xtensa LX6). The S3 uses Xtensa LX7 — a different ISA. Flashing a WROOM binary on S3 causes a white screen of death. You must build from source with this config.

### Board Identity
- **MCU:** ESP32-S3-CAM N16R8 RE 1.3
- **Flash:** 16 MB QIO | **PSRAM:** 8 MB OPI (octal)
- **Display:** 2.8" ILI9341 240×320 resistive touch via FPC ribbon
- **USB:** Left USB-C = UART0/PROG | Right USB-C = Native USB OTG (GPIO19/20)
- **LED:** Single WS2812 on GPIO48 (NOT discrete RGB)

### GPIO Pinout

**TFT Display (ILI9341, hardware SPI2)**

| Signal | GPIO | Notes |
|--------|------|-------|
| MOSI   | 45   | strapping pin — safe after boot |
| SCLK   | 3    | also JTAG_SEL — safe after boot |
| CS     | 14   | |
| DC     | 47   | |
| RST    | 21   | |
| BL     | —    | hardwired to 3.3V, NOT GPIO → `TFT_BL = -1` |

**Touch (XPT2046, software SPI)**

| Signal   | GPIO |
|----------|------|
| CLK      | 42   |
| CS       | 1    |
| MOSI/DIN | 2    |
| MISO/DO  | 41   |

### Build from Source (PlatformIO)

```bash
# Full Marauder build + flash
~/.platformio/penv/bin/pio run -e cyd28_s3 --target upload

# Minimal TFT diagnostic (RED/GREEN/BLUE then "TFT OK")
~/.platformio/penv/bin/pio run -e tft_diag --target upload

# Serial monitor (won't hold board in reset)
~/.platformio/penv/bin/pio device monitor -e cyd28_s3
```

### Key `platformio.ini` Settings

```ini
platform  = espressif32 @ 6.4.0
board     = esp32-s3-devkitc-1
board_build.arduino.memory_type = qio_opi  ; OPI PSRAM — required
monitor_dtr = 0                             ; prevents monitor reset loop
monitor_rts = 0
```

### Known Issues / Disabled Features

| Feature | Reason disabled | How to re-enable |
|---------|----------------|------------------|
| `HAS_BATTERY` | No battery IC; `Wire.endTransmission()` hangs on MAX17048 check | Define correct I2C_SDA/SCL pins first |
| `HAS_GPS` | MARAUDER_V4 maps GPS_TX=GPIO1 (TOUCH_CS) and GPS_RX=GPIO3 (TFT_SCLK) — kills display | Remap GPS_TX/GPS_RX to free GPIOs |

### Debugging

| Symptom | Cause | Fix |
|---------|-------|-----|
| White screen on monitor connect | DTR holding EN low | `monitor_dtr=0` / `monitor_rts=0` |
| Stuck at "Initializing..." | GPS stole TFT_SCLK (GPIO3) | Disable `HAS_GPS` |
| Stuck at "No IP5306 found" | I2C hangs after failed init | Disable `HAS_BATTERY` |
| Colors cyan/magenta/yellow | Wrong color inversion | Use `TFT_INVERSION_OFF` |
| PSRAM not detected | Missing OPI memory type | Add `qio_opi` to `platformio.ini` |
| `gpio_set_level(226)` error | `TFT_BL=-1` cast to uint8_t wraps | Guard with `#if TFT_BL >= 0` |

### Free GPIOs for Future Add-ons (CC1101, NRF24, IR, OLED)

`4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 38, 39, 40`

Avoid: 19/20 (USB OTG), 43/44 (UART0), 48 (WS2812), 35/36/37 (PSRAM)

---

## Compatibility

The project has been successfully tested on:

- **ESP32-S3-CAM N16R8 RE 1.3** (this fork — 2.8" ILI9341 FPC, OPI PSRAM)
- [2.4" Capacitive Touch](https://a.co/d/bTSoo9Z)
- [2.4" Resistive Touch](https://a.co/d/fhM7s0J)
- [2.4" Resistive Touch Guition](https://www.aliexpress.us/item/3256806436471011.html)
- [2.8" Resistive MicroUSB-only](https://amazon.com/dp/B0BVFXR313)
- [2.8" Resistive MicroUSB/Type-C 2USB](https://amazon.com/dp/B0CLR7MQ91)
- [3.2" Resistive Touch](https://www.aliexpress.us/item/3256806436888726.html)
- [3.2" Capacitive Touch](https://a.co/d/faB7oVU)
- [3.5" Resistive Touch](https://a.co/d/cxUc73a)
- [3.5" Capacitive Touch](https://a.co/d/2PFDlvL)

No hardware modifications are required, thanks to **@ggaljoen's** fork of the [TFT_eSPI](https://github.com/ggaljoen/TFT_eSPI) library

---

## GPS Functionality

GPS functionality is fully supported via the 4-pin connector near the MicroUSB port. For a list of compatible GPS hardware, refer to the [official wiki](https://github.com/justcallmekoko/ESP32Marauder/wiki/gps-modification)

| GPS | -> | CYD |
|-----|:--:|-----|
| VCC | -> | VIN |
| GND | -> | GND |
| TX  | -> | TX  |
| RX  | -> | RX  |

Note: On 2.4" and 3.5" models swap RX/TX

---

## Example Usage

After flashing, you'll boot into the Marauder interface. Refer to the [ESP32 Marauder Wiki](https://github.com/justcallmekoko/ESP32Marauder/wiki) for detailed usage instructions

<p align="center">
  <img src="https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/blob/master/screenshots/2.gif" alt="Demo 1" width="45%">
  <img src="https://github.com/Fr4nkFletcher/ESP32-Marauder-Cheap-Yellow-Display/blob/master/screenshots/swift2.gif" alt="Demo 2" width="45%">
</p>

---

## Acknowledgments

A huge thanks to **@cod5fgzj**, [**smoochiee**](https://github.com/smoochiee), [**ggaljoen**](https://github.com/ggaljoen), [**ATOMNFT**](https://github.com/ATOMNFT), [**jstockdale**](https://github.com/jstockdale), and [**sorinbotirla**](https://github.com/sorinbotirla). And a special mention to [**JustCallMeKoko**](https://github.com/justcallmekoko) for the foundational work on [ESP32Marauder](https://github.com/justcallmekoko/ESP32Marauder).

---

## Disclaimer

This project is for educational purposes only. Always obtain proper authorization before testing on networks you don't own or have explicit permission to test. Don't be a dick!
