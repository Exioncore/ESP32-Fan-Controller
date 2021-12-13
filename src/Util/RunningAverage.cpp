#include "Util/RunningAverage.h"

RunningAverage::RunningAverage(uint16_t numberOfSamples, uint16_t defaultValue) {
    this->samples = new uint16_t[numberOfSamples];
    for (uint16_t i = 0; i < numberOfSamples; i++) {
        this->samples[i] = defaultValue;
    }
    this->numberOfSamples = numberOfSamples;
    this->total = defaultValue * numberOfSamples;
    this->index = 0;
}    
void RunningAverage::update(uint16_t value) {
    this->total -= this->samples[this->index];
    this->samples[this->index] = value;
    this->total += this->samples[this->index];

    this->index = this->index < (this->numberOfSamples - 1) ? this->index + 1 : 0;
}
uint16_t RunningAverage::get_average() {
    return (total / numberOfSamples);
}