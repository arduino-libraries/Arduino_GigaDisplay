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
    public:
        GigaDisplayBacklight() {}
        void begin(int target_percent = 100) {
            pin = new mbed::DigitalOut(PB_12);
            ticker = new mbed::Ticker();
            ticker->attach(mbed::callback(this, &GigaDisplayBacklight::cb), 2ms);
            set(target_percent);
        }
        void cb() {
            static int counter = 0;
            if (counter >= intensity) {
                *pin = 0;
            } else {
                *pin = 1;
            }
            counter += 10;
            if (counter == 100) {
                counter = 0;
            }
        }
        void set(int target_percent) {
            intensity = target_percent;
        }
        void off() {
            set(0);
        }
    private:
        mbed::Ticker* ticker;
        mbed::DigitalOut* pin;
        int intensity;
};

#endif
