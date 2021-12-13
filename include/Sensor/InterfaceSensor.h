#ifndef InterfaceSensor_h
#define InterfaceSensor_h

#include <Arduino.h>
#include "Storage/InterfaceEEPROM.h"

/**
 * Abstract Class
 */
class InterfaceSensor : public InterfaceEEPROM {
    public:
        enum SensorType {
            MOBO_PWM = 0,
            DHT,
            THERMISTOR,
            SOFTWARE
        };
        /**
         * Update sensor reading
         */
        virtual void update() = 0;
        /**
         * Retrieve the temperature of the sensor
         * @return the temperature
         */
        virtual double get_temperature() = 0;
        /**
         * Retrieve the controller type
         * @return the type
         */
        virtual SensorType get_type() = 0;
};

#endif