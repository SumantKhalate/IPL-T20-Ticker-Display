# **IPL T20 Ticker Display**

---

![Image](/Images/IMG_1.jpg)

It's IPL T20 time over here in India and I thought of building a small ticker display for it.

Initially, I thought of using some API to get the stats however I couldn't find an API that is free to use and provides granular control over the data that is being requested, as ESP8266 doesn't have enough memory to handle such a huge response.

So I had to settle for using the [ThingSpeak](https://thingspeak.com/) thingHTTP app, which scrapes the ESPN Cricinfo web page's required data using its XPath reference.
And then make HTTP GET requests to this previously defined thingHTTP apps and display it over the OLED.

It uses an [Adafruit Feather HUZZAH](https://www.adafruit.com/product/2821) with [Adafruit Feather 128x32 OLED](https://www.adafruit.com/product/2900) add-on and makes a request every 10s.

---

### Note:

Please create `credentials.h` and `APIKey.h` files in the root folder with below data

```C++
// credentials.h
// credentials to connect to the Wi-Fi

#define   mySSID      "yourSSID"
#define   myPASSWORD  "yourPASSWORD"
```

```C++
// APIKey.h
// API keys to access the thingHTTP app endpoint, can be multiple endpoints.

String APIKeys[] = {
  "ABCMNPOXYZ123789",
  "ABCMNPOXYZ123789"
};
```

thingHTTP app screenshot

![thingHTTP](/Images/thingHTTP.jpg)
