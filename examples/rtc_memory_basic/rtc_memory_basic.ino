/******************************************************
 * A simple sketch to demostrate functionalities 
 * offered by RTC Memory Library.
 ******************************************************/
#include <rtc_memory.h>

// Define a struct that map what's inside the RTC memory
// Remember that this struct must take max 508 bytes. 
typedef struct {
  int counter;
} MyData;

RtcMemory rtcMemory;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("RTC Memory - Basic");
  // This cycle is to avoid that the code starts
  // without the human intervention
  Serial.println("Press 's' to start the sketch");
  while(1){
    delay(10);
    char c = Serial.read();
    if(c == 's') break;
  }
  
  if(rtcMemory.begin()){
    Serial.println("Initialization done!");
  } else {
    Serial.println("No previous data found. The memory is reset to zeros!");
  }
  
  // Get the data
  MyData* data = rtcMemory.getData<MyData>();
  
  // Modify data
  data->counter++;
  Serial.println(String("Value to persist: ") + data->counter);
  
  // Save the data in the rtc memory
  rtcMemory.save();

  delay(10000);
  ESP.restart();
}

void loop() {}
