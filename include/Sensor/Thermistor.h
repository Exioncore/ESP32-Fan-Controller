#ifndef Thermistor_h
#define Thermistor_h

#include <Arduino.h>

#include "Sensor/ADCCalibration.h"
#include "Sensor/InterfaceSensor.h"
#include "Util/RunningAverage.h"

class Thermistor : InterfaceSensor
{
public:
    // Empty Constructor
    Thermistor();
    // Constructor
    Thermistor(
        ADCCalibration *adc_calibration,
        uint8_t power_pin,
        uint8_t sense_pin,
        uint16_t thermistor_nominal_resistance,
        int16_t thermistor_nominal_temperature,
        uint16_t series_resistor,
        uint16_t b_coefficient,
        uint16_t number_of_samples,
        float rate_of_change);

    //InterfaceSensor
    void update();
    double get_temperature();
    InterfaceSensor::SensorType get_type();

    void calibrate_adc(bool debug);

    // InterfaceEEPROM
    void setEEPROMaddr(uint16_t addr);
    void writeToEEPROM();
    void readFromEEPROM();
    uint16_t sizeOnEEPROM();

private:
    // Calculate temperature from raw value
    double calculate_temp_from_raw(uint16_t raw);
    double round_measure(double original, double previous_rounding);

    RunningAverage *avg_algo;
    ADCCalibration *adc_calibration;
    // The power pin of the Sensor
    uint8_t power_pin;
    // The sense pin of the Sensor
    uint8_t sense_pin;
    // The processed Sensor reading
    double temperature;
    double internal_temperature;
    // Parameters
    uint16_t thermistor_nominal_resistance, thermistor_nominal_temperature;
    uint16_t series_resistor = 10000, b_coefficient = 3950;
    uint16_t number_of_samples = 20;
    float rounding = 0.1;
    float rate_of_change;
};

#endif