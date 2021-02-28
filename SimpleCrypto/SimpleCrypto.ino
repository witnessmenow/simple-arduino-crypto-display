#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include "SH1106.h"

char ssid[] = "WiFiNetworkName";
char password[] = "password";

// Pins based on your wiring
#define SCL_PIN D5
#define SDA_PIN D3

unsigned long screenChangeDelay = 10000;
SH1106 display(0x3c, SDA_PIN, SCL_PIN);


void setup() {
  Serial.begin(115200);
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, F("HODL Display"));
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 18, F("By Brian Lough"));
  display.drawString(64, 36, F("/ updated by piotrwawrzyn"));
  display.display();
}

void loop() {

}