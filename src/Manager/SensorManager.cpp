#include <EEPROM.h>

#include "Manager/SensorManager.h"
#include "Sensor/Thermistor.h"
#include "Sensor/SoftwareSensor.h"

// Allocating the pointer to the static instance
SensorManager *SensorManager::instance = 0;

//////////////////////
//   Constructors   //
//////////////////////
SensorManager::SensorManager() {}

SensorManager *SensorManager::getInstance()
{
    if (!instance)
    {
        instance = new SensorManager();
    }
    return instance;
}

void SensorManager::setup(
    uint8_t pwr,
    uint8_t ambient_thermistor, uint16_t ambient_series_resistor,
    uint8_t thermistor_1, uint16_t thermistor_1_series_resistor,
    uint8_t thermistor_2, uint16_t thermistor_2_series_resistor,
    uint8_t pin_adc_1, uint8_t pin_adc_2)
{
    adc_calibration.setup(pin_adc_1, pin_adc_2, pwr);
    this->sensors.reserve(3);
    this->sensors.push_back((InterfaceSensor *)new Thermistor(
        &adc_calibration, pwr, ambient_thermistor, 10000, 25, ambient_series_resistor, 3950, 100, 0.1f));
    this->sensors.push_back((InterfaceSensor *)new Thermistor(
        &adc_calibration, pwr, thermistor_1, 10000, 25, thermistor_1_series_resistor, 3950, 100, 0.1f));
    this->sensors.push_back((InterfaceSensor *)new Thermistor(
        &adc_calibration, pwr, thermistor_2, 10000, 25, thermistor_2_series_resistor, 3950, 100, 0.1f));
    this->sensors.push_back((InterfaceSensor *)new SoftwareSensor());
    this->sensors.push_back((InterfaceSensor *)new SoftwareSensor());
}

//////////////////////
//     Getters      //
//////////////////////
InterfaceSensor *SensorManager::get_sensor(SensorSource index)
{
    return this->sensors[index];
}

//////////////////////
//     General      //
//////////////////////
void SensorManager::update()
{
    for (uint8_t i = 0; i < this->sensors.size(); i++)
    {
        this->sensors[i]->update();
    }
}

void SensorManager::calibrate_adc(bool debug)
{
    adc_calibration.calibrate_adc(debug);
    adc_calibration.writeToEEPROM();
    EEPROM.commit();
}

/////////////////////
// InterfaceEEPROM //
/////////////////////
void SensorManager::setEEPROMaddr(uint16_t addr)
{
    this->addr = addr;
    uint16_t ofst = 0;
    adc_calibration.setEEPROMaddr(this->addr);
    ofst += adc_calibration.sizeOnEEPROM();
    for (uint8_t i = 0; i < this->sensors.size(); i++)
    {
        this->sensors[i]->setEEPROMaddr(addr + ofst);
        ofst += this->sensors[i]->sizeOnEEPROM();
    }
    this->eepromSize = ofst;
}
void SensorManager::writeToEEPROM()
{
    adc_calibration.writeToEEPROM();
    for (uint8_t i = 0; i < this->sensors.size(); i++)
    {
        this->sensors[i]->writeToEEPROM();
    }
}
void SensorManager::readFromEEPROM()
{
    adc_calibration.readFromEEPROM();
    for (uint8_t i = 0; i < this->sensors.size(); i++)
    {
        this->sensors[i]->readFromEEPROM();
    }
}
uint16_t SensorManager::sizeOnEEPROM()
{
    return adc_calibration.sizeOnEEPROM();
}