/***************************************************************
 * Benchmarks of the functionalities offered by RTC Memory Library.
 * They enphatize the different performances between the memories
 * involved various init/read/write operations.
 ***************************************************************/
#include <rtc_memory.h>
#include <FS.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  Serial.println();
  Serial.println("RTC Memory - Benchmark");
  
  // This cycle is to avoid that the code starts
  // without the human intervention
  Serial.println("Press 's' to start the benchmark");
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
  
  unsigned long start, end;
  
  RtcMemory rtcMem("/etc/trial.bin");
  byte* data = rtcMem.getData<byte>();
  if(data == nullptr){
    Serial.println("Error: In this case nullptr is correct, you have to explicitly initialize the class");
  }
  
  start = micros();
  rtcMem.begin();
  end = micros();
  Serial.println(String("The initilization had taken: ") + (end - start) + "us");

  // Get the data
  data = rtcMem.getData<byte>();
  // Modify some data
  data[0]++;
  Serial.println(String("Value to persist: ") + data[0]);
  
  //Save the data in the rtc memory
  start = micros();
  rtcMem.save();
  end = micros();
  Serial.println(String("save() method (save only on RTC memory) had taken: ") + (end - start) + "us");

  // Persist the data in rtc mem and in flash
  start = micros();
  rtcMem.persist();
  end = micros();
  Serial.println(String("persist() method (save on RTC memory + flash) had taken: ") + (end - start) + "us");

  delay(10000);
  ESP.restart();
}

void loop() { }
