#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <time.h>
#include <cmath>

#include "display.h"
#include "time_controller.h"
#include "wifi_controller.h"
#include "temperature_controller.h"

static uint32_t BEAUTIFUL_CLOCK_ANIMATION_DELAY = ANIMATION_DELAY;

std::vector<std::pair<int, int>> BEAUTIFUL_CLOCK_NUMS_POSITIONS = {
    {3,  9},
    {12, 9},
    {24, 9},
    {33, 9},
    {45, 9},
    {54, 9}
};

void transition_from0to1(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from1to2(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from2to3(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from3to4(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from4to5(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from5to6(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from6to7(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from7to8(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from8to9(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from9to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);

void transition_from2to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from3to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from4to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void transition_from5to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);

void draw_squares(MatrixPanel_I2S_DMA* display, uint16_t color);

void draw_num(MatrixPanel_I2S_DMA* display, int num, int x, int y, uint16_t color);
void draw_num0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num1(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num2(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num3(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num4(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num5(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num6(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num7(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num8(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);
void draw_num9(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color);

void update_time(struct tm& timeinfo, std::vector<int>& numbers);

void draw_image_clock(MatrixPanel_I2S_DMA* display, bool init) {
    static DisplayController& displayController = DisplayController::getInstance();
    display->clearScreen();
    display->setTextSize(1);
    display->setCursor(8, 6);
    // display->setTextColor(displayController.getColor());
    display->setTextColor(display->color565(255, 255, 255));

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    } else {
        display->printf("%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        display->setCursor(2, 20);
        display->printf("%02d.%02d.%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    }
}

void draw_image_temperature(MatrixPanel_I2S_DMA* display, bool init) {
    static TemperatureController& temperatureController = TemperatureController::getInstance();
    static DisplayController&     displayController     = DisplayController::getInstance();
    display->clearScreen();
    display->setTextSize(1);
    display->setTextColor(displayController.getColor());
    
    display->setCursor(2, 4);
    display->printf("Temp: %.1f", temperatureController.getTemperature());
    display->setCursor(2, 12);
    display->printf("Hum:  %.1f", temperatureController.getHumidity());
    display->setCursor(2, 20);
    display->printf("Heat: %.1f", temperatureController.getHeatIndex());
}

void update_time(struct tm& timeinfo, std::vector<int>& numbers) {
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
    } else {
        numbers[0] = timeinfo.tm_hour / 10;
        numbers[1] = timeinfo.tm_hour % 10;
        numbers[2] = timeinfo.tm_min / 10;
        numbers[3] = timeinfo.tm_min % 10;
        numbers[4] = timeinfo.tm_sec / 10;
        numbers[5] = timeinfo.tm_sec % 10;
    }
}

void draw_image_beautiful_clock(MatrixPanel_I2S_DMA* display, bool init) {
    static DisplayController& displayController = DisplayController::getInstance();
    static uint16_t color = displayController.getColor();

    static std::vector<int> numbers     = {0, 0, 0, 0, 0, 0};
    static std::vector<int> new_numbers = {0, 0, 0, 0, 0, 0};

    static struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
    } else {
        if (init) {
            update_time(timeinfo, numbers);
            draw_squares(display, color);
            for (int i = 0; i < 6; ++i) {
                draw_num(display, numbers[i], BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            }
        }

        update_time(timeinfo, new_numbers); 
        for (int i = 5; i >=0; --i) {
            if (new_numbers[i] == numbers[i]) {
                continue;
            } else if (new_numbers[i] == 1 && numbers[i] == 0) {
                transition_from0to1(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 2 && numbers[i] == 1) {
                transition_from1to2(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 3 && numbers[i] == 2) {
                transition_from2to3(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 4 && numbers[i] == 3) {
                transition_from3to4(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 5 && numbers[i] == 4) {
                transition_from4to5(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 6 && numbers[i] == 5) {
                transition_from5to6(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 7 && numbers[i] == 6) {
                transition_from6to7(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 8 && numbers[i] == 7) {
                transition_from7to8(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 9 && numbers[i] == 8) {
                transition_from8to9(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 9) {
                transition_from9to0(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 6) {
                transition_from6to7(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 5) {
                transition_from5to0(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 4) {
                transition_from4to0(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 3) {
                transition_from3to0(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 2) {
                transition_from2to0(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else if (new_numbers[i] == 0 && numbers[i] == 9) {
                transition_from9to0(display, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color);
            } else {
                draw_num(display, new_numbers[i], BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].first, BEAUTIFUL_CLOCK_NUMS_POSITIONS[i].second, color); 
            }
            numbers[i] = new_numbers[i];
        }
    }
}

void draw_squares(MatrixPanel_I2S_DMA* display, uint16_t color) {
    display->drawRect(21, 13, 2, 2, color);
    display->drawRect(21, 18, 2, 2, color);

    display->drawRect(42, 13, 2, 2, color);
    display->drawRect(42, 18, 2, 2, color);
}

void transition_from0to1(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 7; ++i) {
        display->drawPixel(x+i+1, y, display->color565(0,0,0));
        display->drawPixel(x+i+1, y+14, display->color565(0,0,0));
        
        display->drawLine(x+i,   y,   x+i,   y+13, display->color565(0,0,0));
        display->drawLine(x+i+1, y+1, x+i+1, y+6,  color);
        display->drawLine(x+i+1, y+8, x+i+1, y+13,  color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from1to2(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 6; i >= 0; --i) {
        if (i > 0) {
            display->drawPixel(x+i, y,    color);
            display->drawPixel(x+i, y+7,  color);
            display->drawPixel(x+i, y+14, color);
        }
        
        display->drawLine(x+i+1, y+8, x+i+1, y+13,  display->color565(0,0,0));
        display->drawLine(x+i, y+8, x+i, y+13,  color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from2to3(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 7; ++i) {
        display->drawLine(x+i, y+8, x+i, y+13, display->color565(0,0,0));
        display->drawLine(x+i+1, y+8, x+i+1, y+13, color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from3to4(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+1+i, y+14, display->color565(0,0,0));

        display->drawPixel(x+6-i, y, display->color565(0,0,0));
        display->drawPixel(x, y+1+i, color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from4to5(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+14, color);
        
        display->drawPixel(x+7, y+6-i, display->color565(0,0,0));
        display->drawPixel(x+6-i, y, color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from5to6(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 7; ++i) {
        display->drawLine(x+6-i, y+8, x+6-i, y+13, color);
        if (i > 0) {
            display->drawLine(x+7-i, y+8, x+7-i, y+13, display->color565(0,0,0));
        }
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from6to7(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 7; ++i) {
        display->drawPixel(x+i, y+7, display->color565(0,0,0));
        display->drawPixel(x+i, y+14, display->color565(0,0,0));

        display->drawLine(x+i, y+1, x+i, y+6, display->color565(0,0,0));
        display->drawLine(x+i, y+8, x+i, y+13, display->color565(0,0,0));

        display->drawLine(x+1+i, y+1, x+1+i, y+6, color);
        display->drawLine(x+1+i, y+8, x+1+i, y+13, color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from7to8(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {        
        display->drawPixel(x+6-i, y+7, color);
        display->drawPixel(x+6-i, y+14, color);

        display->drawLine(x+6-i, y+1, x+6-i, y+6, color);
        display->drawLine(x+6-i, y+8, x+6-i, y+13, color);
        
        if (i > 0) {
            display->drawLine(x+7-i, y+1, x+7-i, y+6, display->color565(0,0,0));
            display->drawLine(x+7-i, y+8, x+7-i, y+13, display->color565(0,0,0));
        }
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
    display->drawLine(x, y+1, x, y+6, color);
    display->drawLine(x, y+8, x, y+13, color);
    display->drawLine(x+1, y+1, x+1, y+6, display->color565(0,0,0));
    display->drawLine(x+1, y+8, x+1, y+13, display->color565(0,0,0));
    vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
}

void transition_from8to9(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 7; ++i) {
        display->drawLine(x+i, y+8, x+i, y+13, display->color565(0,0,0));
        display->drawLine(x+i+1, y+8, x+i+1, y+13, color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from9to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x, y+8+i, color);    
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from2to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+1+i, y+7, display->color565(0,0,0));
        display->drawPixel(x+7, y+8+i, color);

        display->drawLine(x+6-i, y+1, x+6-i, y+6, color);

        if (i > 0) {
            display->drawLine(x+7-i, y+1, x+7-i, y+6, display->color565(0,0,0));
        }
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
    display->drawLine(x, y+1, x, y+6, color);
    display->drawLine(x+1, y+1, x+1, y+6, display->color565(0,0,0));
    vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
}

void transition_from4to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+1+i, y, color);

        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x, y+8+i, color);

        display->drawPixel(x+6-i, y+14, color);

        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from3to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x, y+6-i, color);
        display->drawPixel(x, y+8+i, color);

        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from5to0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x+1+i, y+7, display->color565(0,0,0));

        display->drawPixel(x, y+8+i, color);
        display->drawPixel(x+7, y+6-i, color);
        vTaskDelay(BEAUTIFUL_CLOCK_ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void draw_num0(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    display->drawLine(x+1, y, x+6, y, color);
    display->drawLine(x+1, y+14, x+6, y+14, color);
    
    display->drawLine(x, y+1, x, y+6, color);
    display->drawLine(x, y+8, x, y+13, color);

    display->drawLine(x+7, y+1, x+7, y+6, color);
    display->drawLine(x+7, y+8, x+7, y+13, color);
}

void draw_num1(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num0(display, x, y, color);
    transition_from0to1(display, x, y, color);
}



void draw_num2(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num1(display, x, y, color);
    transition_from1to2(display, x, y, color);
}

void draw_num3(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num2(display, x, y, color);
    transition_from2to3(display, x, y, color);
}

void draw_num4(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num3(display, x, y, color);
    transition_from3to4(display, x, y, color);
}

void draw_num5(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num4(display, x, y, color);
    transition_from4to5(display, x, y, color);
}

void draw_num6(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num5(display, x, y, color);
    transition_from5to6(display, x, y, color);
}

void draw_num7(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num6(display, x, y, color);
    transition_from6to7(display, x, y, color);
}

void draw_num8(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num7(display, x, y, color);
    transition_from7to8(display, x, y, color);
}

void draw_num9(MatrixPanel_I2S_DMA* display, int x, int y, uint16_t color) {
    draw_num8(display, x, y, color);
    transition_from8to9(display, x, y, color);
}

void draw_num(MatrixPanel_I2S_DMA* display, int num, int x, int y, uint16_t color) {
    BEAUTIFUL_CLOCK_ANIMATION_DELAY = ANIMATION_DELAY_QUICK;
    switch (num) {
        case 0:
            draw_num0(display, x, y, color);
            break;
        case 1:
            draw_num1(display, x, y, color);
            break;
        case 2:
            draw_num2(display, x, y, color);
            break;
        case 3:
            draw_num3(display, x, y, color);
            break;
        case 4:
            draw_num4(display, x, y, color);
            break;
        case 5:
            draw_num5(display, x, y, color);
            break;
        case 6:
            draw_num6(display, x, y, color);
            break;
        case 7:
            draw_num7(display, x, y, color);
            break;
        case 8:
            draw_num8(display, x, y, color);
            break;
        case 9:
            draw_num9(display, x, y, color);
            break;
    }
    BEAUTIFUL_CLOCK_ANIMATION_DELAY = ANIMATION_DELAY;
}
