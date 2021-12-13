#ifndef FanManager_h
#define FanManager_h

#include <Arduino.h>
#include <vector>

#include "Fan/Fan.h"
#include "Storage/InterfaceEEPROM.h"

using namespace std;

class FanManager : InterfaceEEPROM {
    public:
        // Retrieve the instance
        static FanManager* getInstance();
        // Setup the instance
        void setup(
            const uint8_t* fan_pwm_pin,
            const uint8_t* fan_rpm_pin,
            const uint8_t number_of_fans
        );
        void update(vector<double> temperature);
        void set_profile(uint8_t n);

        Fan* get_fan(uint8_t n);
        uint8_t get_profile();
        // InterfaceEEPROM
        void setEEPROMaddr(uint16_t addr);
        void writeToEEPROM();
        void readFromEEPROM();
        uint16_t sizeOnEEPROM();
    private:
        // Store the instance
        static FanManager* instance;
        // Constructor
        FanManager();

        vector<Fan*> fan;
        uint8_t profile;

        // EEPROM
        void read_profile();
};

#endif