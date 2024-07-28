#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <pwmWrite.h>
#include <WiFi.h>
#include <CheapStepper.h>
// #include <WebServer.h>
// #include <ArduinoJson.h>
// #include <WiFiManager.h>
// #include <SPIFFS.h>
// #include <FS.h>

#define FORMAT_SPIFFS_IF_FAILED true

const char *DATABASE_FILE = "/database.json";
const char* ssid = "PokerHacks";  // Enter SSID here
const char* password = "PokerHacks";  //Enter Password here
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// RTC_DS3231 rtc;
// Pwm pwm = Pwm();
CheapStepper stepper(18, 19, 21, 23);
int a = 0;



void setup()
{
  Serial.begin(9600);
  Serial.println();

  stepper.setRpm(10);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // TODO: Error checking on rtc, ensure its there and not error
  // rtc.begin();
  // rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
  stepper.moveDegreesCW(360);
  // bool val = digitalRead(5);

  // Serial.printf("%d\naaa", val);
  // if (val)
  // {
  //   digitalWrite(4, HIGH);
  //   Serial.println("Turn on relay");
  // }
  // else
  // {
  //   digitalWrite(4, LOW);
  //   Serial.println("Turn off relay");
  // }

  // DateTime now = rtc.now();

  // Serial.printf("%d:%d:%d\n", now.hour(), now.minute(), now.second());

  // sleep(1);
}