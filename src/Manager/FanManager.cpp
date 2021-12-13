#include <EEPROM.h>

#include "Manager/FanManager.h"

// Allocating the pointer to the static instance
FanManager* FanManager::instance = NULL;

//////////////////////
//   Constructors   //
//////////////////////
FanManager::FanManager() {}

FanManager* FanManager::getInstance() {
    if (!instance) {
        instance = new FanManager();
    }
    return instance;
}

//////////////////////
//     Setters      //
//////////////////////
void FanManager::setup( 
    const uint8_t* fan_pwm_pin,
    const uint8_t* fan_rpm_pin,
    const uint8_t number_of_fans
) {
    fan.clear();
    fan.reserve(5);
    for (uint8_t i = 0; i < 5; i++) {
        fan.push_back(new Fan(fan_pwm_pin[i], i, fan_rpm_pin[i], i));
    }
}

void FanManager::update(vector<double> temperature) {
    for (uint8_t i = 0; i < fan.size(); i++) {
        fan[i]->update(temperature);
    }
}

void FanManager::set_profile(uint8_t n) {
    this->profile = n;
    read_profile();
}

//////////////////////
//     Getters      //
//////////////////////
Fan* FanManager::get_fan(uint8_t n) {
    return this->fan[n];
}

uint8_t FanManager::get_profile() {
    return this->profile;
}

/////////////////////
// InterfaceEEPROM //
/////////////////////
void FanManager::setEEPROMaddr(uint16_t addr) {
    this->addr = addr;
}
void FanManager::writeToEEPROM() {
    EEPROM.writeUChar(this->addr, profile);
    for (uint8_t i = 0; i < fan.size(); i++) {
        fan[i]->writeToEEPROM();
    }
}
void FanManager::readFromEEPROM() {
    this->profile = EEPROM.readUChar(this->addr);
    if (this->profile > 2) this->profile = 1;
    read_profile();
}
uint16_t FanManager::sizeOnEEPROM() {
    return 1 + (Fan::max_size_on_EEPROM() * 5) * 3;
}

void FanManager::read_profile() {
    for (uint8_t i = 0; i < fan.size(); i++) {
        fan[i]->setEEPROMaddr(
            this->addr + 1 + 
            (Fan::max_size_on_EEPROM() * 5) * this->profile + 
            Fan::max_size_on_EEPROM() * i
        );
        fan[i]->readFromEEPROM();
    }
}