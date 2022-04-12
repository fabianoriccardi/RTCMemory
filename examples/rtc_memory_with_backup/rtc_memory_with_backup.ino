/******************************************************
 * This sketch shows how to use the RTC memory and how
 * to backup and retrieve data from flash memory.
 ******************************************************/
#include <FS.h>
#include <rtc_memory.h>

// Define a struct that maps what's inside the RTC memory
// Max size is 508 bytes.
typedef struct {
  int counter;
} MyData;

RtcMemory rtcMemory("/etc/trial.bin");

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("RTCMemory - Basic with Backup");
  // This cycle is to avoid that the code starts when serial monitor is not
  // opened
  Serial.println("Press 's' to start the sketch");
  while (1) {
    delay(10);
    char c = Serial.read();
    if (c == 's') break;
  }

  // Remember to initialize the SPIFFS memory
  Serial.print("Filesystem initialization... ");
  if (SPIFFS.begin()) {
    Serial.println("Done!");
  } else {
    Serial.println("Error");
  }

  MyData *data = rtcMemory.getData<MyData>();

  if (data == nullptr) {
    Serial.println("Error: In this case nullptr is correct, you have to "
                   "explicitly initialize the class!");
  }

  if (rtcMemory.begin()) {
    Serial.println("Initialization done!");
  } else {
    Serial.println("No previous data found. The memory is reset to zeros!");
    // Here you can initialize your data structure.
  }

  // Get the data
  data = rtcMemory.getData<MyData>();
  Serial.println(String("Value read: ") + data->counter);

  // Modify data
  data->counter++;
  Serial.println(String("Incremented value: ") + data->counter);

  // Persist data in RTC and in flash memory
  // It preserves data even on power loss
  rtcMemory.persist();

  delay(10000);
  ESP.restart();
}

void loop() {}
