#include "SerialHandler.h"

SerialHandler::SerialHandler() {}

SerialHandler::SerialHandler(HardwareSerial* serial, uint16_t buffer_size, char separator) {
    this->serial = serial;
    this->buffer_index = 0;
    this->buffer_size = buffer_size;
    this->buffer = new char[buffer_size];
    this->buffer_ready = 0;
    this->separator = separator;
}

int8_t SerialHandler::update() {
    if (buffer_ready != 0) return buffer_ready;

    int16_t c;
    while ((c = serial->read()) >= 0) {
        // Check if received char is a separator
        if (c == separator) {
            buffer[buffer_index] = '\0';
            buffer_ready = 1;
            break;
        } else {
            buffer[buffer_index] = c;
        }
        buffer_index++;
        // Check if buffer is full
        if (buffer_index == buffer_size) {
            buffer_ready = -1;
            break;
        }
    }

    return buffer_ready;
}

void SerialHandler::discardBuffer() {
    buffer_index = 0;
    buffer_ready = 0;
}