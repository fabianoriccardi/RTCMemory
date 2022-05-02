/***********************************************************************
 * Benchmark and test the APIs offered by RTCMemory.
 * It shows the different speeds provided by RTC and flash memory.
 ***********************************************************************/
#include <RTCMemory.h>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("RTCMemory - Benchmark");

  // Stop the sketch until serial monitor is opened and user presses 's'
  Serial.println("Press 's' to start the benchmark");
  while (1) {
    delay(10);
    char c = Serial.read();
    if (c == 's') break;
  }

  // Remember to initialize the SPIFFS memory
  Serial.print("Filesystem initialization... ");
  if (LittleFS.begin()) {
    Serial.println("Done!");
  } else {
    Serial.println("Error");
  }

  unsigned long start, end;

  RTCMemory<byte> rtcMem("/etc/rtc_mem.bin");
  byte *data = rtcMem.getData();
  if (data == nullptr) {
    Serial.println("OK: here nullptr is expected, you have to initialize the instance explicitly");
  }

  start = micros();
  rtcMem.begin();
  end = micros();
  Serial.println(String("The initialization had taken: ") + (end - start) + "us");

  data = rtcMem.getData();
  Serial.println(String("Value read: ") + data[0]);

  // Modify data
  data[0]++;
  Serial.println(String("Value to store: ") + data[0]);

  // Save the data to RTC memory
  start = micros();
  rtcMem.save();
  end = micros();
  Serial.println(String("save() method (save to RTC memory) had taken: ") + (end - start) + "us");

  // Backup the data to flash memory
  start = micros();
  rtcMem.backup();
  end = micros();
  Serial.println(String("backup() method (save to flash memory) had taken: ") + (end - start) + "us");

  delay(10000);
  ESP.restart();
}

void loop() {}
