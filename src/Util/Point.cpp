#include <Util/Point.h>

Point_i16::Point_i16(){}

Point_i16::Point_i16(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}

void Point_i16::set(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}

Point_i16 Point_i16::get_delta(Point_i16 p1, Point_i16 p2) {
    return Point_i16(p2.x - p1.x, p2.y - p1.y);
}

uint8_t Point_i16::get_size() {
    return 4;
}

Point_i8::Point_i8(){}

Point_i8::Point_i8(int8_t x, int8_t y) {
    this->x = x;
    this->y = y;
}

void Point_i8::set(int8_t x, int8_t y) {
    this->x = x;
    this->y = y;
}

Point_i16 Point_i8::get_delta(Point_i8 p1, Point_i8 p2) {
    return Point_i16(p2.x - p1.x, p2.y - p1.y);
}

uint8_t Point_i8::get_size() {
    return 2;
}