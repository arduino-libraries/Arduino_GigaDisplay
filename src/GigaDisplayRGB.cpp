#include "GigaDisplayRGB.h"

GigaDisplayRGB::GigaDisplayRGB(){
}

void GigaDisplayRGB::begin()
{
    Wire1.begin();
    writeByte(0x1, 0xF1);
    writeByte(0x2, 0xFF);
}

void GigaDisplayRGB::on(uint8_t r, uint8_t g, uint8_t b)
{
    writeByte(0x10, r);
    writeByte(0x11, g);
    writeByte(0x12, b);
    writeByte(0x2b, 0xc5);
}

void GigaDisplayRGB::off()
{
    on(0, 0, 0);
}

void GigaDisplayRGB::writeByte(uint8_t subAddress, uint8_t data)
{
    Wire1.beginTransmission(0x50);
    Wire1.write(subAddress);
    Wire1.write(data);
    Wire1.endTransmission();
}

#ifdef __ZEPHYR__
GigaDisplayBacklight* GigaDisplayBacklight::_instance = nullptr;

void GigaDisplayBacklight::zephyr_timer_handler(struct k_timer *timer_id) {
    if (_instance) {
        _instance->process_step();
    }
}
#endif

GigaDisplayBacklight::GigaDisplayBacklight() : intensity(0) {
    #ifdef __ZEPHYR__
    _instance = this;
    #endif
}

void GigaDisplayBacklight::begin(int target_percent) {
    set(target_percent);

#if defined(__MBED__)
    pin = new mbed::DigitalOut(PB_12);
    ticker = new mbed::Ticker();
    ticker->attach(mbed::callback(this, &GigaDisplayBacklight::cb), 2ms);
#elif defined(__ZEPHYR__)
    pinMode(74, OUTPUT); // 74 = PB_12
    k_timer_init(&timer, GigaDisplayBacklight::zephyr_timer_handler, NULL);
    k_timer_start(&timer, K_MSEC(2), K_MSEC(2));
#endif
}

#if defined(__MBED__)
void GigaDisplayBacklight::cb() {
    static int counter = 0;
    if (counter >= intensity) {
        *pin = 0;
    } else {
        *pin = 1;
    }
    counter += 10;
    if (counter == 100) counter = 0;
}
#elif defined(__ZEPHYR__)
void GigaDisplayBacklight::process_step() {
    static int counter = 0;
    if (counter >= intensity) {
        digitalWrite(74, LOW); 
    } else {
        digitalWrite(74, HIGH);
    }
    counter += 10;
    if (counter == 100) counter = 0;
}
#endif

void GigaDisplayBacklight::set(int target_percent) {
    intensity = target_percent;
}

void GigaDisplayBacklight::off() {
    set(0);
}