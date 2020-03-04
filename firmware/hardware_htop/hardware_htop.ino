#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

#define RAM_Pin 13
#define HDD_Pin 15

#define CORE_01_Pin 2
#define CORE_02_Pin 0
#define CORE_03_Pin 4
#define CORE_04_Pin 16
#define CORE_05_Pin 17
#define CORE_06_Pin 5
#define CORE_07_Pin 18
#define CORE_08_Pin 23

#define BODY_BUFFER_SIZE 1024

 
// SSID & Password
const char* ssid = "*****";         // Enter your SSID here
const char* password = "*******";  //Enter your Password here
 
AsyncWebServer server(80);
 
void setup() {

  ledcAttachPin(RAM_Pin, 0);
  ledcAttachPin(HDD_Pin, 1);
  ledcAttachPin(CORE_01_Pin, 2);
  ledcAttachPin(CORE_02_Pin, 3);
  ledcAttachPin(CORE_03_Pin, 4);
  ledcAttachPin(CORE_04_Pin, 5);
  ledcAttachPin(CORE_05_Pin, 6);
  ledcAttachPin(CORE_06_Pin, 7);
  ledcAttachPin(CORE_07_Pin, 8);
  ledcAttachPin(CORE_08_Pin, 9);

  for(int n = 0; n <= 9; n++) {
    ledcSetup(n, 4000, 8);
    ledcWrite(n, 0);
  }

  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 
  server.on(
    "/post",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

      DynamicJsonDocument doc(BODY_BUFFER_SIZE);
      auto error = deserializeJson(doc, data);
      if (error) {
          AsyncWebServerResponse *response = request->beginResponse(500, "text/plain", error.c_str());
          request->send(response);
      }
 
      int core01  = (int)((float)(doc["CORE01"])*(255.0/100.0));
      int core02  = (int)((float)(doc["CORE02"])*(255.0/100.0));
      int core03  = (int)((float)(doc["CORE03"])*(255.0/100.0));
      int core04  = (int)((float)(doc["CORE04"])*(255.0/100.0));
      int core05  = (int)((float)(doc["CORE05"])*(255.0/100.0));
      int core06  = (int)((float)(doc["CORE06"])*(255.0/100.0));
      int core07  = (int)((float)(doc["CORE07"])*(255.0/100.0));
      int core08  = (int)((float)(doc["CORE08"])*(255.0/100.0));
      int memLoad = (int)((float)(doc["MEMORY_LOAD"])*(255.0/100.0));
      int hddLoad = (int)((float)(doc["HDD_LOAD"])*(255.0/100.0));

      Serial.println("-------------------------------");
      Serial.println("CORE01 >> " + String(core01));
      Serial.println("CORE02 >> " + String(core02));
      Serial.println("CORE03 >> " + String(core03));
      Serial.println("CORE04 >> " + String(core04));
      Serial.println("CORE05 >> " + String(core05));
      Serial.println("CORE06 >> " + String(core06));
      Serial.println("CORE07 >> " + String(core07));
      Serial.println("CORE08 >> " + String(core08));
      Serial.println("RAM    >> " + String(memLoad));
      Serial.println("HDD    >> " + String(hddLoad));
      Serial.println("-------------------------------");

      ledcWrite(0, memLoad);
      ledcWrite(1, hddLoad);
      ledcWrite(2, core01);
      ledcWrite(3, core02);
      ledcWrite(4, core03);
      ledcWrite(5, core04);
      ledcWrite(6, core05);
      ledcWrite(7, core06);
      ledcWrite(8, core07);
      ledcWrite(9, core08);
 
      request->send(200);
  });
 
  server.begin();
  
  Serial.println("HTTP server started");
  delay(100); 
}
 
void loop() {}
