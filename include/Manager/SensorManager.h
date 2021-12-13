#ifndef SensorManager_h
#define SensorManager_h

#include <Arduino.h>
#include <vector>

#include "Sensor/ADCCalibration.h"
#include "Sensor/InterfaceSensor.h"
#include "Storage/InterfaceEEPROM.h"

using namespace std;

/**
 * Singleton Class
 * This class is used to manage all the Sensor's
 */

class SensorManager : InterfaceEEPROM {
    public:
        enum SensorSource {
            Ambient = 0,
            T1,
            T2,
            CPU,
            GPU,
        };
        // Retrieve the instance
        static SensorManager* getInstance();
        // Setup the instance
        void setup(
            uint8_t pwr,
            uint8_t ambient_thermistor, uint16_t ambient_series_resistor,
            uint8_t thermistor_1, uint16_t thermistor_1_series_resistor,
            uint8_t thermistor_2, uint16_t thermistor_2_series_resistor,
            uint8_t pin_adc_1,
            uint8_t pin_adc_2
        );
        // Update Sensor's reading
        void update();
        // Retrieve a Sensor
        InterfaceSensor* get_sensor(SensorSource index);
        // Execute ADC calibrations
        void calibrate_adc(bool debug);

        // InterfaceEEPROM
        void setEEPROMaddr(uint16_t addr);
        void writeToEEPROM();
        void readFromEEPROM();
        uint16_t sizeOnEEPROM();
    private:
        // Store the instance
        static SensorManager* instance;
        // Constructor
        SensorManager();

        ADCCalibration adc_calibration;

        // Sensor's
        vector<InterfaceSensor*> sensors;

        // EEPROM size requirement
        uint16_t eepromSize;
};

#endif