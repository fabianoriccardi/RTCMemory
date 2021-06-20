# RTCMemory

[![arduino-library-badge](https://www.ardu-badge.com/badge/RTCMemory.svg)](https://www.ardu-badge.com/badge/RTCMemory.svg) ![Compile Library Examples](https://github.com/fabiuz7/rtc-memory-esp8266/actions/workflows/CompileLibraryExamples.yml/badge.svg)

RTCMemory is a library to efficiently manage the different levels of data persistence and speed offered by RAM, RTC memory, and flash memory on ESP8266.

If you are wondering why you should consider an RTC memory or, in general, any low-power memory, you may watch this [video](https://www.youtube.com/watch?v=r-hEOL007nw).

## Motivation

[ESP8266 Arduino core](https://github.com/esp8266/Arduino) provides *byte-oriented* functions to read, write, and check the integrity data on RTC memory. Hence, accessing data types such as string, integer, or float requires dealing with pointers and casting, leading to hard-to-find errors. So I created a snippet to make this process easier, more intuitive, and error-proof. Finally, I have come up with this library and some additional nice-to-have features such as backup on flash memory.

## Features

- Make trivial the management of RTC memory
- Backup data on flash memory
- Save data on different memories only when needed
- Compatibility with SPIFFS and LittleFS file systems

## Installation

You can find RTCMemory on Arduino and PlatformIO library registries. You can install it through your favorite IDEs, or you can use the respective command-line tools running:

    arduino-cli lib install RTCMemory

or:

    pio lib install "fabiuz7/RTCMemory"

## Usage

You should be aware of very few APIs to effectively use RTCMemory.  
First, define a data structure that will contains your data (max 508 bytes):

    typedef struct {
        int counter;
    } MyData;

Then instantiate `rtcMemory`:

    RtcMemory rtcMemory("/path/to/file");

the filepath is an optional parameter, if you don't need the backup on flash memory you can omit it. Then, initialize RTC memory through:

    bool result = rtcMemory.begin();

`result` is true if there are valid data in RTC memory or, if RTC memory is not valid and a filepath was provided, in flash memory, otherwise is false.
Get the memory area containing your data:

    MyData* myData = rtcMemory.getData<MyData>();

Modify data as you need. Finally, save data on RTC:

    rtcMemory.save();

Remember that RTC memory is a low-power volatile memory, hence data on this memory are lost on power loss. To backup your data in flash, call:

    rtcMemory.persist();

This is a quick overview of the library, for more comprehensive documentation you may look at the header file.

### Selecting the file system

ESP8266 Arduino provides 2 file systems: the SPIFFS and LittleFS. In April 2020, SPIFFS was deprecated, but given its incompatibility with LittleFS (formatting is required), some projects may continue to require SPIFFS. By default, this library uses SPIFFS, but you can switch to LittleFS (un)commenting the proper #defines at the beginning of `rtc_memory.cpp`.
