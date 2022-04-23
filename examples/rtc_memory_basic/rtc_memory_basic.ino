/*******************************************************************************
 * This sketch shows the minimal usage of RTCMemory.
 * NOTE: You will lose data on RTC memory if the device is not powered.
 *******************************************************************************/
#include <RTCMemory.h>

// Define a struct that maps what's inside the RTC memory
// Max size is 508 bytes.
typedef struct {
  int counter;
} MyData;

RTCMemory<MyData> rtcMemory;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("RTCMemory - Basic");

  // Stop the sketch until serial monitor is opened and user presses 's'
  Serial.println("Press 's' to start the sketch");
  while (1) {
    delay(10);
    char c = Serial.read();
    if (c == 's') break;
  }

  if (rtcMemory.begin()) {
    Serial.println("Initialization done! Previous data found.");
  } else {
    Serial.println("Initialization done! No previous data found. The buffer is cleared.");
    // Here you can initialize your data structure
  }

  // Get data
  MyData *data = rtcMemory.getData();
  Serial.println(String("Value read: ") + data->counter);

  // Modify data
  data->counter++;
  Serial.println(String("Value to save: ") + data->counter);

  // Save data
  rtcMemory.save();

  delay(10000);
  ESP.restart();
}

void loop() {}
