#ifndef InterfaceEEPROM_h
#define InterfaceEEPROM_h

#include <Arduino.h>

/**
 * Abstract Class
 */
class InterfaceEEPROM
{
public:
    /**
         * Sets the EEPROM address to save the Object to
         * @param addr the address in the EEPROM
         */
    virtual void setEEPROMaddr(uint16_t addr) = 0;
    /**
         * Save in the EEPROM at the {@code addr}
         */
    virtual void writeToEEPROM() = 0;
    /**
         * Read from the EEPROM at the {@code addr}
         */
    virtual void readFromEEPROM() = 0;
    /**
         * Retrieve the amount of space occupied on the EEPROM in bytes
         * @return the bytes occupied
         */
    virtual uint16_t sizeOnEEPROM() = 0;

protected:
    // Address in EEPROM
    uint16_t addr;
};

#endif