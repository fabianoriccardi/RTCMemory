#include <rtc_memory.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);

  // This cycle is to avoid that the code start without the human code. For test purpose
  while(1){
    delay(10);
    char c = Serial.read();
    if(c == 'r') break;
  }
  
  Serial.print("Filesystem initialization... ");
  if(SPIFFS.begin()){
    Serial.println("Done!");
  }else{
    Serial.println("Error");
  }
  
  int start,end;
  
  RtcMemory rtcMem("/etc/trial.bin", 2);
  byte* data = rtcMem.getRtcData();
  if(data==NULL){
    Serial.println("Error: In this case NULL is correct, you have to initialiaze explicitly the class");
  }
  start=micros();
  rtcMem.init();
  end=micros();
  Serial.println(String("The initilization had taken: ") + (end-start) + "micros");

  // Get the data
  data=rtcMem.getRtcData();
  // Modify some data
  data[0]++;
  Serial.println(String("Value to persist: ") + data[0]);
  
  //Save the data in the rtc memory
  start=micros();
  rtcMem.save();
  end=micros();
  Serial.println(String("The saving on rtc memory had taken: ") + (end-start) + "micros");

  // Persist the data in rtc mem and in flash
  start=micros();
  rtcMem.persist();
  end=micros();
  Serial.println(String("The persist operation (rtc mem + flash) had taken: ") + (end-start) + "micros");

  delay(10000);
  ESP.restart();
}

void loop() {

}
