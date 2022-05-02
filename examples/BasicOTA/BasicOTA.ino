/***********************************************************************
 * This sketch shows how to integrate RTCMemory with OTA update.
 * Basically, it is the straight merge of the OTA update example
 * provided in ESP8266 Arduino Core and Basic example of this library.
 ***********************************************************************/
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <RTCMemory.h>

const char* ssid = "YOUR-SSID";
const char* password = "YOUR-PASSWORD";

struct MyData {
  int counter;
};

RTCMemory<MyData> rtcMemory;

void setup() {
  Serial.begin(115200);
  Serial.println("RTCMemory - Basic with OTA update");

  // Change this value before an OTA update to observe if the update succeeds
  Serial.println("v1");

  rtcMemory.begin();
  MyData* data = rtcMemory.getData();

  data->counter++;

  Serial.println(data->counter);

  rtcMemory.save();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
}
