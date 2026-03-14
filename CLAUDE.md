# k4eru — ESP32-S3-CAM N16R8 RE 1.3 + 2.8" ILI9341 Marauder Port

## Board Identity
- **MCU module:** ESP32-S3-CAM N16R8 RE 1.3 (NOT a standard CYD/WROOM board)
- **Flash:** 16MB QIO | **PSRAM:** 8MB OPI (octal — requires `board_build.arduino.memory_type = qio_opi`)
- **Display:** 2.8" ILI9341 240×320 resistive touch, connected via FPC ribbon on PCB back
- **USB:** Left USB-C = UART0/PROG | Right USB-C = Native USB OTG (GPIO19/20)
- **LED:** Single WS2812 on GPIO48 (NOT discrete RGB)

## Confirmed GPIO Pinout (from manufacturer FPC schematic)

### TFT Display (ILI9341, hardware SPI2)
| Signal | GPIO | Notes |
|--------|------|-------|
| MOSI   | 45   | strapping pin — safe after boot |
| SCLK   | 3    | also JTAG_SEL — safe after boot |
| CS     | 14   | |
| DC     | 47   | |
| RST    | 21   | |
| MISO   | 46   | input-only/strapping; effectively unused |
| BL     | —    | hardwired to 3.3V, NOT GPIO → TFT_BL = -1 |

### Touch (XPT2046, software SPI — SOFTSPI in TFT_eSPI)
| Signal   | GPIO |
|----------|------|
| CLK      | 42   |
| CS       | 1    |
| MOSI/DIN | 2    |
| MISO/DO  | 41   |
| IRQ      | -1 (NC) |

### MicroSD (SDMMC 1-bit — no CS pin, VDD always-on, pull-ups on PCB)
| Signal | GPIO | Notes |
|--------|------|-------|
| CLK    | 38   | **silkscreen says CMD** — manufacturer label is WRONG |
| CMD    | 39   | **silkscreen says CLK/MTCK** — manufacturer label is WRONG |
| D0     | 40   | silkscreen correct (MTDO) |

> **Warning:** Do NOT trust the silkscreen for SD pins on this board. CLK and CMD are swapped vs the label. Confirmed by exhaustive pin permutation test.

### Other onboard
| Function | GPIO |
|----------|------|
| WS2812   | 48   |
| Flash LED | 2   |
| USB D+   | 19   |
| USB D-   | 20   |
| PSRAM    | 35,36,37 (internal — NEVER use externally) |

## TFT_eSPI User_Setup.h Critical Settings
```c
#define ILI9341_DRIVER       // NOT ILI9341_2_DRIVER
#define TFT_INVERSION_OFF    // panel is natively non-inverted; ON causes cyan/magenta/yellow
// No TFT_RGB_ORDER needed — native RGB
// No USE_HSPI_PORT — S3 GPIO matrix routes SPI2 to any pin
#define SOFTSPI              // touch on separate pins requires software SPI
```

## platformio.ini Critical Settings
```ini
board_build.arduino.memory_type = qio_opi  ; OPI PSRAM — without this PSRAM silently fails
monitor_dtr = 0                             ; prevents monitor from holding board in reset loop
monitor_rts = 0
```

## configs.h Changes for MARAUDER_V4 / CYD_28

### Disabled features and why
```c
//#define HAS_BATTERY
// No battery IC on board. Wire.begin() receives invalid I2C pin (gpio 870 error),
// then Wire.endTransmission() on MAX17048 check hangs forever.
// TO RE-ENABLE: define correct I2C_SDA / I2C_SCL for this board first, then uncomment.

//#define HAS_GPS
// MARAUDER_V4 maps GPS_TX=GPIO1 (= TOUCH_CS) and GPS_RX=GPIO3 (= TFT_SCLK).
// Serial2.begin() steals those pins and kills both display and touch.
// TO RE-ENABLE: remap GPS_TX/GPS_RX to free GPIOs before uncommenting.
```

### Added to CYD_28 block
```c
#define TFT_BL -1   // backlight hardwired; prevents pinMode(-1) wrapping to GPIO226 error
#define TOUCH_CS 1  // correct XPT2046 CS for this board
```

### Added to neopixel PIN block
```c
#elif defined(MARAUDER_V4)
  #define PIN 48    // WS2812 on GPIO48 — fallback was GPIO4 (camera pin)
```

## esp32_marauder.ino Changes
```cpp
// Guard all TFT_BL pin operations:
#if defined(TFT_BL) && TFT_BL >= 0
  pinMode(TFT_BL, OUTPUT);
#endif
// Same guard in backlightOn() / backlightOff()

// Removed while(!Serial) after Serial.begin()
// That guard is for USB CDC only. On UART0 it hangs when monitor connects via DTR.
```

## Debugging Reference
| Symptom | Cause | Fix |
|---------|-------|-----|
| White screen on serial monitor connect | monitor DTR holding EN pin low | `monitor_dtr=0` / `monitor_rts=0` in platformio.ini |
| Stuck at "Initializing..." | GPS Serial2.begin() stole TFT_SCLK (GPIO3) | disable HAS_GPS |
| Stuck at "No IP5306 found" | Wire.endTransmission() hangs after I2C init fails | disable HAS_BATTERY |
| Colors cyan/magenta/yellow | TFT_INVERSION_ON sent INVON to non-inverted panel | switch to TFT_INVERSION_OFF |
| PSRAM not detected at boot | default board targets QD PSRAM, board has OPI | add `qio_opi` memory_type |
| gpio_set_level(226) error | TFT_BL=-1 cast to uint8_t = 255, then wraps | guard with `#if TFT_BL >= 0` |
| White screen of death on flash | wrong binary arch (ESP32 bin on S3) | always build from source with this config |

## Build Commands
```bash
~/.platformio/penv/bin/pio run -e cyd28_s3 --target upload   # full Marauder
~/.platformio/penv/bin/pio run -e tft_diag --target upload    # minimal TFT diagnostic
~/.platformio/penv/bin/pio device monitor -e cyd28_s3         # serial monitor (no reset)
```

## Free GPIOs for Future Peripherals (CC1101, NRF24, IR, OLED)
Not used by display/touch/PSRAM/USB/UART:
`4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 38, 39, 40`
- Avoid 19/20 (USB OTG), 43/44 (UART0), 48 (WS2812), 35/36/37 (PSRAM)
