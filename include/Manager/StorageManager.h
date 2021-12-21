#ifndef StorageManager_h
#define StorageManager_h

#include <Arduino.h>
#include <EEPROM.h>

/**
 * Singleton Class
 * This class is used to manage the EEPROM for Storage
 */

class StorageManager
{
public:
    static StorageManager *getInstance();
    // Setup Storage
    void initializeEEPROM();
    // Allocate space in the EEPROM
    void addSize(uint16_t size);
    // Retrieve last EEPROM addres
    uint16_t getLastAddr();
    // Commit Change
    void saveChanges();

private:
    // Store the instance
    static StorageManager *instance;
    // Constructor
    StorageManager();

    uint16_t lastAddr;
};

#endif