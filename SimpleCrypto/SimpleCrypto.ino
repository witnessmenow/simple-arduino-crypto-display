/*******************************************************************
    A project to display crypto currency prices using an ESP8266

    Main Hardware:
    - NodeMCU Development Board (Any ESP8266 dev board will work)
    - OLED I2C Display (SH1106)

    Written by Brian Lough
    https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 *******************************************************************/

// ----------------------------
// Standard Libraries - Already Installed if you have ESP8266 set up
// ----------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <CoinMarketCapApi.h>
// For Integrating with the CoinMarketCap.com API
// Available on the library manager (Search for "CoinMarket")
// https://github.com/witnessmenow/arduino-coinmarketcap-api

#include "SH1106.h"
// The driver for the OLED display
// Available on the library manager (Search for "oled ssd1306")
// https://github.com/squix78/esp8266-oled-ssd1306

#include <ArduinoJson.h>
// Required by the CoinMarketCapApi Library for parsing the response
// Available on the library manager (Search for "arduino json")
// https://github.com/squix78/esp8266-oled-ssd1306

#include <WiFiManager.h>
// For configuring the Wifi credentials without re-programing
// Availalbe on library manager (WiFiManager)
// https://github.com/tzapu/WiFiManager

#include <DoubleResetDetector.h>
// For entering Config mode by pressing reset twice
// Available on the library manager (Double Reset Detector)
// https://github.com/datacute/DoubleResetDetector


// ----------------------------
// Configurations - Update these
// ----------------------------


// Pins based on your wiring
#define SCL_PIN D5
#define SDA_PIN D3

// CoinMarketCap's limit is "no more than 10 per minute"
// Make sure to factor in if you are requesting more than one coin.
// We'll request a new value just before we change the screen so it's the most up to date
unsigned long screenChangeDelay = 10000; // Every 10 seconds

// Have tested up to 10, can probably do more
#define MAX_HOLDINGS 10

#define CURRENCY "eur" //See CoinMarketCap.com for currency options (usd, gbp etc)
#define CURRENCY_SYMBOL "E" // Euro doesn't seem to work, $ and £ do

// You also need to add your crypto currecnies in the setup function

// ----------------------------
// End of area you need to change
// ----------------------------


WiFiClientSecure client;
CoinMarketCapApi api(client);

SH1106 display(0x3c, SDA_PIN, SCL_PIN);

DoubleResetDetector drd(10, 0);

ESP8266WebServer server(80);

unsigned long screenChangeDue;

struct Holding {
  String tickerId;
  float amount;
  bool inUse;
  CMCTickerResponse lastResponse;
};

Holding holdings[MAX_HOLDINGS];

int currentIndex = -1;
bool doubleResetFlag;
String ipAddressString;

// Callback notifying us that config mode has been entered
void onEnterConfigMode (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  String configModeMessage = "Connect to the \"" + String(myWiFiManager->getConfigPortalSSID());
  configModeMessage.concat("\" network to enter your WiFi details, they will be saved for next time!");
  displayMessage(configModeMessage);
  drd.stop();
}

void addNewHolding(String tickerId, float amount = 0) {
  int index = getNextFreeHoldingIndex();
  if (index > -1) {
    holdings[index].tickerId = tickerId;
    holdings[index].amount = amount;
    holdings[index].inUse = true;
  }
}

void setup() {

  Serial.begin(115200);

  // Invoking the DRD library
  doubleResetFlag = drd.detectDoubleReset();

  // ----------------------------
  // Holdings - Add your currencies here
  // ----------------------------
  // Go to the currencies coinmarketcap.com page
  // and take the tickerId from the URL (use bitcoin or ethereum as an example)
  
  addNewHolding("bitcoin");
  addNewHolding("dogecoin");
  addNewHolding("ethereum");

  // ----------------------------
  // Everything below can be thinkered with if you want but should work as is!
  // ----------------------------

  // Initialising the display
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "HODL Display");
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 18, "By Brian Lough");
  display.display();
  

  WiFiManager wifiManager;
  wifiManager.setAPCallback(onEnterConfigMode);

  if (doubleResetFlag) {
    Serial.println("Double Reset Detected");
    wifiManager.startConfigPortal("HodlDisplay", "hodltothemoon");
  } else {
    Serial.println("No Double Reset Detected");
    wifiManager.autoConnect("HodlDisplay", "hodltothemoon");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  ipAddressString = ip.toString();

  drd.stop();
}

int getNextFreeHoldingIndex() {
  for (int i = 0; i < MAX_HOLDINGS; i++) {
    if (!holdings[i].inUse) {
      return i;
    }
  }

  return -1;
}

int getNextIndex() {
  for (int i = currentIndex + 1; i < MAX_HOLDINGS; i++) {
    if (holdings[i].inUse) {
      return i;
    }
  }

  for (int j = 0; j <= currentIndex; j++) {
    if (holdings[j].inUse) {
      return j;
    }
  }

  return -1;
}

void displayHolding(int index) {

  CMCTickerResponse response = holdings[index].lastResponse;

  display.clear();

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, response.symbol);
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 20, formatCurrency(response.price_usd));
  display.setFont(ArialMT_Plain_16);
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
//  display.drawString(0, 48, " 1h:" + String(response.percent_change_1h));
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 48, "24h: " + String(response.percent_change_24h) + "%");

  display.display();
}

void displayMessage(String message){
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 0, 128, message);
  display.display();
}

String formatCurrency(float price) {
  String formattedCurrency = CURRENCY_SYMBOL;
  int pointsAfterDecimal = 6;
  if (price > 100) {
    pointsAfterDecimal = 2;
  } else if (price > 1) {
    pointsAfterDecimal = 4;
  }
  formattedCurrency.concat(String(price, pointsAfterDecimal));
  return formattedCurrency;
}

bool loadDataForHolding(int index) {
  int nextIndex = getNextIndex();
  if (nextIndex > -1 ) {
    holdings[index].lastResponse = api.GetTickerInfo(holdings[index].tickerId, CURRENCY);
    return holdings[index].lastResponse.error == "";
  }

  return false;
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > screenChangeDue))  {
    currentIndex = getNextIndex();
    if (currentIndex > -1) {
      if (loadDataForHolding(currentIndex)) {
        displayHolding(currentIndex);
      } else {
        // Display error?
      }
    } else {
      displayMessage(F("No funds to display. Edit the setup to add them"));
    }
    screenChangeDue = timeNow + screenChangeDelay;
  }

  server.handleClient();
}
