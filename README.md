# Simple Arduino Crypto Display

## 2021 UPDATE

![Crypto watch preview](https://user-images.githubusercontent.com/42513971/110157099-16014a00-7de8-11eb-9579-9d23f2c6eccb.JPG)

The code has been updated to work with free [Coingecko API](https://api.coingecko.com/). Besides that, a few UI tweeks on the display make it even more pleasant to watch.

All of the setup is similar to the previous version, so most of the instructions / video tutorials still apply. The main difference is that you now put your cryptos in the `cryptos.h` file. And the best for last... you can now add the logo to every displayed cryptocurrency! To do that you will need a bitmap of a 32x32 image that you put alongside coin id in `cryptos.h` file.

[COINGECKO ALL COINS ID LIST](https://api.coingecko.com/api/v3/coins/list?include_platform=false)

### Getting bitmap of your crypto's logo

There are several ways to convert a regular image into a bitmap, that you can use in this project. The general recipe for creating a logo that displays nicely I use is:

1. Download coin image 
2. Open it in GIMP
3. Scale it to 32x32
4. Change mode to indexed (it will make the picture black and white, everything that is black will be displayed on the screen)
5. Save it as .xbm file
6. Open .xbm file in text editor and copy the bit-array to your cryptos.h

***

![alt text](https://i.imgur.com/QcoTabI.png 'Display')

[![Alt text](https://img.youtube.com/vi/2IRsOFaWfho/0.jpg)](https://www.youtube.com/watch?v=2IRsOFaWfho)

[Check out the youtube video of the project!](https://www.youtube.com/watch?v=2IRsOFaWfho)

A display built with an ESP8266 that fetches live currency prices directly from the [CoinMarketCap.com](CoinMarketCap.com) API.

I've tried to make this project as easy to make as possible. There is no soldering and you also do not need to know how to code. In total it should take 10-15 minutes. The total cost is also only about $8 or so.

Crypto Wallets (If you feel like tipping, obviously no need to!)

- ETH: 0x8b4334dc87d2231745772DBaDdC5FE8E705613BE
- BTC: 14pzPrjedUyYb4iqxNA8xCPVKPcDKgeV1x

### Parts Required

- [NodeMCU ESP8266 Board](http://s.click.aliexpress.com/e/ayNNnYN)
- [OLED Display 1.3" version](http://s.click.aliexpress.com/e/EqByrzb)
- [Female-Female Dupont cables](http://s.click.aliexpress.com/e/2nq7AuR)

### 3D printed case from video:

[Thingiverse](https://www.thingiverse.com/thing:2539435)

### Wiring

It should be wired as followed

| Pin On Display      | Pin on NodeMCU           |
| ------------------- | ------------------------ |
| GND                 | G                        |
| VCC                 | 3V (some boards say 3.3V |
| SCL (Sometimes SCK) | D5                       |
| SDA                 | D3                       |

---

NOTE: Some displays have GND and VCC swapped around, always wire as above!

---

![alt text](https://i.imgur.com/jaC6E2S.jpg?1 'Warning')

![alt text](https://i.imgur.com/OrTd5SX.jpg 'Wiring')
![alt text](https://i.imgur.com/0pLIPvH.png 'Wiring to Screen')
![alt text](https://i.imgur.com/xgvBrwZ.png 'Wiring to Board')
