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

#endif