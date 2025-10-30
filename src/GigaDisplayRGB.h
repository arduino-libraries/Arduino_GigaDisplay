#ifndef GigaDisplayRGB_h
#define GigaDisplayRGB_h

#include "Wire.h"

class GigaDisplayRGB {
    public:
        GigaDisplayRGB();
        void begin();
        void on(uint8_t,uint8_t,uint8_t);
        void off();
    private:
        void writeByte(uint8_t,uint8_t);
};

#include "mbed.h"
using namespace std::chrono_literals;
using namespace std::chrono;


class GigaDisplayBacklight {
private:
    mbed::Ticker* ticker = nullptr;
    mbed::DigitalOut* pin = nullptr;

    // Exponential Lightness Model: This constants are determined 
    // by Table II of the OSA report (exponent = 0.426, wikipedia: lightness)
    // and empirical measurement of Giga Display KD040WVFID026-01-C025A by Albertus Liberius
    static constexpr float min_pwm_percent = 3.36136f;
    static constexpr float max_pwm_percent = 100.f;
    static inline float pwm_percent_from_lightness_percent(float lightness_percent) {
        return min_pwm_percent * pow(max_pwm_percent / min_pwm_percent, lightness_percent / 100);
    }

    // Specification by LED Driver chip LV52204MT
    inline static constexpr std::chrono::microseconds tick = 50us;    // Ton(20us) < tick < Tw0(100us)
                                                            // to prevent digital mode and goto pwm mode
    inline static constexpr uint16_t start_sequence_size = 50;  // more than Tsel = 2.2ms 
                                                            // (mode selection determination time period)

    // PDM counter
    uint16_t intensity = 0xFFFF; //maxIntensity
    uint16_t counter = 0;
    uint16_t start_seq_cnt = 0;

    void callback() {
        if (start_sequence_size > start_seq_cnt) {
            *pin = start_seq_cnt++ & 1;
        } else {
            // PDM
            static uint16_t newcounter = 0;
            newcounter = (counter + intensity) & 0xFFFF;
            *pin = (counter > newcounter) ? 1 : 0;
            counter = newcounter;
        }
    }
public:
    GigaDisplayBacklight() {}

    void begin(float target_percent=100.f) {
        if (pin) delete pin;
        if (ticker) {
            ticker->detach();
            delete ticker;
        }
        pin = new mbed::DigitalOut(PB_12);
        ticker = new mbed::Ticker();
        ticker->attach(mbed::callback(this, &GigaDisplayBacklight::callback), tick); 
        set(target_percent);
    }

    void set(float target_percent) {
        if (0 == intensity) start_seq_cnt = 0; // restart initialization sequence
        intensity = static_cast<uint16_t>(
            constrain(
                pwm_percent_from_lightness_percent(target_percent),
                min_pwm_percent, 
                max_pwm_percent
            ) / 100.f * 65535.f
        );
    }

    void off() {
        intensity = 0; // if it is longer than 8.9ms, it is shutdown mode.
    }

    inline virtual ~GigaDisplayBacklight() {
        if (ticker) {
            ticker->detach();
            delete ticker;
        }
        if (pin) {
            delete pin;
        }
    }

};


#endif
