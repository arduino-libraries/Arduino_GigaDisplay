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
