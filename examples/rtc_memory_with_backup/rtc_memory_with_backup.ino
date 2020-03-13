/******************************************************
 * A simple sketch to demostrate how to persist (backup) 
 * data on Flash memory.
 ******************************************************/
#include <rtc_memory.h>
#include <FS.h>

// Define a struct that map what's inside the RTC memory
// Remember that this struct must take max 508 bytes. 
typedef struct {
  int counter;
} MyData;

RtcMemory rtcMemory("/etc/trial.bin");

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("RTC Memory - Basic with Backup");
  // This cycle is to avoid that the code starts
  // without the human intervention
  Serial.println("Press 's' to start the sketch");
  while(1){
    delay(10);
    char c = Serial.read();
    if(c == 's') break;
  }

  // Remember to initialize the SPIFFS memory
  Serial.print("Filesystem initialization... ");
  if(SPIFFS.begin()){
    Serial.println("Done!");
  }else{
    Serial.println("Error");
  }
  
  MyData* data = rtcMemory.getData<MyData>();
  
  if (data == nullptr) {
    Serial.println("Error: In this case nullptr is correct, you have to explicitly initialize the class!");
  }

  if(rtcMemory.begin()){
    Serial.println("Initialization done!");
  } else {
    Serial.println("No previous data found. The memory is reset to zeros!");
    // Here you can initialize your data structure.
  }
  
  // Get the data
  data = rtcMemory.getData<MyData>();
  Serial.println(String("Value read: ") + data->counter);
  
  // Modify some data
  data->counter++;
  Serial.println(String("Incremented value: ") + data->counter);

  // Persist the data in RTC and in flash memory
  // You will get back the data even if you turn off your device.
  rtcMemory.persist();

  delay(10000);
  ESP.restart();
}

void loop() {}
