#include "Fan/Header.h"
#include "driver/pcnt.h"

#define MAX_NAME_LEN            15      // 15 characters max length
#define PWM_FREQUENCY           25000   // 25 KHz
#define PWM_RESOLUTION          12      // 12 bit resolution
#define MAX_PWM_VALUE           ((uint16_t)pow(2, PWM_RESOLUTION) - 1)
#define PWM_REVERSED

#define MIN_PCNT_COUNT          50
#define PCNT_TIMEOUT_MS         5000

Header::Header(){}

Header::Header(
    uint8_t pwm_pin,
    uint8_t pwm_channel,
    uint8_t rpm_pin,
    uint8_t pcnt_channel
) {
    this->pwm_channel = pwm_channel;
    this->pcnt_channel = pcnt_channel;
    // Pins setup
    pinMode(pwm_pin, OUTPUT);
    pinMode(rpm_pin, INPUT_PULLUP);
    // PWM setup
    ledcSetup(pwm_channel, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(pwm_pin, pwm_channel);
    // RPM setup
    this->rpm = 0;
    pcnt_init(pcnt_channel, rpm_pin);
    last_pcnt_update = millis();
}
////////////////////////
//       Updates      //
////////////////////////
void Header::rpm_update() {
    ulong time = millis();
    // PCNT
    int16_t pcnt_val;
    pcnt_get_counter_value((pcnt_unit_t)pcnt_channel, &pcnt_val);
    // Check triggers
    bool pcnt_count_trigger = pcnt_val >= MIN_PCNT_COUNT;
    bool pcnt_timeout_trigger = time - last_pcnt_update >= PCNT_TIMEOUT_MS;
    // Update RPM
    if (pcnt_count_trigger)
        this->rpm = round((pcnt_val / 2.0) / ((time - last_pcnt_update) / 1000.0) * 60.0);
    else if (pcnt_timeout_trigger)
        this->rpm = 0;
    // Clear PCNT if RPM updated
    if (pcnt_count_trigger || pcnt_timeout_trigger) {
        pcnt_counter_clear((pcnt_unit_t)pcnt_channel);
        last_pcnt_update = time;
    }
}
////////////////////////
//      Setters       //
////////////////////////
void Header::set_duty(float duty) {
    this->duty = duty;
#ifdef PWM_REVERSED
    ledcWrite(pwm_channel, MAX_PWM_VALUE - (uint16_t)roundf(MAX_PWM_VALUE / 100.0 * duty));
#else
    ledcWrite(pwm_channel, (uint16_t)roundf(MAX_PWM_VALUE / 100.0 * duty));
#endif
}
////////////////////////
//     Getters        //
////////////////////////
float Header::get_duty() {
    return this->duty;
}

uint16_t Header::get_rpm() {
    return this->rpm;
}
////////////////////////
// Internal functions //
////////////////////////
void Header::pcnt_init(int channel, int pin) {
    pcnt_config_t pcnt_config;
    // Base setup
    pcnt_config.pulse_gpio_num = pin;
    pcnt_config.ctrl_gpio_num = PCNT_PIN_NOT_USED;
    pcnt_config.channel = PCNT_CHANNEL_0;
    pcnt_config.unit = (pcnt_unit_t)channel;
    // Setup counting edge
    pcnt_config.pos_mode = PCNT_COUNT_INC;
    pcnt_config.neg_mode = PCNT_COUNT_DIS;
    // Setup control input
    pcnt_config.lctrl_mode = PCNT_MODE_KEEP;
    pcnt_config.hctrl_mode = PCNT_MODE_REVERSE;
    // Setup counter boundaries
    pcnt_config.counter_h_lim = INT16_MAX;
    pcnt_config.counter_l_lim = 0;

    // Initialize PCNT unit
    pcnt_unit_config(&pcnt_config);

    // Setup filtering
    pcnt_set_filter_value((pcnt_unit_t)channel, 100);
    pcnt_filter_enable((pcnt_unit_t)channel);

    // Reset PCNT counter
    pcnt_counter_clear((pcnt_unit_t)channel);
}
