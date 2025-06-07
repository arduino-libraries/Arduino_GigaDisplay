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
 * Camera driver.
 */
#ifdef __ZEPHYR__

#include "Arduino.h"
#include "Arduino_GigaDisplay.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>


#ifdef HAS_ARDUINOGRAPHICS
Display::Display(int width, int height) : ArduinoGraphics(width, height), gdev(NULL)
#else
Display::Display(int width, int height) : gdev(NULL)
#endif
{
  _height   = height;
  _width    = width;
  _rotated = (width >= height) ? true : false;
  printk("height: %d, width: %d, rotated: %d\n", _height, _width, _rotated);
}

bool Display::begin(DisplayPixelFormat pixformat) {
  
  int ret = 0;

  #if DT_HAS_CHOSEN(zephyr_display)
  this->gdev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
  #endif

  if (!this->gdev || !device_is_ready(this->gdev)) {
    printk("\t<err> Zephy Display Not Ready!...");
      return false;
  }

  // Get capabilities
  struct display_capabilities capabilities = {0};
  display_get_capabilities(this->gdev, &capabilities);

  uint8_t format_spec = 0;
  
  switch (pixformat) {
      case DISPLAY_RGB565:
          //Serial.println("Set RGB565");
          if (capabilities.current_pixel_format != PIXEL_FORMAT_RGB_565) {
            ret = display_set_pixel_format(this->gdev, PIXEL_FORMAT_RGB_565);
          }
          format_spec = 1;
          break;
      case DISPLAY_RGB888:
          //Serial.println("Set RGB888");
          if (capabilities.current_pixel_format != PIXEL_FORMAT_RGB_888) {
            ret = display_set_pixel_format(this->gdev, PIXEL_FORMAT_RGB_888);
          }
          format_spec = 1;
          break;
      default:
          break;
  }

  if(format_spec == 0) {
    Serial.println("\t<err> The specified format is not supported");
    return false;
  }
   
  if (ret) {
		Serial.println("\t<err> Unable to set display format");
		return false;
	}
  
  /*
  display_orientation orientation;
  switch(rotation){
    case 0:
      orientation = DISPLAY_ORIENTATION_NORMAL;
      break;
    case 1:
      orientation = DISPLAY_ORIENTATION_ROTATED_90;
      break;
    case 2:
      orientation = DISPLAY_ORIENTATION_ROTATED_180;
      break;      
    case 3:
      orientation = DISPLAY_ORIENTATION_ROTATED_270;
      break;
    default:
      orientation = DISPLAY_ORIENTATION_NORMAL;
      break;   
  }
  //Rotation not supported
  Serial.print("Orientation: "); Serial.println(orientation);
  ret = display_set_orientation(this->gdev, orientation);
  Serial.println(ret);
  if(ret) {
    Serial.println("\t<err> Failed to set display rotation");
    //return false;
  }
  */
  
  display_get_capabilities(this->gdev, &capabilities);

	printk("- Capabilities:\n");
	printk("  x_resolution = %u, y_resolution = %u\n, supported_pixel_formats = %u\n"
	       "  current_pixel_format = %u, current_orientation = %u\n",
	       capabilities.x_resolution, capabilities.y_resolution,
	       capabilities.supported_pixel_formats, capabilities.current_pixel_format,
	       capabilities.current_orientation);
#ifndef HAS_ARDUINOGRAPHICS
  _height = capabilities.y_resolution;
  _width = capabilities.x_resolution;
#endif
  
#ifdef HAS_ARDUINOGRAPHICS  
#ifdef CONFIG_SHARED_MULTI_HEAP
  void* ptrFB = getFrameBuffer();
  if (ptrFB == nullptr){
    printk("Memory not allocated successfully." );
    while(1){}
  }
  // Cast the void pointer to an int pointer to use it
  buffer = static_cast<uint16_t*>(ptrFB);
  //buffer = (uint16_t*)shared_multi_heap_aligned_alloc(SMH_REG_ATTR_EXTERNAL, 16, (this->width() * this-> height() * sizeof(uint16_t)));
#else
  SDRAM.begin();
  buffer = (uint16_t*)SDRAM.malloc(this->width() * this-> height() * sizeof(uint16_t));
#endif    
  sizeof_framebuffer = width() * height() * sizeof(uint16_t);
  setFrameDesc(width(), height(), width(), sizeof_framebuffer);
  Serial.print("Buffer: 0x"); Serial.println((uint32_t)buffer, HEX);
  
  // turn on the display backlight
  pinMode(74, OUTPUT);
  digitalWrite(74, HIGH);
    
  if (!ArduinoGraphics::begin()) {
    return 1; /* Unknown err */
  }

  textFont(Font_5x7);
#endif //arduinoGraphics

  return true;
}

int Display::write8(const uint16_t x,
            const uint16_t y,
            const void *buf) {
              

//  printk("Display::write8(%u %u %p) %p %x\n", x, y, buf, 
//      &((struct display_driver_api *)gdev->api)->write, *((uint32_t*)(&((struct display_driver_api *)gdev->api)->write)));

  return display_write(this->gdev, x, y, this->buf_desc, buf);
  
}

void Display::setFrameDesc(uint16_t w, uint16_t h, uint16_t pitch, uint32_t buf_size) {
	this->buf_desc->buf_size = buf_size;
	this->buf_desc->width = w;  /** Number of pixels between consecutive rows in the data buffer */
	this->buf_desc->height = h;  /** Data buffer row width in pixels */
	this->buf_desc->pitch = pitch;	/** Data buffer row height in pixels */
    
}

void Display::startFrameBuffering() {

	this->buf_desc->frame_incomplete = false;

}

void Display::endFrameBuffering() {

	this->buf_desc->frame_incomplete = true;

}

int Display::setBlanking(bool on) {
  int ret = 0;
  if(on) {
    ret = display_blanking_on(this->gdev);
  } else {
    ret = display_blanking_off(this->gdev);
  }
  if(ret < 0) {
    return false;
  }
  return true;
}

void* Display::getFrameBuffer() {
  void* fb  = display_get_framebuffer(this->gdev);
  return fb;
}


#ifdef HAS_ARDUINOGRAPHICS
void Display::beginDraw() {
  ArduinoGraphics::beginDraw();
  startFrameBuffering();
  lcdClear(0); 
}

void Display::endDraw() {
  ArduinoGraphics::endDraw();
  
  endFrameBuffering();
}

void Display::clear(){
  uint32_t bg = ArduinoGraphics::background();
  uint32_t x_size, y_size;

  if(_rotated) {
    x_size = (height() <= getDisplayXSize())? height() : getDisplayXSize();
    y_size = (width() <= getDisplayYSize())? width() : getDisplayYSize();
  } else {
    x_size = (width() <= getDisplayXSize())? width() : getDisplayXSize();
    y_size = (height() <= getDisplayYSize())? height() : getDisplayYSize();
  }

  //lcdFillArea((void *)(dsi_getCurrentFrameBuffer()), x_size, y_size, bg);
  lcdClear(bg);
}

void Display::set(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t x_rot, y_rot;

    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
      return;
    
    if (_rotated) {
      x_rot = ((height()-1) - y);
      y_rot = x;
      
      //if (x_rot >= height() || y_rot >= width()) 
      //  return;
    } else {
      x_rot = x;
      y_rot = y;

      //if (x_rot >= width() || y_rot >= height()) 
      //  return;
    }

    if (x_rot >= getDisplayXSize() || y_rot >= getDisplayYSize()) 
      return;
    //printk("%u, %u, %u, %u, %u\n", x, y, x_rot, y_rot, x_rot + (width() * y_rot));

    uint16_t color =  ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    //uint32_t color =  (uint32_t)((uint32_t)(r << 16) | (uint32_t)(g << 8) | (uint32_t)(b << 0));
    buffer[(x_rot + (getDisplayXSize() * y_rot)) ] = color;
    
}

void Display::lcdClear(uint16_t Color) {
	/* Clear the LCD */
  uint8_t hi = Color >> 8, lo = Color & 0xFF;
  if (hi == lo) {
    memset(buffer, lo, width() * height() * 2);
  } else {
    uint32_t i, pixels = width() * height();
    for (i = 0; i < pixels; i++)
      buffer[i] = Color;
  }

}
#endif  //HAS_ARDUINOGRAPHICS

#endif //__ZEPHYR__
