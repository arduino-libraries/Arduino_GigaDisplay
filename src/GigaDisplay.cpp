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


#if __has_include ("lvgl.h")
#include "lvgl.h"
#endif

/* Private function prototypes -----------------------------------------------*/
#if __has_include ("lvgl.h")
#if __MBED__
#include "mbed.h"
#endif
#if (LVGL_VERSION_MAJOR == 9)
void lvgl_displayFlushing(lv_display_t * display, const lv_area_t * area, unsigned char * px_map);
#if __MBED__
static void inc_thd() {
    while (1) {
      lv_tick_inc(16);
      delay(16);
    }
}
static rtos::Thread lvgl_inc_thd;
#endif
#else
void lvgl_displayFlushing(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
#endif
#endif

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
  
#if defined( HAS_ARDUINOGRAPHICS) || __has_include ("lvgl.h") 
  printk("setting up buffer....\n");
  setBlanking(false);
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
  setBlanking(true);

  sizeof_framebuffer = width() * height() * sizeof(uint16_t);
  setFrameDesc(width(), height(), width(), sizeof_framebuffer);
  Serial.print("Buffer: 0x"); Serial.println((uint32_t)buffer, HEX);
  
  // turn on the display backlight
  pinMode(74, OUTPUT);
  digitalWrite(74, HIGH);
#endif

#ifdef HAS_ARDUINOGRAPHICS
  if (!ArduinoGraphics::begin()) {
    return 1; /* Unknown err */
  }

  textFont(Font_5x7);
#endif //arduinoGraphics


  #if __has_include("lvgl.h")
  printk("initializing LVGL!!!!\n");
    /* Initiliaze LVGL library */
    lv_init();


  #if (LVGL_VERSION_MAJOR == 9)
    /* Create a draw buffer */
    static lv_color_t * buf1 = (lv_color_t*)malloc((width() * height() / 10)); /* Declare a buffer for 1/10 screen size */
    if (buf1 == NULL) {
      return 2; /* Insuff memory err */
    }

    lv_display_t *display;
    if(_rotated) {
      display = lv_display_create(height(), width());
      lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
      //display->sw_rotate = 1;
    } else {
      display = lv_display_create(width(), height());
    }
    lv_display_set_buffers(display, buf1, NULL, width() * height() / 10, LV_DISPLAY_RENDER_MODE_PARTIAL);  /*Initialize the display buffer.*/
    lv_display_set_flush_cb(display, lvgl_displayFlushing);
   #if __MBED__
    lvgl_inc_thd.start(inc_thd);
   #endif
    printk("LVGL Initialized\n");

  #else //LVGL_VERSION_MAJOR

      /* Create a draw buffer */
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t * buf1;
    buf1 = (lv_color_t*)malloc((width() * height() / 10) * sizeof(lv_color_t)); /* Declare a buffer for 1/10 screen size */
    if (buf1 == NULL) {
      return 2; /* Insuff memory err */
    }
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, width() * height() / 10);      /* Initialize the display buffer. */

    /* Initialize display features for LVGL library */
    static lv_disp_drv_t disp_drv;              /* Descriptor of a display driver */
    lv_disp_drv_init(&disp_drv);                /* Basic initialization */
    disp_drv.flush_cb = lvgl_displayFlushing;   /* Set your driver function */
    disp_drv.draw_buf = &draw_buf;              /* Assign the buffer to the display */
    if(_rotated) {
      disp_drv.hor_res = height();        /* Set the horizontal resolution of the display */
      disp_drv.ver_res = width();         /* Set the vertical resolution of the display */
      disp_drv.rotated  = LV_DISP_ROT_270;
    } else {
      disp_drv.hor_res = width();         /* Set the horizontal resolution of the display */
      disp_drv.ver_res = height();        /* Set the vertical resolution of the display */
      disp_drv.rotated  = LV_DISP_ROT_NONE;
    }
    disp_drv.sw_rotate = 1;
    lv_disp_drv_register(&disp_drv);        /* Finally register the driver */
  #endif
  #endif


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

#if __has_include("lvgl.h")
#if (LVGL_VERSION_MAJOR == 9)
static uint8_t* rotated_buf = nullptr;
void lvgl_displayFlushing(lv_display_t * disp, const lv_area_t * area, unsigned char * px_map) {
    uint32_t w     = lv_area_get_width(area);
    uint32_t h     = lv_area_get_height(area);
    lv_area_t* area_in_use = (lv_area_t *)area;

    // TODO: find a smart way to tackle sw rotation
    lv_display_rotation_t rotation = lv_display_get_rotation(disp);
    lv_area_t rotated_area;
    if (rotation != LV_DISPLAY_ROTATION_0) {
        rotated_buf = (uint8_t*)realloc(rotated_buf, w * h * 4);
        lv_color_format_t cf = lv_display_get_color_format(disp);
        #if (LVGL_VERSION_MINOR < 2) 
        rotation = LV_DISPLAY_ROTATION_90; // bugfix: force 90 degree rotation for lvgl 9.1 end earlier
        #endif
        lv_draw_sw_rotate(px_map, rotated_buf,
                          w, h, lv_draw_buf_width_to_stride(w, cf),
                          lv_draw_buf_width_to_stride(h, cf),
                          rotation, cf);

        rotated_area.x1 = lv_display_get_vertical_resolution(disp) - area->y2 - 1;
        rotated_area.y1 = area->x1;
        //rotated_area.y2 = dsi_getDisplayYSize() - area->x1 - 1;
        rotated_area.x2 = rotated_area.x1 + h - 1;
        rotated_area.y2 = rotated_area.y1 + w + 1;

        area_in_use = &rotated_area;
        px_map = rotated_buf;
        auto temp = w;
        w = h;
        h = temp;
    }

#if defined(__MBED__)
    uint32_t offsetPos  = (area_in_use->x1 + (dsi_getDisplayXSize() * area_in_use->y1)) * sizeof(uint16_t);
    dsi_lcdDrawImage((void *) px_map, (void *)(dsi_getActiveFrameBuffer() + offsetPos), w, h, DMA2D_INPUT_RGB565);
#else
    uint32_t offsetPos  = (area_in_use->x1 + (lcd_x_size * area_in_use->y1)) * sizeof(uint16_t);
    //dsi_lcdDrawImage((void *) px_map, (void *)(dsi_getActiveFrameBuffer() + offsetPos), w, h, DMA2D_INPUT_RGB565);
    //--- cant do this memcpy(px_map, buffer + offsetPos, w * h);
    Display::write8(0, 0, buffer);
#endif
    lv_display_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

#else
void lvgl_displayFlushing(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    uint32_t width      = lv_area_get_width(area);
    uint32_t height     = lv_area_get_height(area);
    uint32_t offsetPos  = (area->x1 + (dsi_getDisplayXSize() * area->y1)) * sizeof(uint16_t);
#if defined(__MBED__)
    dsi_lcdDrawImage((void *) color_p, (void *)(dsi_getActiveFrameBuffer() + offsetPos), width, height, DMA2D_INPUT_RGB565);
#else
    //--- cant do this memcpy(px_map, buffer + offsetPos, w * h);
    Display::write8(0, 0, buffer);

#endif
    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}
#endif
#endif

#endif //__ZEPHYR__