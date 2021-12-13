#include <EEPROM.h>
#include "Fan/Fan.h"
#include "Fan/Algorithms/Curve.h"
#include "Fan/Algorithms/PID.h"

//////////////////////
//   Constructors   //
//////////////////////
Fan::Fan(){}

Fan::Fan(uint8_t pwm_pin, uint8_t pwm_channel, uint8_t rpm_pin, uint8_t pcnt_channel) {
    this->header = Header(pwm_pin, pwm_channel, rpm_pin, pcnt_channel);
}

//////////////////////
//     Setters      //
//////////////////////
void Fan::update(vector<double> temperatures) {
    this->header.rpm_update();
    float highest_duty = 0, t = 0;
    uint8_t n = 1;
    if (manual_duty == -1) {
        for (uint8_t i = 0; i < temperatures.size() - 1; i++) {
            if ((enabled_sensors & n) > 0) {
                algorithms.at(i)->set_ambient_temperature(temperatures.at(0));
                float d = algorithms.at(i)->get_duty_at(temperatures.at(i + 1));
                if (d > highest_duty) {
                    highest_duty = d;
                    t = temperatures.at(i + 1);
                }
            }
            n <<= 1;
        }
        this->temperature = t;
        this->header.set_duty(highest_duty);
    } else {
        this->header.set_duty(manual_duty);
    }
}

void Fan::set_duty(int8_t duty) {
    if (duty >= 0 && duty <= 100) {
        this->manual_duty = duty;
        this->header.set_duty(duty);
    } else {
        this->manual_duty = -1;
    }
}

void Fan::set_smoothing_value(uint8_t value) {
    this->smoothness = value / 100.0f;
}

void Fan::set_enabled_sensors(uint8_t value) {
    this->enabled_sensors = value;
}

//////////////////////
//     Getters      //
//////////////////////
uint16_t Fan::get_rpm() {
    return this->header.get_rpm();
}

float Fan::get_duty() {
    return this->header.get_duty();
}

float Fan::get_temperature() {
    return this->temperature;
}

bool Fan::is_manual() {
    return this->manual_duty >= 0 ? true : false;
}

uint8_t Fan::get_smoothing_value() {
    return this->smoothness;
}

InterfaceAlgorithm* Fan::get_algorithm(uint8_t index) {
    return algorithms.at(index);
}

uint8_t Fan::get_enabled_sensors() {
    return this->enabled_sensors;
}

//////////////////////
// InterfaceEEPROM  //
//////////////////////
void Fan::setEEPROMaddr(uint16_t addr) {
    this->addr = addr;
}

void Fan::writeToEEPROM() {
    EEPROM.writeUChar(this->addr, this->enabled_sensors);
    EEPROM.writeChar(this->addr + 1, this->manual_duty);
    for (uint8_t i  = 0; i < algorithms.size(); i++) {
        algorithms[i]->writeToEEPROM();
    }
}

void Fan::readFromEEPROM() {
    this->enabled_sensors = EEPROM.readUChar(this->addr);
    this->manual_duty = EEPROM.readChar(this->addr + 1);
    algorithms.clear();
    for (uint8_t i  = 0; i < 4; i++) {
        algorithms.push_back((InterfaceAlgorithm*)new Curve(5));
        algorithms[algorithms.size() - 1]->setEEPROMaddr(this->addr + 2 + i * algorithms[algorithms.size() - 1]->sizeOnEEPROM());
        algorithms[i]->readFromEEPROM();
    }
}

uint16_t Fan::sizeOnEEPROM() {
    return 2 + 15 * 4;
}

uint16_t Fan::max_size_on_EEPROM() {
    return 2 + 15 * 4;
}