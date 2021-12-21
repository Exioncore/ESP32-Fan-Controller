#include <EEPROM.h>
#include "Fan/Algorithms/PID.h"

//////////////////////
//   Constructors   //
//////////////////////
PID::PID() {}

//////////////////////
//  Configuratiion  //
//////////////////////
void PID::set_P(float p)
{
    this->p = p;
}
void PID::set_I(float i)
{
    this->i = i;
    this->max_integral = (max - min) / i;
}
void PID::set_D(float d)
{
    this->d = d;
}
void PID::set_min(uint8_t min)
{
    this->min = min;
    this->max_integral = (max - min) / i;
}
void PID::set_max(uint8_t max)
{
    this->max = max;
    this->max_integral = (max - min) / i;
}
void PID::set_target(int8_t target)
{
    this->target = target;
}

float PID::get_P()
{
    return this->p;
}
float PID::get_I()
{
    return this->i;
}
float PID::get_D()
{
    return this->d;
}
uint8_t PID::get_min()
{
    return this->min;
}
uint8_t PID::get_max()
{
    return this->max;
}
int8_t PID::get_target()
{
    return this->target;
}

//////////////////////////////////
// InterfaceAlgorithm Functions //
//////////////////////////////////
void PID::set_ambient_temperature(float temperature)
{
    this->ambient_temperature = temperature;
}

void PID::set_ambient_reference(bool enable)
{
    this->use_ambient_temperature = enable;
}

bool PID::is_using_ambient_reference()
{
    return this->use_ambient_temperature;
}

float PID::get_duty_at(float temperature)
{
    // Delta time
    double dt = (millis() - lastUpdate) / 1000.0;
    // Error
    double error = temperature - (ambient_temperature + target);

    // Proportional term
    double Pout = p * error;
    Pout = Pout < 0 ? 0 : Pout;
    // Integral term
    _integral += error * dt;
    _integral = _integral > max_integral ? max_integral : (_integral < 0 ? 0 : _integral);
    double Iout = i * _integral;
    // Derivative term
    double Dout = d * (error - _pre_error) / dt;
    Dout = Dout < 0 ? 0 : Dout;

    // Compute Output
    float output = Pout + Iout + Dout;
    // Add min to output
    output += min;
    // Keep output within boundaries
    output = output < min ? min : (output > max ? max : output);
    // Update time
    lastUpdate = millis();

    return output;
}

/////////////////////
// InterfaceEEPROM //
/////////////////////
void PID::setEEPROMaddr(uint16_t addr)
{
    this->addr = addr;
}
void PID::writeToEEPROM()
{
    EEPROM.writeFloat(this->addr + 0, p);
    EEPROM.writeFloat(this->addr + 4, i);
    EEPROM.writeFloat(this->addr + 8, d);
    EEPROM.writeUChar(this->addr + 12, min);
    EEPROM.writeUChar(this->addr + 13, max);
    EEPROM.writeBool(this->addr + 14, use_ambient_temperature);
    EEPROM.writeChar(this->addr + 15, target);
}
void PID::readFromEEPROM()
{
    this->p = EEPROM.readFloat(this->addr + 0);
    this->p = isnan(this->p) ? 0 : (this->p > 100 ? 100 : this->p);
    this->i = EEPROM.readFloat(this->addr + 4);
    this->i = isnan(this->i) ? 0 : (this->i > 100 ? 100 : this->i);
    this->d = EEPROM.readFloat(this->addr + 8);
    this->d = isnan(this->d) ? 0 : (this->d > 100 ? 100 : this->d);
    this->min = EEPROM.readUChar(this->addr + 12);
    this->min = this->min < 0 ? 0 : (this->min > 100 ? 100 : this->min);
    this->max = EEPROM.readUChar(this->addr + 13);
    this->max = this->max < 0 ? 0 : (this->max > 100 ? 100 : this->max);
    this->use_ambient_temperature = EEPROM.readBool(this->addr + 14);
    this->target = EEPROM.readChar(this->addr + 15);
    this->target = this->target < 0 ? 0 : (this->target > 100 ? 100 : this->target);
    this->max_integral = (max - min) / i;
}
uint16_t PID::sizeOnEEPROM()
{
    return 4 * 3 + 4;
}