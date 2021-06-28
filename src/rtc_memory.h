/***************************************************************************
 *   Copyright (C) 2020-2021  Fabiano Riccardi                             *
 *                                                                         *
 *   This file is part of RTCMemory.                                       *
 *                                                                         *
 *   RTCMemory is free software; you can redistribute                      *
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   RTCMemory is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with RTCMemory; if not, see <http://www.gnu.org/licenses/>      *
 ***************************************************************************/
#ifndef RTC_MEMORY_H
#define RTC_MEMORY_H

#include <Arduino.h>
#include <type_traits>

/**
 * Max size of user data.
 *
 * Keep for compatibility with old releases.
 */
const static unsigned int RTC_DATA_LENGTH = 508;

class RtcMemory {
public:
  /**
   * Create RtcMemory instance.
   * Set a valid filepath to backup RTC memory on flash memory.
   */
  RtcMemory(String path = "");

  /**
   * Initialize the RAM memory. Firstly, it tries to get data from RTC memory.
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
   *
   * Return true if the operation is completed successfully, otherwise false.
   */
  bool save();

  /**
   * Write on RTC memory and on flash.
   *
   * Return true if both write operations are completed successfully, otherwise
   * false.
   */
  bool persist();

  /**
   * Get a pointer to the internal buffer. You should use getData() method.
   */
  byte *getRtcData() __attribute__((deprecated));

  /**
   * Get a pointer to the internal buffer, structured accordigly the
   * specialized template. Return nullptr if you didn't call begin().
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
  /**
   * Size of user memory.
   */
  const static unsigned int USER_RTC_MEMORY_SIZE = 508;

  /**
   * Size of RTC memory (including CRC).
   */
  const static unsigned int TOTAL_RTC_MEMORY_SIZE = USER_RTC_MEMORY_SIZE + 4;

  struct RtcData {
    uint32_t crc32;
    byte data[USER_RTC_MEMORY_SIZE];
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
  const static int verbosity = 0;

  String filePath;

  /**
   * Load data from flash to RAM. If the file is not found, an empty file is
   * created and the RAM memory is reset.
   *
   * Return true if read is completed successfully, otherwise false.
   */
  bool readFromFlash();

  /**
   * Write data from RAM to flash.
   *
   * Return true on success, otherwise false (even if no filepath was set).
   */
  bool writeToFlash();

  /**
   * Calculate the CRC (32bit) of input memory.
   *
   * Return the CRC code.
   */
  uint32_t calculateCRC32(const uint8_t *data, size_t length) const;

  /**
   * Write all zeros in the RAM memory.
   */
  void memoryReset();
};

#endif // END RTC_MEMORY_H
