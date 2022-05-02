/********************************************************************
 * This sketch shows how to store and retrieve a variable from
 * RTC memory and how to backup it on flash memory.
 ********************************************************************/
#include <RTCMemory.h>

// Define a struct that maps what's inside the RTC memory
// Max size is 508 bytes.
typedef struct {
  int counter;
} MyData;

RTCMemory<MyData> rtcMemory("/etc/rtc_mem.bin");

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("RTCMemory - Basic with Backup");

  // Stop the sketch until serial monitor is opened and user presses 's'
  Serial.println("Press 's' to start the sketch");
  while (1) {
    delay(10);
    char c = Serial.read();
    if (c == 's') break;
  }

  // Remember to initialize the flash memory
  Serial.print("Filesystem initialization... ");
  if (LittleFS.begin()) {
    Serial.println("Done!");
  } else {
    Serial.println("Error");
  }

  MyData *data = rtcMemory.getData();

  if (data == nullptr) {
    Serial.println("OK: here nullptr is expected, you have to initialize the instance explicitly");
  }

  if (rtcMemory.begin()) {
    Serial.println("Initialization done! Previous data found.");
  } else {
    Serial.println("Initialization done! No previous data found. The buffer is cleared.");
    // Here you can initialize your data structure.
  }

  // Get the data
  data = rtcMemory.getData();
  Serial.println(String("Value read: ") + data->counter);

  // Modify data
  data->counter++;
  Serial.println(String("Value to save: ") + data->counter);

  // Persist data in RTC and in flash memory
  // It preserves data even on power loss
  rtcMemory.persist();

  delay(10000);
  ESP.restart();
}

void loop() {}
