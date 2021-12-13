#ifndef PID_h
#define PID_h

#include <Arduino.h>

#include "Fan/Algorithms/InterfaceAlgorithm.h"

class PID : InterfaceAlgorithm {
    public:
        // Constructors
        PID();

        // Configuration
        //// Setters
        void set_P(float p);
        void set_I(float i);
        void set_D(float d);
        void set_min(uint8_t min);
        void set_max(uint8_t max);
        void set_target(int8_t target);
        //// Getters
        float get_P();
        float get_I();
        float get_D();
        uint8_t get_min();
        uint8_t get_max();
        int8_t get_target();

        // InterfaceAlgorithm Functions
        void set_ambient_temperature(float temperature);
        void set_ambient_reference(bool enable);

        bool is_using_ambient_reference();
        float get_duty_at(float temperature);

        FCtype getType() { return FCtype::PID; }

        // InterfaceEEPROM
        void setEEPROMaddr(uint16_t addr);
        void writeToEEPROM();
        void readFromEEPROM();
        uint16_t sizeOnEEPROM();
    private:
        int8_t target;
        float p, i, d;
        uint8_t min, max;
        // Internal variables
        long lastUpdate = 0;
        double max_integral = 0;
        double _integral = 0, _pre_error = 0;
};

#endif