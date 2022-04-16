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
#ifndef RTCMEMORY_H
#define RTCMEMORY_H

#include <Arduino.h>
#include <type_traits>

class RTCMemory {
public:
  /**
   * Create RTCMemory. Set a valid filepath to enable the backup of RTC memory on flash memory.
   */
  RTCMemory(String path = "");

  /**
   * Initialize the buffer using data in RTC memory. If data are invalid, load data from
   * flash. If it fails again, the buffer is cleared. After this call you can safely use this
   * instance.
   *
   * Return true if at least one data source is valid, otherwise false.
   */
  bool begin();

  /**
   * Write data to RTC memory.
   *
   * Return true if the operation is completed successfully, otherwise false.
   */
  bool save();

  /**
   * Write data to flash memory.
   *
   * Return true if the operation is completed successfully, otherwise false.
   */
  bool backup();

  /**
   * Write data to both RTC and flash RTCMemory. It is equivalent to subsequent call to save() and
   * backup().
   *
   * Return true if both write operations are completed successfully, otherwise false.
   */
  bool persist();

  /**
   * Get a pointer to the user buffer, structured accordingly to the typename T.
   *
   * Return a valid pointer to the data if begin() was called, otherwise nullptr.
   */
  template<typename T> T *getData() {
    static_assert(sizeof(T) <= sizeof(RTCData::data), "Error: max size is 508 Byte");

    if (ready) { return reinterpret_cast<T *>(rtcData.data); }

    if (verbosity > 0) Serial.println("Call init before other calls!");
    return nullptr;
  };

private:
  /**
   * Max size of user memory.
   */
  const static unsigned int USER_RTC_MEMORY_SIZE = 508;

  /**
   * Total size of RTC memory (including CRC).
   */
  const static unsigned int TOTAL_RTC_MEMORY_SIZE = USER_RTC_MEMORY_SIZE + 4;

  struct RTCData {
    uint32_t crc32;
    // The user buffer
    byte data[USER_RTC_MEMORY_SIZE];
  };

  // The buffer
  RTCData rtcData;

  /**
   * Tell if this class is ready to be used i.e. if begin() was called.
   */
  bool ready;

  /**
   * Verbosity levels:
   * 0 - No output (Default)
   * 1 - Only errors
   * 2 - Debug
   */
  const static int verbosity = 0;

  String filePath;

  /**
   * Load data from flash.
   *
   * Return true if it reads valid data, otherwise false (return false also if filepath is not set).
   */
  bool readFromFlash();

  /**
   * Save data to flash.
   *
   * Return true on success, otherwise false (return false also if filepath is not set).
   */
  bool writeToFlash();

  /**
   * Calculate the CRC (32bit) of the given buffer.
   *
   * Return the CRC code.
   */
  uint32_t calculateCRC32(const uint8_t *data, size_t length) const;

  /**
   * Clear the buffer.
   */
  void clearBuffer();
};

#endif  // END RTCMEMORY_H
