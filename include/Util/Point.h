#ifndef Point_h
#define Point_h

#include <Arduino.h>

class Point_i16 {
    public:
        Point_i16();
        Point_i16(int16_t x, int16_t y);
        void set(int16_t x, int16_t y);
        static Point_i16 get_delta(Point_i16 p1, Point_i16 p2);
        static uint8_t get_size();
        int16_t x, y;
};

class Point_i8 {
    public:
        Point_i8();
        Point_i8(int8_t x, int8_t y);
        void set(int8_t x, int8_t y);
        static Point_i16 get_delta(Point_i8 p1, Point_i8 p2);
        static uint8_t get_size();
        int8_t x, y;
};

#endif