#ifndef Header_h
#define Header_h

#include <Arduino.h>

class Header
{
public:
    // Empty Constructor
    Header();
    // Constructor
    Header(
        uint8_t pwm_pin,
        uint8_t pwm_channel,
        uint8_t rpm_pin,
        uint8_t pcnt_channel);
    // Updates
    void rpm_update();
    // Setters
    void set_duty(float duty);
    // Getters
    float get_duty();
    uint16_t get_rpm();

private:
    void pcnt_init(int channel, int pin);

    // Static data
    uint8_t pwm_channel;
    uint8_t pcnt_channel;

    // Dynamic data
    float duty;
    uint16_t rpm;

    // Internal variables
    ulong last_pcnt_update;
};

#endif
