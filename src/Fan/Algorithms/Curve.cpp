#include <EEPROM.h>
#include "Fan/Algorithms/Curve.h"

//////////////////////
//   Constructors   //
//////////////////////
Curve::Curve(){}

Curve::Curve(uint8_t numberOfPoints) {
    this->points.reserve(numberOfPoints);
    for (uint8_t i = 0; i < numberOfPoints; i++) this->points.push_back(Point_i8(0, 0));
}

Curve::Curve(vector<Point_i8> points) {
    this->points.swap(points);
}

//////////////////////
//  Configuratiion  //
//////////////////////
void Curve::set_point(Point_i8 point, uint8_t index) {
    this->points[index] = point;
}

vector<Point_i8> Curve::get_points() {
    return this->points;
}

uint8_t Curve::get_number_of_points() {
    return this->points.size();
}

//////////////////////////////////
// InterfaceAlgorithm Functions //
//////////////////////////////////
void Curve::set_ambient_temperature(float temperature) {
    this->ambient_temperature = temperature;
}

void Curve::set_ambient_reference(bool enable) {
    this->use_ambient_temperature = enable;
}

bool Curve::is_using_ambient_reference() {
    return this->use_ambient_temperature;
}

float Curve::get_duty_at(float temperature) {
    if (this->use_ambient_temperature) {
        temperature -= this->ambient_temperature;
    }
    // Check if x is lower than the 1st Point in the Curve
    if (temperature <= this->points[0].x) {
        // Return y of 1st Point in the Curve
        return this->points[0].y;
    } else {
        // Iterate through each Point in the Curve
        for (uint8_t i = 0; i < this->points.size() - 1; i++) {
            // Check if x is between 2 Point's
            if (this->points[i].x <= temperature && temperature < this->points[i + 1].x) {
                // Calculate the correct duty
                if (this->points[i].x != this->points[i + 1].x) {
                    Point_i16 delta = Point_i8::get_delta(this->points[i], this->points[i + 1]);
                    return this->points[i].y + (delta.y / delta.x) * (temperature - this->points[i].x);
                } else {
                    return this->points[i].y;
                }
            }
        }
    }
    // Return y of last Point in the Curve
    return this->points[this->points.size() - 1].y;
}

//////////////////////
// InterfaceEEPROM  //
//////////////////////
void Curve::setEEPROMaddr(uint16_t addr) {
    this->addr = addr;
}

void Curve::writeToEEPROM() {
    EEPROM.writeBool(this->addr, this->use_ambient_temperature);
    for (uint8_t i = 0; i < this->points.size(); i++) {
        EEPROM.writeChar(this->addr + 1 + i * 2, this->points[i].x);
        EEPROM.writeChar(this->addr + 1 + i * 2 + 1, this->points[i].y);
    }
}

void Curve::readFromEEPROM() {
    this->use_ambient_temperature = EEPROM.readBool(this->addr);
    this->points.clear();
    for (uint8_t i = 0; i < this->points.capacity(); i++) {
        int8_t x =  EEPROM.readChar(this->addr + 1 + i * 2);
        int8_t y =  EEPROM.readChar(this->addr + 1 + i * 2 + 1);
        // Clamp y 0 - 100%
        x = x > 100 ? 100 : (x < 0 ? 0 : x);
        y = y > 100 ? 100 : (y < 0 ? 0 : y);
        this->points.push_back(Point_i8(x, y));
    }
}

uint16_t Curve::sizeOnEEPROM() {
    return this->points.size() * Point_i8::get_size() + 1;
}