/******************************************************
 * A simple sketch to demostrate functionalities 
 * offered by RTC Memory Library.
 ******************************************************/
#include <rtc_memory.h>
#include <FS.h>

// Define a struct that map what's inside the RTC memory
// Remember that this struct must take max 508 bytes. 
typedef struct {
  int counter;
} MyData;

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

  // Remember to initialize the SPIFFS memory
  Serial.print("Filesystem initialization... ");
  if(SPIFFS.begin()){
    Serial.println("Done!");
  }else{
    Serial.println("Error");
  }
  
  RtcMemory rtcMem("/etc/trial.bin");
  MyData* data = rtcMem.getData<MyData>();
  
  if (data == nullptr) {
    Serial.println("Error: In this case nullptr is correct, you have to explicitly initialize the class!");
  }

  if(rtcMem.begin()){
    Serial.println("Initialization done!");
  } else {
    Serial.println("Initialization error!");
  }
  
  // Get the data
  data = rtcMem.getData<MyData>();
  // Modify some data
  data->counter++;
  Serial.println(String("Value to persist: ") + data->counter);
  
  // Save the data in the rtc memory
  rtcMem.save();

  // Persist the data in rtc mem and in flash
  rtcMem.persist();

  delay(10000);
  ESP.restart();
}

void loop() {

}
