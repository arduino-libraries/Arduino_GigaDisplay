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

#if defined(__MBED__)
    #include "mbed.h"
    using namespace std::chrono_literals;
    using namespace std::chrono;
#elif defined(__ZEPHYR__)
    #include <zephyr/kernel.h>
#endif

class GigaDisplayBacklight {
    public:
        GigaDisplayBacklight();
        void begin(int target_percent = 100);
        void set(int target_percent);
        void off();

    private:
        int intensity;
        
        #if defined(__MBED__)
            mbed::Ticker* ticker;
            mbed::DigitalOut* pin;
            void cb();
        #elif defined(__ZEPHYR__)
            struct k_timer timer;
            static void zephyr_timer_handler(struct k_timer *dummy);
            static GigaDisplayBacklight* _instance; 
            void process_step();
        #endif
};

#endif
