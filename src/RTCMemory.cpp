/***************************************************************************
 *   Copyright (C) 2020-2022  Fabiano Riccardi                             *
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
#include "RTCMemory.h"
#include <FS.h>

// Select (uncomment) ONLY ONE file system
//(NOTE: SPIFFS is deprecated but working)
#define ESP_LOGGER_FLASH_FS_SPIFFS
//#define ESP_LOGGER_FLASH_FS_LITTLEFS

#ifdef ESP_LOGGER_FLASH_FS_SPIFFS
#define ESP_LOGGER_FLASH_FS SPIFFS
#elif defined(ESP_LOGGER_FLASH_FS_LITTLEFS)
#define ESP_LOGGER_FLASH_FS LittleFS
#include <LittleFS.h>
#endif

RTCMemory::RTCMemory(String path) : ready(false), filePath(path) {}

bool RTCMemory::begin() {
  if (ready) {
    if (verbosity > 1) Serial.println("Instance already initialized");
    return true;
  }

  if (verbosity > 1) Serial.print("Loading RTC memory... ");

  if (!ESP.rtcUserMemoryRead(0, (uint32_t *)&rtcData, TOTAL_RTC_MEMORY_SIZE)) {
    if (verbosity > 0) Serial.println("Read RTC memory failure");
    return false;
  }

  uint32_t crcOfData = calculateCRC32((uint8_t *)&rtcData.data, USER_RTC_MEMORY_SIZE);
  if (verbosity > 1) {
    Serial.print("Calculated CRC: ");
    Serial.println(crcOfData, HEX);
    Serial.print("CRC read from RTC memory: ");
    Serial.println(rtcData.crc32, HEX);
  }

  ready = true;

  if (crcOfData != rtcData.crc32) {
    if (verbosity > 0)
      Serial.print("CRC in RTC memory doesn't match calculated CRC. Data are "
                   "invalid! Trying to restore backup from flash memory..");

    if (readFromFlash()) {
      if (verbosity > 1) Serial.println("Loading backup from flash ok");
    } else {
      if (verbosity > 0) Serial.println("Loading backup from flash FAILED, data are resetted");
      clearBuffer();
      writeToFlash();
      return false;
    }
  } else {
    if (verbosity > 1) Serial.println("CRC is correct");
  }

  if (verbosity > 1) Serial.println("Done!");
  return true;
}

bool RTCMemory::save() {

  if (ready) {
    uint32_t crcOfData = calculateCRC32((uint8_t *)&rtcData.data, USER_RTC_MEMORY_SIZE);
    rtcData.crc32 = crcOfData;

    if (ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtcData, TOTAL_RTC_MEMORY_SIZE)) {
      if (verbosity > 1) Serial.println("Write to RTC memory done");
      return true;
    } else {
      if (verbosity > 0) Serial.println("Write to RTC memory failed");
      return false;
    }
  } else {
    if (verbosity > 0) Serial.println("Call begin() before using this method");
    return false;
  }
}

bool RTCMemory::backup() {
  if (ready) {
    return writeToFlash();
  } else {
    if (verbosity > 0) Serial.println("Call begin() before using this method");
  }
  return false;
}

bool RTCMemory::persist() {
  if (ready) {
    bool res = save();
    if (res) { return writeToFlash(); }
  } else {
    if (verbosity > 0) Serial.println("Call begin() before using this method");
  }
  return false;
}

bool RTCMemory::readFromFlash() {
  if (verbosity > 1) Serial.print(String("Reading from ") + filePath + "... ");

  if (filePath.length() == 0) {
    if (verbosity > 1) Serial.println("No filepath was set");
    return false;
  }

  if (!ESP_LOGGER_FLASH_FS.exists(filePath)) {
    if (verbosity > 0) Serial.println("File not existing");
    return false;
  }

  File f = ESP_LOGGER_FLASH_FS.open(filePath, "r");
  if (f) {
    int byteRead = f.read((uint8_t *)&rtcData, TOTAL_RTC_MEMORY_SIZE);
    if (verbosity > 1) Serial.println(String("Bytes read:") + byteRead);
    f.close();
    if (byteRead != TOTAL_RTC_MEMORY_SIZE) { return false; }
    uint32_t crc = calculateCRC32((uint8_t *)&rtcData.data, USER_RTC_MEMORY_SIZE);
    if (crc != rtcData.crc32) { return false; }
  } else {
    if (verbosity > 0) Serial.println(String("Error opening file: ") + filePath);
    return false;
  }

  return true;
}

bool RTCMemory::writeToFlash() {
  if (verbosity > 1) Serial.print(String("Writing to ") + filePath + "... ");

  if (filePath.length() == 0) {
    if (verbosity > 1) Serial.print("No filepath was set");
    return false;
  }

  File f = ESP_LOGGER_FLASH_FS.open(filePath, "w");
  if (f) {
    int n = f.write((uint8_t *)&rtcData, TOTAL_RTC_MEMORY_SIZE);
    f.close();
    if (n == TOTAL_RTC_MEMORY_SIZE) {
      if (verbosity > 1) Serial.print("Done!");
      return true;
    } else {
      if (verbosity > 0) Serial.println("Cannot write the requested amount of bytes on flash");
    }
  } else {
    if (verbosity > 0) Serial.println("Error writing file. Is the filesystem initialized?");
    return false;
  }
  return false;
}

uint32_t RTCMemory::calculateCRC32(const uint8_t *data, size_t length) const {
  uint32_t crc = 0xffffffff;
  while (length--) {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if (c & i) { bit = !bit; }
      crc <<= 1;
      if (bit) { crc ^= 0x04c11db7; }
    }
  }
  return crc;
}

void RTCMemory::clearBuffer() {
  memset((void *)&rtcData, 0, TOTAL_RTC_MEMORY_SIZE);
  uint32_t result = calculateCRC32((uint8_t *)rtcData.data, USER_RTC_MEMORY_SIZE);
  rtcData.crc32 = result;
}
