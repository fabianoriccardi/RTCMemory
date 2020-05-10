# RTCMemory

A library to make easier to benefit of RTC memory embedded in ESP8266, designed for the Arduino platform.  

## Motivation

[ESP8266 Arduino core](https://github.com/esp8266/Arduino) provides *byte-oriented* functions to read, write, and check the integrity data on RTC memory. Hence, accessing common data types, such as string, integer, or float, requires to deal with pointers and casting. RTCMemory hides this complexity, making you access directly to high-level data stored in RTC memory. Moreover, if you need to preserve this data from power loss, this library allows you to back up the entire content on non-volatile flash memory in a single line.

## Features

 - Make easier to access to RTC data
 - Backup data on flash memory
 - Support for SPIFFS and LittleFS file systems

## Usage

You should be aware of very few APIs to effectively use RTCMemory.  
First, define a data structure that will contains your data (max 508 bytes):

    typedef struct {
        int counter;
    } MyData;

Then instantiate the rtcMemory (global scope is fine):

    RtcMemory rtcMemory("/path/to/file");

the filepath is an optional parameter, if you don't need the persistent backup you can omit it! Then you will initialize RTC Memory through:

    bool result = rtmMemory.begin();

*result* is true if there is some valid user data in RTC memory (or if RTC memory is not valid and filepath was provided, in the flash), otherwise is false.
Get the buffer containing your data:

    MyData* myData = rtcMemory.getData<MyData>();

Modify your data as you prefer. Finally, save data on RTC:

    rtcMemory.save();

Remember that RTC memory is a low-power volatile memory, hence data on this memory are lost on power loss. To backup your data in flash, call:

    rtcMemory.persist();

This is a quick overview of the library, for more comprehensive documentation you may look at the header file.

### Selecting the file system

ESP8266 Arduino provides 2 file systems: the SPIFFS and LittleFS. In April 2020, SPIFFS was deprecated, but given its incompatibility with LittleFS (formatting is required), some projects may continue to require SPIFFS. By default, this library uses SPIFFS, but you are free to switch the LittleFS enabling (uncommenting) the proper #define. By default, in the first lines of rtc_memory.cpp, you can find:

    #define ESP_LOGGER_FLASH_FS_SPIFFS 
    //#define ESP_LOGGER_FLASH_FS_LITTLEFS
