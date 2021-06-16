/******************************************************
 * This sketch shows how to use the RTC memory
 * without backup on flash memory. You will lose data
 * on RTC memory on power loss.
 ******************************************************/
#include <rtc_memory.h>

// Define a struct that maps what's inside the RTC memory
// Max size is 508 bytes.
typedef struct {
  int counter;
} MyData;

RtcMemory rtcMemory;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("RTC Memory - Basic");
  // This cycle is to avoid that the code starts when serial monitor is not
  // opened
  Serial.println("Press 's' to start the sketch");
  while (1) {
    delay(10);
    char c = Serial.read();
    if (c == 's')
      break;
  }

  if (rtcMemory.begin()) {
    Serial.println("Initialization done!");
  } else {
    Serial.println("No previous data found. The memory is reset to zeros!");
  }

  // Get data
  MyData *data = rtcMemory.getData<MyData>();

  // Modify data
  data->counter++;
  Serial.println(String("Value to persist: ") + data->counter);

  // Save data
  rtcMemory.save();

  delay(10000);
  ESP.restart();
}

void loop() {}
