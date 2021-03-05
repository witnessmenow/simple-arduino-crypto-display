#define ARDUINOJSON_USE_DOUBLE 1
#define ARDUINOJSON_USE_LONG_LONG 1

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WifiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SH1106.h
#include "cryptos.h"

// ----------------------------
// Configurations - Update these
// ----------------------------

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
unsigned long screenChangeDelay = 10000;

#define SCL_PIN D5
#define SDA_PIN D3

// ----------------------------
// End of area you need to change
// ----------------------------

SH1106 display(0x3c, SDA_PIN, SCL_PIN);
unsigned long screenChangeDue;
int cryptosCount = (sizeof(cryptos) / sizeof(cryptos[0]));

void renderSimpleText(String text) {
  display.clear();
  display.drawString(64, 18, text);
  display.display();
}

void renderWelcomeMsg() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, F("HODL Display"));
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 18, F("By Brian Lough"));
  display.drawString(64, 36, F("/ updated by piotrwawrzyn"));
  display.display();
}

void connect() {
  WiFi.begin(ssid, password);
  String dots[3] = {".", "..", "..."};
  int numberOfDots = 1;
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 18, "Connecting to WiFi " + dots[numberOfDots]);
    display.display();

    if (numberOfDots == 3) {
      numberOfDots = 0;
    } else {
      numberOfDots++;
    }

    delay(300);
  }

   renderSimpleText("Connected.");
}

String createApiUrl(String vsCurrency) {
  String cryptosString = "";

  for (int i = 0; i < cryptosCount; i++) {
    cryptosString += cryptos[i].apiName + ",";
  }

  return "https://api.coingecko.com/api/v3/coins/markets?vs_currency=" + vsCurrency + "&ids=" + cryptosString + "&order=market_cap_desc&per_page=100&page=1&sparkline=false&price_change_percentage=24h%2C7d";
}

int getCryptoIndexById(String id) {
  for (int i = 0; i < cryptosCount; i++) {
    if (cryptos[i].apiName == id) return i;
  }
}

void downloadData(String vsCurrency) {
  HTTPClient http;          
  WiFiClientSecure client;
  client.setInsecure(); 

  String apiUrl = createApiUrl(vsCurrency);
  
  client.connect(apiUrl, 443);
  http.begin(client, apiUrl);

  String response;
  if (http.GET() == HTTP_CODE_OK) {
    response = http.getString();  
  } else {
    renderSimpleText("Error connecting to API");
    return;
  }

  DynamicJsonDocument doc(4000);
  DeserializationError error = deserializeJson(doc, response);
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    renderSimpleText("JSON deserialization error");
    return;
  }

  for (int i = 0; i < cryptosCount; i++) {
    JsonObject json = doc[i];
    String id = json["id"];
    int cryptoIndex = getCryptoIndexById(id);
    String symbol = json["symbol"];
    symbol.toUpperCase();
    double currentPrice = json["current_price"];
    double dayChange = json["price_change_percentage_24h_in_currency"];
    double weekChange = json["price_change_percentage_7d_in_currency"];
    
    cryptos[cryptoIndex].symbol = symbol;
    cryptos[cryptoIndex].priceUsd = currentPrice;
    cryptos[cryptoIndex].dayChange = dayChange;
    cryptos[cryptoIndex].weekChange = weekChange;
  }
 
}

String formatCurrency(float price) {
  int digitsAfterDecimal = 3;
  
  if (price >= 1000) {
    digitsAfterDecimal = 0;
  } else if (price >= 100) {
    digitsAfterDecimal = 1;
  } else if (price >= 1) {
    digitsAfterDecimal = 2;
  } else if (price < 0.001) {
    digitsAfterDecimal = 4;
  } 
  
  return String(price, digitsAfterDecimal);
}

void renderCryptoLogo(Crypto crypto) {  
  int offX = 90;
  int offY = 4;
  int width = 32;
  int height = 32;
  
  display.drawXbm(offX, offY, width, height, (uint8_t*)( crypto.bitmap ));
}  

void renderCrypto(Crypto crypto) {
  display.clear();
  renderCryptoLogo(crypto);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(90, 45, crypto.symbol);
  display.drawString(0, 10, "$");
  display.setFont(ArialMT_Plain_24);
  display.drawString(10, 6, formatCurrency(crypto.priceUsd));
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 42, "24h:  " + String(crypto.dayChange) + "%");
  display.drawString(0, 54, "7d:  " + String(crypto.weekChange) + "%");
  display.display();
}




void setup() {
  Serial.begin(115200);
  display.init();
  renderWelcomeMsg();
  delay(4000);
  connect();
}

void loop() {
  downloadData("usd");
    
  for (int i = 0; i < cryptosCount; i++) {
    renderCrypto(cryptos[i]);
    delay(screenChangeDelay);
  }
}
