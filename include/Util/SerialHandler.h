#ifndef SerialHandler_h
#define SerialHandler_h

#include <Arduino.h>
#include <HardwareSerial.h>

class SerialHandler {
    public:
        // Constructor
        SerialHandler();
        SerialHandler(HardwareSerial* serial, uint16_t buffer_size, char separator);

        int8_t update();
        void discardBuffer();

        char* buffer = NULL;
    private:
        HardwareSerial* serial;
        uint16_t buffer_size;
        uint16_t buffer_index;
        int8_t buffer_ready;

        char separator;
};

#endif