/******************************************************
 * A simple sketch to demostrate functionalities 
 * offered by RTC Memory Library.
 * 
 * This includes a couple of benchmark to show 
 * how much time is spend in init/read/write memories.
 ******************************************************/
#include <rtc_memory.h>
#include <FS.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);

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
  
  int start, end;
  
  RtcMemory rtcMem("/etc/trial.bin", 2);
  byte* data = rtcMem.getRtcData();
  if(data==nullptr){
    Serial.println("Error: In this case nullptr is correct, you have to explicitly initialize the class");
  }
  start=micros();
  rtcMem.begin();
  end=micros();
  Serial.println(String("The initilization had taken: ") + (end-start) + "us");

  // Get the data
  data=rtcMem.getRtcData();
  // Modify some data
  data[0]++;
  Serial.println(String("Value to persist: ") + data[0]);
  
  //Save the data in the rtc memory
  start=micros();
  rtcMem.save();
  end=micros();
  Serial.println(String("The saving on rtc memory had taken: ") + (end-start) + "us");

  // Persist the data in rtc mem and in flash
  start=micros();
  rtcMem.persist();
  end=micros();
  Serial.println(String("The persist operation (rtc mem + flash) had taken: ") + (end-start) + "us");

  delay(10000);
  ESP.restart();
}

void loop() {

}
