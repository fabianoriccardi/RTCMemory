#ifndef RTC_MEMORY_H
#define RTC_MEMORY_H

#include <Arduino.h>

// The total RTC memory of ESP8266 is 512 bytes.
const unsigned int RTC_DATA_LENGTH = 508;

struct RtcData{
  uint32_t crc32;
  byte data[RTC_DATA_LENGTH];
};

class RtcMemory{
  public:
    RtcMemory(String path, int verbosity = 1);

    /**
     * Initialize the RAM memory. Firstly, it tries to get data from RTC Memory.
     * If not valid, try to load data from flash memory. If not valid, 
     * reset the memory.
     */
    bool begin();
    
    /**
     * Write data only on RTC memory.
     * Return true is the operation is completed successfully, otherwise false.
     */
    bool save();
    
    /**
     * Write on RTC memory and on flash. True if both write operations get
     * successfully completed, otherwise false.
     */
    bool persist();

    /**
     * Get a pointer the internal buffer.
     */
    byte* getRtcData();

  private:
    RtcData rtcData;
    bool ready;
    
    /**
     * 0 - No output
     * 1 - Only error
     * 2 - Verbose output
     */
    int verbosity;
    String filePath;
    const int dataLength = RTC_DATA_LENGTH;

    /**
     * Load data from flash in RAM. 
     * If the file is not found, an empty file is created and
     * the RAM memory is reset. 
     * True in case of successful reading, otherwise false.
     */
    bool readFromFlash();

    /**
     * Write data from RAM to the flash.
     * True on success, false otherwise.
     */
    bool writeToFlash();

    /**
     * Calculate the CRC (32bit) of input memory.
     */
    uint32_t calculateCRC32(const uint8_t *data, size_t length) const;

    /**
     * Reset the RAM memory.
     */
    void memoryReset();
};

#endif // END RTC_MEMORY_H
