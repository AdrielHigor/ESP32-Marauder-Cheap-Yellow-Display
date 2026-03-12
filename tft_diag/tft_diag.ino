// Minimal TFT diagnostic — no Marauder, no WiFi, no libraries except TFT_eSPI
// If any color appears on screen → SPI is working, Marauder setup is the issue
// If screen stays white → wrong GPIO pins for this board
//
// RGB LED: R=GPIO4  G=GPIO16  B=GPIO17  (active LOW on CYD)
// TFT BL tested: GPIO21 (CYD2USB) then GPIO27 (standard CYD)

#include <TFT_eSPI.h>

TFT_eSPI tft;

#define LED_R 4
#define LED_G 16
#define LED_B 17

void setup() {
  // RGB LED init — active LOW
  pinMode(LED_R, OUTPUT); digitalWrite(LED_R, LOW);   // red = alive
  pinMode(LED_G, OUTPUT); digitalWrite(LED_G, HIGH);
  pinMode(LED_B, OUTPUT); digitalWrite(LED_B, HIGH);

  delay(500);

  // Try backlight on GPIO21 first
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  // Also try GPIO27 (some CYD variants)
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  // Init TFT
  tft.init();
  tft.setRotation(0);

  // Flash three colors so we can see if ANY command gets through
  tft.fillScreen(TFT_RED);   delay(800);
  tft.fillScreen(TFT_GREEN); delay(800);
  tft.fillScreen(TFT_BLUE);  delay(800);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 100);
  tft.println("TFT OK");
  tft.println("Pins: MOSI=13");
  tft.println("SCK=14 CS=15");
  tft.println("DC=2  BL=21/27");

  // Green = done
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
}

void loop() {
  // Slow blue blink = stuck in loop (setup completed)
  digitalWrite(LED_B, LOW);  delay(500);
  digitalWrite(LED_B, HIGH); delay(500);
}
