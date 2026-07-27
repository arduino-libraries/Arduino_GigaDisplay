#ifndef  _ARDUINO_GIGADISPLAY_H_
#define  _ARDUINO_GIGADISPLAY_H_

#include "GigaDisplayRGB.h"

/*
 * Video output compatibility across cores:
 *  - The Zephyr core provides the "Arduino_Video" library/class.
 *  - The Mbed core provides the "Arduino_H7_Video" library/class.
 * Pull in the correct header and expose a unified "Arduino_Video" type so
 * sketches can stay core-agnostic. The video include is opt-in: define
 * ARDUINO_GIGA_DISPLAY_VIDEO before including this header (video sketches only)
 * so RGB/backlight sketches don't drag in the video library.
 */
#if defined(ARDUINO_GIGA_DISPLAY_VIDEO)
#if defined(ARDUINO_ARCH_ZEPHYR)
#include "Arduino_Video.h"
#else
#include "Arduino_H7_Video.h"
using Arduino_Video = Arduino_H7_Video;
#endif
#endif

#endif