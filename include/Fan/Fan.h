#ifndef Fan_h
#define Fan_h

#include <Arduino.h>
#include <vector>

#include "Fan/Algorithms/InterfaceAlgorithm.h"
#include "Fan/Header.h"
#include "Storage/InterfaceEEPROM.h"

using namespace std;

class Fan : InterfaceEEPROM
{
public:
    // Empty Constructor
    Fan();
    // Constructor
    Fan(
        uint8_t pwm_pin,
        uint8_t pwm_channel,
        uint8_t rpm_pin,
        uint8_t pcnt_channel);

    // Update Fan
    void update(vector<double> temperatures);
    // Set Fan Duty
    void set_duty(int8_t duty);
    // Set smooth value
    void set_smoothing_value(uint8_t value);
    // Set enabled sensors
    void set_enabled_sensors(uint8_t value);

    // Get RPM
    uint16_t get_rpm();
    // Get duty
    float get_duty();
    // Get temperature on the curve
    float get_temperature();
    // Get operating mode
    bool is_manual();
    // Get smooth value
    uint8_t get_smoothing_value();
    // Get Control Algorithm
    InterfaceAlgorithm *get_algorithm(uint8_t index);
    // Get enabled sensors
    uint8_t get_enabled_sensors();

    // InterfaceEEPROM
    void setEEPROMaddr(uint16_t addr);
    void writeToEEPROM();
    void readFromEEPROM();
    uint16_t sizeOnEEPROM();

    static uint16_t max_size_on_EEPROM();

private:
    Header header;
    vector<InterfaceAlgorithm *> algorithms;

    int8_t manual_duty = -1;
    uint8_t enabled_sensors = 0;
    float temperature;
    float smoothness;
};

#endif