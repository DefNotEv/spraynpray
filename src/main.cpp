#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <pwmWrite.h>
#include <CheapStepper.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <SPIFFS.h>
#include <FS.h>

#define FORMAT_SPIFFS_IF_FAILED true

const char *DATABASE_FILE = "/database.json";
const char* ssid = "PokerHacks";  // Enter SSID here
const char* password = "PokerHacks";  //Enter Password here
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);


// RTC_DS3231 rtc;
// Pwm pwm = Pwm();
CheapStepper stepper(18, 19, 21, 23);
int a = 0;



bool setData(const StaticJsonDocument<1024> &jsonDoc)
{
  // Open the file for writing
  File file = SPIFFS.open(DATABASE_FILE, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }

  // Serialize the JSON document and write to the file
  if (serializeJson(jsonDoc, file) == 0)
  {
    Serial.println("Failed to write JSON data to file");
    file.close();
    return false;
  }

  file.close();
  return true;
}

StaticJsonDocument<1024> getData()
{
  StaticJsonDocument<1024> jsonDoc;

  File file = SPIFFS.open(DATABASE_FILE, "r");
  if (!file || file.isDirectory())
  {
    // File not found or is a directory, return an error message
    jsonDoc["error"] = "File not found";
  }
  else
  {
    // Read the file content
    String fileContent = file.readString();
    file.close();

    // Deserialize the JSON data
    DeserializationError error = deserializeJson(jsonDoc, fileContent);
    if (error)
    {
      jsonDoc["error"] = "Failed to parse JSON";
    }
  }

  return jsonDoc;
}

void getAlarms()
{
  String jso;
  StaticJsonDocument<1024> doc = getData();
  deserializeJson(doc, jso);
  Serial.println(jso);
  Serial.println("GET: /alarms");
  server.send(200, "application/json", jso);
}

// replace all the data
void putAlarms()
{
  String body = server.arg("plain");
  StaticJsonDocument<1024> jsonDoc;

  deserializeJson(jsonDoc, body);
  setData(jsonDoc);
  Serial.println("PUT: /alarms");
  server.send(200, "application/json", "{\"message\": \"woohoo!\"}");
}

// add a single new alarm
void postAlarms()
{
  Serial.println("POST: /alarms");
  String requestBody = server.arg("plain");

  sleep(180);

  stepper.moveDegreesCW(1440);


  // StaticJsonDocument<1024> newAlarmDoc;
  // DeserializationError error = deserializeJson(newAlarmDoc, requestBody);
  // if (error)
  // {
  //   server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
  //   return;
  // }

  // // Validate the incoming JSON
  // if (!newAlarmDoc.containsKey("time") || !newAlarmDoc.containsKey("days"))
  // {
  //   server.send(400, "application/json", "{\"error\":\"Invalid JSON structure\"}");
  //   return;
  // }

  // // Open and read the existing database.json file
  // StaticJsonDocument<1024> jsonDoc = getData();
  // JsonArray alarmsArray = jsonDoc["alarms"].as<JsonArray>();

  // // Add the new alarm to the existing alarms array
  // JsonObject newAlarm = alarmsArray.createNestedObject();
  // newAlarm["time"] = newAlarmDoc["time"].as<String>();
  // JsonArray daysArray = newAlarm.createNestedArray("days");
  // for (JsonVariant day : newAlarmDoc["days"].as<JsonArray>())
  // {
  //   daysArray.add(day.as<bool>());
  // }

  // // Write the updated JSON back to the file
  // File file = SPIFFS.open("/database.json", "w");
  // if (!file)
  // {
  //   server.send(500, "application/json", "{\"error\":\"Failed to open file for writing\"}");
  //   return;
  // }

  // serializeJson(jsonDoc, file);
  // file.close();

  // Respond to the client
  server.send(200, "application/json", "{\"status\":\"Alarm added successfully\"}");
}


void setup()
{
  Serial.begin(9600);
  Serial.println();

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // setupApi();
  server.on("/alarms", HTTP_GET, getAlarms);
  server.on("/alarms", HTTP_PUT, putAlarms);
server.on("/alarms", HTTP_POST, postAlarms);

  server.begin();
  Serial.println("Server started");

  stepper.setRpm(16);
  // Serial.println(stepper.getRpm());

  if (!SPIFFS.begin(true))
  {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
  // TODO: Error checking on rtc, ensure its there and not error
  // rtc.begin();
  // rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
  // File file = SPIFFS.open(DATABASE_FILE, "r");
  //       // Read the file content
  //       String fileContent = file.readString();
  //       file.close();

  // Serial.println(fileContent);
  // sleep(5);

  server.handleClient();
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