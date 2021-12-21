#ifndef ADCCalibration_h
#define ADCCalibration_h

#include <Arduino.h>

#include "Storage/InterfaceEEPROM.h"

class ADCCalibration : InterfaceEEPROM
{
public:
    // Constructor
    ADCCalibration(){};

    void setup(uint8_t pin_adc_1, uint8_t pin_adc_2, uint8_t pin_pwr);
    void calibrate_adc(bool debug);
    bool is_calibrated();
    uint16_t *get_calibration_table(uint8_t n);

    uint16_t get_calibrated_reading(uint8_t pin, uint16_t raw_value);

    // InterfaceEEPROM
    void setEEPROMaddr(uint16_t addr);
    void writeToEEPROM();
    void readFromEEPROM();
    uint16_t sizeOnEEPROM();

private:
    bool calibrated;
    double conv_unit;
    uint8_t pin_pwr;
    uint8_t pin_adc_1, pin_adc_2;
    uint16_t calibration_table[2][256];
};

#endif