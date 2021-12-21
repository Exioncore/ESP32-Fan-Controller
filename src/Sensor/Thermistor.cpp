#include <EEPROM.h>

#include "Sensor/Thermistor.h"

//////////////////////
//   Constructors   //
//////////////////////
Thermistor::Thermistor() {}

Thermistor::Thermistor(
    ADCCalibration *adc_calibration,
    uint8_t power_pin,
    uint8_t sense_pin,
    uint16_t thermistor_nominal_resistance,
    int16_t thermistor_nominal_temperature,
    uint16_t series_resistor,
    uint16_t b_coefficient,
    uint16_t number_of_samples,
    float rate_of_change)
{
    this->adc_calibration = adc_calibration;
    this->power_pin = power_pin;
    this->sense_pin = sense_pin;
    this->thermistor_nominal_resistance = thermistor_nominal_resistance;
    this->thermistor_nominal_temperature = thermistor_nominal_temperature;
    this->series_resistor = series_resistor;
    this->b_coefficient = b_coefficient;
    this->number_of_samples = number_of_samples;
    this->rate_of_change = rate_of_change;
    // Initialize Pins
    pinMode(power_pin, OUTPUT);
    pinMode(sense_pin, INPUT);
    digitalWrite(power_pin, 0);
    adcAttachPin(sense_pin);
    // Read initial Temperature
    avg_algo = new RunningAverage(this->number_of_samples, 0);
    digitalWrite(this->power_pin, 1);
    for (uint16_t i = 0; i < this->number_of_samples; i++)
    {
        avg_algo->update(analogRead(this->sense_pin));
    }
    digitalWrite(this->power_pin, 0);
    this->internal_temperature = calculate_temp_from_raw(avg_algo->get_average());
    this->internal_temperature = this->temperature = (int)round(internal_temperature * (int)(1.0 / rounding)) / (1.0 / rounding);
    this->temperature = round_measure(this->internal_temperature, this->temperature);
}

//////////////////////
//     Getters      //
//////////////////////
double Thermistor::get_temperature()
{
    return this->temperature;
}

InterfaceSensor::SensorType Thermistor::get_type()
{
    return SensorType::THERMISTOR;
}

//////////////////////
//     General      //
//////////////////////
void Thermistor::update()
{
    digitalWrite(power_pin, 1);
    for (uint16_t i = 0; i < this->number_of_samples; i++)
    {
        avg_algo->update(analogRead(this->sense_pin));
    }
    digitalWrite(power_pin, 0);
    double t = calculate_temp_from_raw(this->adc_calibration->get_calibrated_reading(sense_pin, avg_algo->get_average()));
    this->internal_temperature = this->internal_temperature * (1.0 - rate_of_change) + t * rate_of_change;
    // Rounding
    this->temperature = round_measure(this->internal_temperature, this->temperature);
}

double Thermistor::calculate_temp_from_raw(uint16_t raw)
{
    raw = raw == 0 ? 1 : raw;
    double resistance;
    resistance = (4095.0 / raw) - 1;
    resistance = series_resistor / resistance;
    double steinhart;
    steinhart = resistance / thermistor_nominal_resistance;       // (R/Ro)
    steinhart = log(steinhart);                                   // ln(R/Ro)
    steinhart /= b_coefficient;                                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (thermistor_nominal_temperature + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                                  // Invert
    steinhart -= 273.15;                                          // convert to C
    return steinhart;
}

double Thermistor::round_measure(double original, double previous_rounding)
{
    if (abs(original - previous_rounding) >= this->rounding)
    {
        uint16_t factor = round(1.0 / rounding);
        double t = (int)round(original * factor);
        t /= (double)factor;
        return t;
    }
    return previous_rounding;
}

/////////////////////
// InterfaceEEPROM // NOT NECESSARY SO THIS IS A DUMMY
/////////////////////
void Thermistor::setEEPROMaddr(uint16_t addr) { this->addr = addr; }
void Thermistor::writeToEEPROM() { return; }
void Thermistor::readFromEEPROM() { return; }
uint16_t Thermistor::sizeOnEEPROM() { return 0; }
