/*
 * Copyright 2025 Arduino SA
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef __GIGA_DISPLAY_H__
#define __GIGA_DISPLAY_H__

#ifdef __ZEPHYR__

#include "Arduino.h"
/** 
 * @enum DisplayPixelFormat
 * @brief Display pixel format enumeration.
 * 
 * The different formats use different numbers of bits per pixel:
 * - Grayscale (8-bit)
 * - RGB565 (16-bit)
 */
enum DisplayPixelFormat {
    DISPLAY_RGB565,    /**< RGB565 format (16-bit). */
    DISPLAY_RGB888    /**< RGB888 format (16-bit). */
};

// Color definitions

#define RGB565_BLACK 0x0000       /*   0,   0,   0 */
#define RGB565_NAVY 0x000F        /*   0,   0, 128 */
#define RGB565_DARKGREEN 0x03E0   /*   0, 128,   0 */
#define RGB565_DARKCYAN 0x03EF    /*   0, 128, 128 */
#define RGB565_MAROON 0x7800      /* 128,   0,   0 */
#define RGB565_PURPLE 0x780F      /* 128,   0, 128 */
#define RGB565_OLIVE 0x7BE0       /* 128, 128,   0 */
#define RGB565_LIGHTGREY 0xC618   /* 192, 192, 192 */
#define RGB565_DARKGREY 0x7BEF    /* 128, 128, 128 */
#define RGB565_BLUE 0x001F        /*   0,   0, 255 */
#define RGB565_GREEN 0x07E0       /*   0, 255,   0 */
#define RGB565_CYAN 0x07FF        /*   0, 255, 255 */
#define RGB565_RED 0xF800         /* 255,   0,   0 */
#define RGB565_MAGENTA 0xF81F     /* 255,   0, 255 */
#define RGB565_YELLOW 0xFFE0      /* 255, 255,   0 */
#define RGB565_WHITE 0xFFFF       /* 255, 255, 255 */
#define RGB565_ORANGE 0xFD20      /* 255, 165,   0 */
#define RGB565_GREENYELLOW 0xAFE5 /* 173, 255,  47 */
#define RGB565_PINK 0xF81F

/**
 * @class Display
 * @brief The main class for controlling a camera.
 */
class Display {
private:
    const struct device *gdev;
    struct display_buffer_descriptor *buf_desc;
    
 protected:
    int16_t _height, _width;

public:
    /**
    * @brief Construct a new Camera object.
    */
    Display();

    /**
    * @brief Initialize the display
    */
    bool begin(DisplayPixelFormat pixformat = DISPLAY_RGB565, int rotation = 0);
    
    /**
    * @brief a frame.
    * 
    * @param fb Reference to a FrameBuffer object to store the frame data.
    * @param timeout Time in milliseconds to wait for a frame (default: 5000).
    * @return true if the frame is successfully captured, otherwise false.
    */
    //bool grabFrame(FrameBuffer &fb, uint32_t timeout = 5000);
    
    
    /**
    *
    *
    *
    *
    */
    int write8(const uint16_t x,
        const uint16_t y,
        const void *buf);
        
    void setFrameDesc(uint16_t w, uint16_t h, uint16_t pitch, uint32_t buf_size);
    void startFrameBuffering();
    void endFrameBuffering();

    int setBlanking(bool on);

    void* getFrameBuffer();

    int16_t width(void)  { return _width; }
    int16_t height(void) { return _height; }

   
};

#endif // __GIGA_DISPLAY_H__

#endif // __ZEPHYR__