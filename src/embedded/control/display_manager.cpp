#include "state.h"
#include "display_manager.h"

void initDisplay() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
}

void drawAPScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("WiFi Setup Mode");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 50);
  tft.println("Connect to AP:");

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(10, 70);
  tft.println("RoomLightingSystemAP");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 100);
  tft.println("Password:");

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 120);
  if (apPassword != nullptr) tft.println(apPassword);
  else tft.println("ERROR");
}

void drawStatusScreen() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("System Status");

  tft.setTextSize(2);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 60);
  tft.print("Lights: ");
  tft.setTextColor(lightsState ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(lightsState ? "ON" : "OFF");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 100);
  tft.print("Blinds: ");
  tft.setTextColor(blindsState ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.println(blindsState ? "OPEN" : "CLOSED");

  // tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // tft.setCursor(10, 140);
  // tft.print("Night Mode: ");
  // tft.setTextColor(nightModeState ? TFT_GREEN : TFT_RED, TFT_BLACK);
  // tft.println(nightModeState ? "ON" : "OFF");
}

void updateDisplay() {
  if (apModeActive) drawAPScreen();
  else drawStatusScreen();
}