#pragma once

#include <vector>
#include <mutex>
#include <ArduinoNvs.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include "images.h"
#include "config.h"
#include "time_controller.h"
#include "wifi_controller.h"

#define PANEL_CHAIN 1
#define PANEL_RES_X 64 
#define PANEL_RES_Y 32

enum ImageType {
    NOT_IMAGE             = -1,
    CLOCK_IMAGE           =  0,
    DHT_IMAGE             =  1,
    BEAUTIFUL_CLOCK_IMAGE =  2
};

class DisplayController {
    public:
    static DisplayController& getInstance() {
        static DisplayController instance;
        return instance;
    }
    
    void begin() {
        display->begin();
    }
    
    int getBrightness() {
        mutex_brightness.lock();
        int brightness = this->brightness;
        mutex_brightness.unlock();
        return brightness;
    }
    
    void setBrightness(int value) {
        mutex_brightness.lock();
        brightness = value;
        mutex_brightness.unlock();
    }
    
    enum ImageType getImage() {
        mutex_image.lock();
        enum ImageType image = this->image;
        mutex_image.unlock();
        return image;
    }
    
    void setImage(enum ImageType image) {
        mutex_image.lock();
        this->image = image;
        mutex_image.unlock();
    }
    
    MatrixPanel_I2S_DMA* getDisplay() {
        return display;
    }

    void setColor(int r, int g, int b) {
        if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
            return;
        }
        mutex_color.lock();
        color = display->color565(r, g, b);
        mutex_color.unlock();
    }

    uint16_t getColor() {
        mutex_color.lock();
        uint16_t color = this->color;
        mutex_color.unlock();
        return color;
    }
    
    private:
    DisplayController() : brightness(100), image(ImageType::BEAUTIFUL_CLOCK_IMAGE) {
        HUB75_I2S_CFG mxconfig(
            PANEL_RES_X,   // Ширина одной панели
            PANEL_RES_Y,   // Высота одной панели
            PANEL_CHAIN    // Количество панелей в цепочке
        );
        
        display = new MatrixPanel_I2S_DMA(mxconfig);
        
        if (NVS.begin()) {
            brightness = NVS.getInt("brightness");
            if (brightness) {
                this->brightness = brightness;
            }
            
            int r = NVS.getInt("color_r", -1);
            int g = NVS.getInt("color_g", -1);
            int b = NVS.getInt("color_b", -1);
            if (r != -1 && g != -1 && b != -1) {
                color = display->color565(r, g, b);
            } else {
                color = display->color565(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
            }
            NVS.close();
        }
    }

    DisplayController(DisplayController const&) = delete;
    void operator=(DisplayController const&) = delete;
    
    ~DisplayController() {
        delete display;
    }
    
    uint16_t color;
    std::mutex mutex_color;
    int brightness;
    std::mutex mutex_brightness;
    enum ImageType image;
    std::mutex mutex_image;
    MatrixPanel_I2S_DMA* display;
};

void draw_image_clock(MatrixPanel_I2S_DMA* display, bool init);
void draw_image_temperature(MatrixPanel_I2S_DMA* display, bool init);
void draw_image_beautiful_clock(MatrixPanel_I2S_DMA* display, bool init);

static std::vector<void (*)(MatrixPanel_I2S_DMA*, bool)> image_draws{
    draw_image_clock,
    draw_image_temperature,
    draw_image_beautiful_clock
};
