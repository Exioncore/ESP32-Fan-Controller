#include <EEPROM.h>

#include "Sensor/ADCCalibration.h"

void ADCCalibration::setup(
    uint8_t pin_adc_1,
    uint8_t pin_adc_2,
    uint8_t pin_pwr)
{
    this->calibrated = false;
    this->conv_unit = 4095.0 / 255.0;
    this->pin_adc_1 = pin_adc_1;
    this->pin_adc_2 = pin_adc_2;
    this->pin_pwr = pin_pwr;

    pinMode(pin_adc_1, INPUT);
    pinMode(pin_adc_2, INPUT);
    pinMode(pin_pwr, OUTPUT);
    adcAttachPin(pin_adc_1);
    adcAttachPin(pin_adc_2);
}

void ADCCalibration::calibrate_adc(bool debug)
{
    char txt[30];
    uint16_t val = 0;
    uint32_t adc_1, adc_2;
    if (debug)
        Serial.println("START");
    for (uint16_t i = 0; i < 256; i++)
    {
        dacWrite(pin_pwr, i);
        adc_1 = 0;
        adc_2 = 0;
        for (uint16_t u = 0; u < 10000; u++)
        {
            adc_1 += analogRead(pin_adc_1);
            adc_2 += analogRead(pin_adc_2);
        }
        int adc_1_val = (int)round(adc_1 / 10000.0f);
        int adc_2_val = (int)round(adc_2 / 10000.0f);
        if (debug)
        {
            int dac_val = (int)round(i * (4095.0 / 255.0));
            sprintf(txt, "%d, %d, %d", dac_val, adc_1_val, adc_2_val);
            Serial.println(txt);
        }
        calibration_table[0][i] = adc_1_val;
        calibration_table[1][i] = adc_1_val;
        val += 1;
    }
    if (debug)
        Serial.println("END");
    calibrated = true;
    dacWrite(pin_pwr, 0);
}

bool ADCCalibration::is_calibrated()
{
    return calibrated;
}

uint16_t *ADCCalibration::get_calibration_table(uint8_t n)
{
    return calibration_table[n];
}

uint16_t ADCCalibration::get_calibrated_reading(uint8_t pin, uint16_t raw_value)
{
    uint8_t adc;
    // Determine ADC
    if (pin >= 32 && pin <= 39)
    {
        adc = 0;
    }
    else if (
        pin == 0 || pin == 2 || pin == 4 ||
        (pin >= 12 && pin <= 15) ||
        (pin >= 25 && pin <= 27))
    {
        adc = 1;
    }
    else
    {
        return 0;
    }
    // Get calibrated reading
    for (uint16_t i = 0; i < 255; i++)
    {
        if (raw_value >= calibration_table[adc][i] && raw_value < calibration_table[adc][i + 1])
        {
            uint16_t bin_size = calibration_table[adc][i + 1] - calibration_table[adc][i];
            double delta = (raw_value - calibration_table[adc][i]) / (double)bin_size;
            uint16_t table_bin_size = round(this->conv_unit * (i + 1) - this->conv_unit * i);
            return round((this->conv_unit * i) + (delta * table_bin_size));
        }
    }
    return 4095;
}

//////////////////////
// InterfaceEEPROM  //
//////////////////////
void ADCCalibration::setEEPROMaddr(uint16_t addr)
{
    this->addr = addr;
}

void ADCCalibration::writeToEEPROM()
{
    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint16_t u = 0; u < 256; u++)
        {
            EEPROM.writeUShort(this->addr + (i * 256 * 2) + (u * 2), calibration_table[i][u]);
        }
    }
    EEPROM.writeBool(this->addr + (256 * 2) * 2, calibrated);
}

void ADCCalibration::readFromEEPROM()
{
    calibrated = EEPROM.readBool(this->addr + (256 * 2) * 2);
    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint16_t u = 0; u < 256; u++)
        {
            calibration_table[i][u] = EEPROM.readUShort(this->addr + (i * 256 * 2) + (u * 2));
        }
    }
}

uint16_t ADCCalibration::sizeOnEEPROM()
{
    return (256 * 2) * 2 + 1;
}