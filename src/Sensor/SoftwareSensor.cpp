#include <EEPROM.h>

#include "Sensor/SoftwareSensor.h"

/**
 * Empty Constructor
 */
SoftwareSensor::SoftwareSensor() {
    this->temperature = 20;
}

void SoftwareSensor::set_temperature(double temperature) {
    this->temperature = temperature;
}
/////////////////////
// InterfaceSensor //
/////////////////////
void SoftwareSensor::update() {}

double SoftwareSensor::get_temperature() {
    return this->temperature;
}

InterfaceSensor::SensorType SoftwareSensor::get_type() {
    return SensorType::SOFTWARE;
}

/////////////////////
// InterfaceEEPROM // NOT NECESSARY SO THIS IS A DUMMY
/////////////////////
void SoftwareSensor::setEEPROMaddr(uint16_t addr) { this->addr = addr; }
void SoftwareSensor::writeToEEPROM() { return; }
void SoftwareSensor::readFromEEPROM() { return; }
uint16_t SoftwareSensor::sizeOnEEPROM() { return 0; }