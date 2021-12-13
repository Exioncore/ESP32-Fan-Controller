#include "Manager/StorageManager.h"


// Allocating the pointer to the static instance
StorageManager* StorageManager::instance = 0;

/**
 * Empty Constructor due to Singleton Object
 */
StorageManager::StorageManager() {
    this->lastAddr = 0;
}

/**
 * This method either retrieves the existing instance or creates a new one if it
 * does not exist.
 * @return StorageManager instance
 */
StorageManager* StorageManager::getInstance() {
    if (!instance) {
        instance = new StorageManager();
    }
    return instance;
}

/**
 * Initialize EEPROM up until {@code lastAddr}
 */
void StorageManager::initializeEEPROM() {
    if (!EEPROM.begin(lastAddr)) {
        Serial.println("Error Initializing EEPROM");
    }
}
/**
 * Allocate additional space on the EEPROM
 * (CAN ONLY BE CALLED BEFORE initializeEEPROM())
 * @param size the additional space amount
 */
void StorageManager::addSize(uint16_t size) {
    this->lastAddr += size;
}
/**
 * Retrieve the last allocated address in the EEPROM
 * @return the address
 */
uint16_t StorageManager::getLastAddr() {
    return this->lastAddr;
}
/**
 * Commit changes to the EEPROM
 */
void StorageManager::saveChanges() {
    EEPROM.commit();
}