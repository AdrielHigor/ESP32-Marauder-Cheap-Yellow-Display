// Minimal TFT diagnostic — ESP32-S3-CAM N16R8 RE 1.3
// Correct pins from manufacturer schematic:
//   MOSI=45  SCLK=3  CS=14  DC=47  RST=21  BL=hardwired(3.3V)
//   Touch: CS=1 MOSI=2 MISO=41 CLK=42 (SoftSPI)
//   WS2812 RGB LED: GPIO48
//
// If any color appears → SPI wiring confirmed correct
// If screen stays white → driver type wrong or SPI bus issue

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft;

void setup() {
  Serial.begin(115200);
  Serial.println("TFT diag start");

  tft.init();
  Serial.println("tft.init() done");

  tft.setRotation(1);

  tft.fillScreen(TFT_RED);   Serial.println("RED"); delay(1000);
  tft.fillScreen(TFT_GREEN); Serial.println("GREEN"); delay(1000);
  tft.fillScreen(TFT_BLUE);  Serial.println("BLUE"); delay(1000);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TFT OK!");
  tft.println("MOSI=45 SCLK=3");
  tft.println("CS=14  DC=47");
  tft.println("RST=21");

  Serial.println("Setup done");
}

void loop() {
  delay(1000);
}
