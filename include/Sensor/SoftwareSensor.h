#ifndef SoftwareSensor_h
#define SoftwareSensor_h

#include <Arduino.h>
#include "Sensor/InterfaceSensor.h"
#include "Storage/InterfaceEEPROM.h"

class SoftwareSensor : InterfaceSensor
{
    public:
        // Empty Constructor
        SoftwareSensor();

        // Set Temperature
        void set_temperature(double temperature);
        
        //InterfaceSensor
        void update();
        double get_temperature();
        InterfaceSensor::SensorType get_type();

        // InterfaceEEPROM
        void setEEPROMaddr(uint16_t addr);
        void writeToEEPROM();
        void readFromEEPROM();
        uint16_t sizeOnEEPROM();
    private:
        // The processed Sensor reading
        double temperature;
};

#endif