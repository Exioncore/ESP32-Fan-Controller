#ifndef RunningAverage_h
#define RunningAverage_h

#include <Arduino.h>

class RunningAverage
{
public:
    // Constructor
    RunningAverage(uint16_t numberOfSamples, uint16_t defaultValue);

    //InterfaceSensor
    void update(uint16_t value);
    uint16_t get_average();

private:
    uint32_t total;
    uint16_t *samples;
    uint16_t numberOfSamples;
    uint16_t index;
};

#endif