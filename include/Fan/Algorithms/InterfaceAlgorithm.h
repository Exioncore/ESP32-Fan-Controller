#ifndef InterfaceFC_h
#define InterfaceFC_h

#include <Arduino.h>
#include "Storage/InterfaceEEPROM.h"

/**
 * Abstract Class
 */
class InterfaceAlgorithm : public InterfaceEEPROM {
    public:
        enum FCtype {
            Curve = 0,
            PID
        };

        virtual void set_ambient_temperature(float temperature) = 0;
        virtual void set_ambient_reference(bool enable) = 0;

        virtual bool is_using_ambient_reference() = 0;
        virtual float get_duty_at(float temperature) = 0;

        virtual FCtype getType() = 0;
    protected:
        float ambient_temperature;
        bool use_ambient_temperature;
};

#endif