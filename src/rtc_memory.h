/***************************************************************************
 *   Copyright (C) 2020-2021  Fabiano Riccardi                             *
 *                                                                         *
 *   This file is part of RTC Memory.                                      *
 *                                                                         *
 *   RTC Memory is free software; you can redistribute                     *
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   RTC Memory is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/
#ifndef RTC_MEMORY_H
#define RTC_MEMORY_H

#include <Arduino.h>
#include <type_traits>

// The total RTC memory of ESP8266 is 512 bytes.
const unsigned int RTC_DATA_LENGTH = 508;

class RtcMemory {
public:
  RtcMemory(String path = "");

  /**
   * Initialize the RAM memory. Firstly, it tries to get data from RTC Memory.
   * If not valid, try to load data from flash memory. If not valid,
   * reset the memory.
   *
   * Return true if previous user data are found, otherwise false (i.e. the
   * memory is reset to all zeros, the user the init this memory depending on
   * its need).
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
   * Get a pointer the internal buffer. You should consider getData() method.
   */
  byte *getRtcData() __attribute__((deprecated));

  /**
   * Get a pointer the internal buffer, structured accordigly the
   * speciliazed template. Return nullptr if you didn't initialized
   * this object.
   */
  template <typename T> T *getData() {
    static_assert(sizeof(T) <= sizeof(RtcData::data),
                  "Error: max size is 508 Byte");

    if (ready) {
      return reinterpret_cast<T *>(rtcData.data);
    }
    if (verbosity > 0)
      Serial.println("Call init before other calls!");
    return nullptr;
  };

private:
  struct RtcData {
    uint32_t crc32;
    byte data[RTC_DATA_LENGTH];
  };

  RtcData rtcData;

  /*
   * Say if begin() was called, i.e. the RTC memory is ready to be used.
   */
  bool ready;

  /**
   * For development:
   * 0 - No output (Default)
   * 1 - Only error
   * 2 - Verbose output
   */
  const int verbosity = 0;
  String filePath;
  const int dataLength = RTC_DATA_LENGTH;

  /**
   * Load data from flash in RAM.
   * If the file is not found, an empty file is created and
   * the RAM memory is reset.
   * True in case of successful reading, otherwise false (even if no filepath
   * was set).
   */
  bool readFromFlash();

  /**
   * Write data from RAM to the flash.
   * True on success, otherwise false (even if no filepath was set).
   */
  bool writeToFlash();

  /**
   * Calculate the CRC (32bit) of input memory.
   */
  uint32_t calculateCRC32(const uint8_t *data, size_t length) const;

  /**
   * Write all zeros in the RAM memory.
   */
  void memoryReset();
};

#endif // END RTC_MEMORY_H
