# Simple Arduino Crypto Display

A display built with an ESP8266 that fetches live currency prices directly from the [CoinMarketCap.com](CoinMarketCap.com) API.

I've tried to make this project as easy to make as possible. There is no soldering and you also do not need to know how to code. In total it should take 10-15 minutes

### Parts Required

* [NodeMCU ESP8266 Board](http://s.click.aliexpress.com/e/ayNNnYN)
* [OLED Display 1.3" version](http://s.click.aliexpress.com/e/EqByrzb)
* [Female-Female Dupont cables](http://s.click.aliexpress.com/e/2nq7AuR)


### Wiring 

It should be wired as followed

| Pin On Display| Pin on NodeMCU |
| ------------- |----------------|
| GND     | G |
| VCC     | 3V (some boards say 3.3V |
| SCK     | D5 |
| SDA     | D3 |

---

NOTE: Some displays have GND and VCC swapped around, always wire as above!

---
![alt text](https://i.imgur.com/jaC6E2S.jpg?1 "Warning")


![alt text](https://i.imgur.com/OrTd5SX.jpg "Wiring")
![alt text](https://i.imgur.com/0pLIPvH.png "Wiring to Screen")
![alt text](https://i.imgur.com/xgvBrwZ.png "Wiring to Board")
