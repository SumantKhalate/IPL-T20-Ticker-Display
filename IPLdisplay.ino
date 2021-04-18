#include <ESP8266WiFi.h>
#include "credentials.h"
#include "APIKey.h"

const char* ssid = mySSID;
const char* password =  myPASSWORD;
WiFiClient client;


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266HTTPClient.h>
const char* host = "api.thingspeak.com";
const int httpPortRead = 80;
const char* url = "/apps/thinghttp/send_request?api_key=";

HTTPClient http;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

int countRST = 0;
int ResetPin = 16;

void setup()
{
  initBoard();
  pinMode(ResetPin, OUTPUT);
  digitalWrite(ResetPin, HIGH);

  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  display.print("Connecting to WiFi...");
  display.display();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000); Serial.println("Connecting to WiFi..");

    countRST = countRST + 1;
    if (countRST == 60)
    {
      digitalWrite(ResetPin, LOW);
      countRST = 0;
    }
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  display.println("\n\nConnected!");
  display.println(WiFi.localIP());
  display.display();
  //  delay(2500);
}


void loop()
{
  countRST = 0;
  String payload;
  String matchData[15];


  while (WiFi.status() != WL_CONNECTED)
  {
    if (countRST == 0)
    {
      WiFi.disconnect();
      delay(1000);
    }
    WiFi.begin(ssid, password);
    Serial.println("Reconnecting to WiFi..");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Reconnecting to WiFi..");
    display.display();
    delay(20000);

    countRST = countRST + 1;
    if (countRST == 15)
    {
      digitalWrite(ResetPin, LOW);  /* reset the board */
      countRST = 0;
    }
  }

  while (!client.connect(host, httpPortRead))
  {
    client.connect(host, httpPortRead);
    countRST = countRST + 1;
    delay(10000);
    if (countRST == 10)
    {
      WiFi.disconnect();
      delay(2000);
      WiFi.begin(ssid, password);
      countRST = 0;
    }
  }

  Serial.println("[HTTP] GET, in progress");

  for (byte k = 0; k < 15; k++)
  {
    String endpoint = url + APIKeys[k];
    if ( http.begin(host, httpPortRead, endpoint))
    {
      int httpCode = http.GET();
      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          payload = http.getString();
          if (payload == "Error parsing document, try a different parse string.")
            payload = "-";

          if (k == 4 || k == 8)
          {
            int len = payload.length();
            payload.remove(len - 2, 2);
          }

          if (k == 14)
          {
            int dotloc = payload.indexOf('.');
            int len = payload.length();
            payload.remove(dotloc, len);
          }

          payload =  getShortTname(payload);
        }
      }
      else
      {
        Serial.printf("[HTTP] GET, failed, error: %s\n", http.errorToString(httpCode).c_str());
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("[HTTP] GET, failed");
        display.display();
      }

      http.end();
    }

    else
    {
      Serial.println("[HTTP} Unable to connect\n");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("[HTTP} Unable to connect");
      display.display();
    }
    matchData[k] = payload;
  }

  Serial.println("\n----------------------");
  for (byte k = 0; k < 15; k++)
  {
    Serial.println(matchData[k]);
  }
  Serial.println("\n\n");

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print(matchData[0]);
  display.setCursor(25, 0);
  display.print(matchData[1]);
  display.setCursor(65, 0);
  display.print(matchData[2]);
  display.setCursor(90, 0);
  display.print(matchData[3]);

  display.setCursor(0, 10);
  display.print(matchData[4]);
  display.setCursor(90, 10);
  display.print(matchData[5]);
  display.setCursor(110, 10);
  display.print(matchData[6]);
  //  display.setCursor(x, 10);
  //  display.print(matchData[7]);

  display.setCursor(0, 20);
  display.print(matchData[8]);
  display.setCursor(90, 20);
  display.print(matchData[9]);
  display.setCursor(110, 20);
  display.print(matchData[10]);
  //  display.setCursor(x, 20);
  //  display.print(matchData[11]);
  display.display();

  delay(8000);
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("CR");
  display.setCursor(25, 0);
  display.print(matchData[12]);

  display.setCursor(65, 0);
  display.print("RR");
  display.setCursor(90, 0);
  display.print(matchData[13]);

  display.setCursor(0, 10);
  display.print(matchData[14]);

  display.display();

  //delay(5000);
}

void initBoard() {
  delay(500);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  //digitalWrite(2, LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  Serial.begin(115200);
  Serial.print("FREE HEAP: "); Serial.println(ESP.getFreeHeap());
}

String getShortTname(String Tname)
{
  if (Tname == "Kolkata Knight Riders")
    Tname = "KKR";
  else if (Tname == "Sunrisers Hyderabad")
    Tname = "SRH";
  else if (Tname == "Delhi Capitals")
    Tname = "DC";
  else if (Tname == "Royal Challengers Bangalore")
    Tname = "RCB";
  else if (Tname == "Mumbai Indians")
    Tname = "MI";
  else if (Tname == "Chennai Super Kings")
    Tname = "CSK";
  else if (Tname == "Punjab Kings")
    Tname = "PBKS";
  else if (Tname == "Rajasthan Royals")
    Tname = "RR";

  return Tname;
}
