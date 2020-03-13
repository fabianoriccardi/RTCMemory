# RTCMemory for ESP8266
A library to makes easier to benefit of RTC memory embedded in ESP8266, designed for Arduino platform.  
## Motivation
[ESP8266 Arduino core](https://github.com/esp8266/Arduino) provides *byte-oriented* functions to read, write and check integrity of RTC memory. Hence accessing to string, integer or float may require to deal with pointers and tricky casting. RTCMemory hides this complexity, making you to access directly to high-level data stored in RTC memory. Moreover, if you really need to preserve this data, you may want to protect them from power loss. For this reason, this library allows you to backup the content of RTC memory on non-volatile flash memory.

## Features

 - Make easier to access to RTC data
 - Backup data on flash memory

## Usage

You should be aware of very few APIs to take advantage from RTCMemory library.  
Firstly, define your data structure (max 512 bytes):

    typedef struct {
        int counter;
    } MyData;

Then declare the rtcMemory (at global scope will be fine):

    RtcMemory rtcMemory("/yourfile");

the filepath is an optional parameter, if you don't need the persistent backup you can omit it! Then you will initialize RTC Memory through:

    bool result = rtmMemory.begin();

*result* tells you if the RTC memory (or the flash, if filepath was provided) has data or is fresh.
Get the data buffer from the RTC memory:

    MyData* myData = rtcMemory.getData<MyData>();

Modify your data as you prefer. Finally save data on RTC:

    rtcMemory.save();

Remember that RTC memory is a low-power volatile memory, hence this memory would be lost on power loss. To persist your data also flash, call:

    rtcMemory.persist();

This is quick overview of the library, for a more comprehensive documentation you would look at the header file.
