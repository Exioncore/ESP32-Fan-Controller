#ifndef Curve_h
#define Curve_h

#include <Arduino.h>
#include <vector>

#include "Fan/Algorithms/InterfaceAlgorithm.h"
#include "Util/Point.h"

using namespace std;

class Curve : InterfaceAlgorithm
{
public:
    // Constructors
    Curve();
    Curve(uint8_t numberOfPoints);
    Curve(vector<Point_i8> points);

    // Configuration
    //// Setters
    void set_point(Point_i8 point, uint8_t index);
    //// Getters
    vector<Point_i8> get_points();
    uint8_t get_number_of_points();

    // InterfaceAlgorithm Functions
    void set_ambient_temperature(float temperature);
    void set_ambient_reference(bool enable);

    bool is_using_ambient_reference();
    float get_duty_at(float temperature);

    FCtype getType() { return FCtype::Curve; }

    // InterfaceEEPROM
    void setEEPROMaddr(uint16_t addr);
    void writeToEEPROM();
    void readFromEEPROM();
    uint16_t sizeOnEEPROM();

private:
    // Point's of the Curve
    vector<Point_i8> points;
};

#endif