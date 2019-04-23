#ifndef RTC_MEMORY_H
#define RTC_MEMORY_H

#include <Arduino.h>

const unsigned int RTC_DATA_LENGTH = 508;

struct RtcData{
  uint32_t crc32;
  byte data[RTC_DATA_LENGTH];
};

class RtcMemory{
  public:
    RtcMemory(String path, int verbosity = 1);

    /**
     * Call this function before every other methods.
     */
    void begin();
    
    /**
     * Write on RTC memory
     */
    bool save();
    
    /**
     * Write on RTC memory and than on flash to allow a secondary backup. Still not fully implemented
     */
    bool persist();

    /**
     * Get the raw data. You can modify it without restriction
     */
    byte* getRtcData(){
      if(ready){
        return rtcData.data;
      }
      Serial.println("Call init before other calls!");
      return nullptr;
    }

  private:
    RtcData rtcData;
    bool ready;
    int verbosity;
    String filePath;
    const int dataLength = RTC_DATA_LENGTH;

    /**
     * Load data from flash. return true in case of successfull reading.
     */
    bool readFromFlash();

    /**
     * Very basic function to write from RAM to the flash.
     */
    bool writeToFlash();

    /**
     * Calculate the CRC (32bit)
     */
    uint32_t calculateCRC32(const uint8_t *data, size_t length);

    /**
     * Reset the RAM memory, write the proper crc32 value
     */
    void memoryReset();
    
};

#endif // END RTC_MEMORY_H
