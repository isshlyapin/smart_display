#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <time.h>
#include <cmath>

#include "display.h"
#include "time_controller.h"
#include "wifi_controller.h"
#include "temperature_controller.h"

void draw_image_clock(MatrixPanel_I2S_DMA* display, bool init) {
    display->clearScreen();
    display->setTextSize(1);
    display->setCursor(8, 6);
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
    TemperatureController& temperatureController = TemperatureController::getInstance();
    display->clearScreen();
    display->setTextSize(1);
    display->setTextColor(display->color565(255, 255, 255));
    
    display->setCursor(2, 4);
    display->printf("Temp: %.1f", temperatureController.getTemperature());
    display->setCursor(2, 12);
    display->printf("Hum:  %.1f", temperatureController.getHumidity());
    display->setCursor(2, 20);
    display->printf("Heat: %.1f", temperatureController.getHeatIndex());
}


std::vector<std::pair<int, int>> positions = {
    {3,  9},
    {12, 9},
    {24, 9},
    {33, 9},
    {45, 9},
    {54, 9}
};

void transition_from0to1(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from1to2(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from2to3(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from3to4(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from4to5(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from5to6(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from6to7(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from7to8(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from8to9(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from9to0(MatrixPanel_I2S_DMA* display, int x, int y);

void transition_from2to0(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from3to0(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from4to0(MatrixPanel_I2S_DMA* display, int x, int y);
void transition_from5to0(MatrixPanel_I2S_DMA* display, int x, int y);


void draw_num(MatrixPanel_I2S_DMA* display, int num, int x, int y);
void draw_squares(MatrixPanel_I2S_DMA* display);

static int ANIMATION_DELAY = 50;

void draw_image_beautiful_clock(MatrixPanel_I2S_DMA* display, bool init) {
    static std::vector<int> numbers = {0, 0, 0, 0, 0, 0};
    
    if (init) {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time");
            return;
        } else {
            numbers[0] = timeinfo.tm_hour / 10;
            numbers[1] = timeinfo.tm_hour % 10;
            numbers[2] = timeinfo.tm_min / 10;
            numbers[3] = timeinfo.tm_min % 10;
            numbers[4] = timeinfo.tm_sec / 10;
            numbers[5] = timeinfo.tm_sec % 10;
            for (int i = 0; i < 6; ++i) {
                draw_num(display, numbers[i], positions[i].first, positions[i].second);
            }
            draw_squares(display);
        }
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    } else {
        std::vector<int> new_numbers = {
            timeinfo.tm_hour / 10, 
            timeinfo.tm_hour % 10, 
            timeinfo.tm_min / 10, 
            timeinfo.tm_min % 10, 
            timeinfo.tm_sec / 10, 
            timeinfo.tm_sec % 10
        };

        for (int i = 5; i >=0; --i) {
            if (new_numbers[i] == numbers[i]) {
                continue;
            } else if (new_numbers[i] == 1 && numbers[i] == 0) {
                transition_from0to1(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 2 && numbers[i] == 1) {
                transition_from1to2(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 3 && numbers[i] == 2) {
                transition_from2to3(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 4 && numbers[i] == 3) {
                transition_from3to4(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 5 && numbers[i] == 4) {
                transition_from4to5(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 6 && numbers[i] == 5) {
                transition_from5to6(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 7 && numbers[i] == 6) {
                transition_from6to7(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 8 && numbers[i] == 7) {
                transition_from7to8(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 9 && numbers[i] == 8) {
                transition_from8to9(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 9) {
                transition_from9to0(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 6) {
                transition_from6to7(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 5) {
                transition_from5to0(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 4) {
                transition_from4to0(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 3) {
                transition_from3to0(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 2) {
                transition_from2to0(display, positions[i].first, positions[i].second);
            } else if (new_numbers[i] == 0 && numbers[i] == 9) {
                transition_from9to0(display, positions[i].first, positions[i].second);
            } else {
                draw_num(display, new_numbers[i], positions[i].first, positions[i].second); 
            }
            numbers[i] = new_numbers[i];
        }
    }
}

void draw_squares(MatrixPanel_I2S_DMA* display) {
    display->drawRect(21, 13, 2, 2, display->color565(204,187,152));
    display->drawRect(21, 18, 2, 2, display->color565(204,187,152));

    display->drawRect(42, 13, 2, 2, display->color565(204,187,152));
    display->drawRect(42, 18, 2, 2, display->color565(204,187,152));
}

void transition_from0to1(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 7; ++i) {
        display->drawPixel(x+i+1, y, display->color565(0,0,0));
        display->drawPixel(x+i+1, y+14, display->color565(0,0,0));
        
        display->drawLine(x+i,   y,   x+i,   y+13, display->color565(0,0,0));
        display->drawLine(x+i+1, y+1, x+i+1, y+6,  display->color565(204,187,152));
        display->drawLine(x+i+1, y+8, x+i+1, y+13,  display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from1to2(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 6; i >= 0; --i) {
        if (i > 0) {
            display->drawPixel(x+i, y,    display->color565(204,187,152));
            display->drawPixel(x+i, y+7,  display->color565(204,187,152));
            display->drawPixel(x+i, y+14, display->color565(204,187,152));
        }
        
        display->drawLine(x+i+1, y+8, x+i+1, y+13,  display->color565(0,0,0));
        display->drawLine(x+i, y+8, x+i, y+13,  display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from2to3(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 7; ++i) {
        display->drawLine(x+i, y+8, x+i, y+13, display->color565(0,0,0));
        display->drawLine(x+i+1, y+8, x+i+1, y+13, display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from3to4(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+1+i, y+14, display->color565(0,0,0));

        display->drawPixel(x+6-i, y, display->color565(0,0,0));
        display->drawPixel(x, y+1+i, display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from4to5(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+14, display->color565(204,187,152));
        
        display->drawPixel(x+7, y+6-i, display->color565(0,0,0));
        display->drawPixel(x+6-i, y, display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from5to6(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 7; ++i) {
        display->drawLine(x+6-i, y+8, x+6-i, y+13, display->color565(204,187,152));
        if (i > 0) {
            display->drawLine(x+7-i, y+8, x+7-i, y+13, display->color565(0,0,0));
        }
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from6to7(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 7; ++i) {
        display->drawPixel(x+i, y+7, display->color565(0,0,0));
        display->drawPixel(x+i, y+14, display->color565(0,0,0));

        display->drawLine(x+i, y+1, x+i, y+6, display->color565(0,0,0));
        display->drawLine(x+i, y+8, x+i, y+13, display->color565(0,0,0));

        display->drawLine(x+1+i, y+1, x+1+i, y+6, display->color565(204,187,152));
        display->drawLine(x+1+i, y+8, x+1+i, y+13, display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from7to8(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {        
        display->drawPixel(x+6-i, y+7, display->color565(204,187,152));
        display->drawPixel(x+6-i, y+14, display->color565(204,187,152));

        display->drawLine(x+6-i, y+1, x+6-i, y+6, display->color565(204,187,152));
        display->drawLine(x+6-i, y+8, x+6-i, y+13, display->color565(204,187,152));
        
        if (i > 0) {
            display->drawLine(x+7-i, y+1, x+7-i, y+6, display->color565(0,0,0));
            display->drawLine(x+7-i, y+8, x+7-i, y+13, display->color565(0,0,0));
        }
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
    display->drawLine(x, y+1, x, y+6, display->color565(204,187,152));
    display->drawLine(x, y+8, x, y+13, display->color565(204,187,152));
    display->drawLine(x+1, y+1, x+1, y+6, display->color565(0,0,0));
    display->drawLine(x+1, y+8, x+1, y+13, display->color565(0,0,0));
    vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
}

void transition_from8to9(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 7; ++i) {
        display->drawLine(x+i, y+8, x+i, y+13, display->color565(0,0,0));
        display->drawLine(x+i+1, y+8, x+i+1, y+13, display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from9to0(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x, y+8+i, display->color565(204,187,152));    
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from2to0(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+1+i, y+7, display->color565(0,0,0));
        display->drawPixel(x+7, y+8+i, display->color565(204,187,152));

        display->drawLine(x+6-i, y+1, x+6-i, y+6, display->color565(204,187,152));

        if (i > 0) {
            display->drawLine(x+7-i, y+1, x+7-i, y+6, display->color565(0,0,0));
        }
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
    display->drawLine(x, y+1, x, y+6, display->color565(204,187,152));
    display->drawLine(x+1, y+1, x+1, y+6, display->color565(0,0,0));
    vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
}

void transition_from4to0(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+1+i, y, display->color565(204,187,152));

        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x, y+8+i, display->color565(204,187,152));

        display->drawPixel(x+6-i, y+14, display->color565(204,187,152));

        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from3to0(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x, y+6-i, display->color565(204,187,152));
        display->drawPixel(x, y+8+i, display->color565(204,187,152));

        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void transition_from5to0(MatrixPanel_I2S_DMA* display, int x, int y) {
    for (int i = 0; i < 6; ++i) {
        display->drawPixel(x+6-i, y+7, display->color565(0,0,0));
        display->drawPixel(x+1+i, y+7, display->color565(0,0,0));

        display->drawPixel(x, y+8+i, display->color565(204,187,152));
        display->drawPixel(x+7, y+6-i, display->color565(204,187,152));
        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }
}

void draw_num0(MatrixPanel_I2S_DMA* display, int x, int y) {
    display->drawLine(x+1, y, x+6, y, display->color565(204,187,152));
    display->drawLine(x+1, y+14, x+6, y+14, display->color565(204,187,152));
    
    display->drawLine(x, y+1, x, y+6, display->color565(204,187,152));
    display->drawLine(x, y+8, x, y+13, display->color565(204,187,152));

    display->drawLine(x+7, y+1, x+7, y+6, display->color565(204,187,152));
    display->drawLine(x+7, y+8, x+7, y+13, display->color565(204,187,152));
}

void draw_num1(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num0(display, x, y);
    transition_from0to1(display, x, y);
}



void draw_num2(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num1(display, x, y);
    transition_from1to2(display, x, y);
}

void draw_num3(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num2(display, x, y);
    transition_from2to3(display, x, y);
}

void draw_num4(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num3(display, x, y);
    transition_from3to4(display, x, y);
}

void draw_num5(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num4(display, x, y);
    transition_from4to5(display, x, y);
}

void draw_num6(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num5(display, x, y);
    transition_from5to6(display, x, y);
}

void draw_num7(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num6(display, x, y);
    transition_from6to7(display, x, y);
}

void draw_num8(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num7(display, x, y);
    transition_from7to8(display, x, y);
}

void draw_num9(MatrixPanel_I2S_DMA* display, int x, int y) {
    draw_num8(display, x, y);
    transition_from8to9(display, x, y);
}

void draw_num(MatrixPanel_I2S_DMA* display, int num, int x, int y) {
    int old_ANIMATION_DELAY = ANIMATION_DELAY;
    ANIMATION_DELAY = 5;
    switch (num) {
        case 0:
            draw_num0(display, x, y);
            break;
        case 1:
            draw_num1(display, x, y);
            break;
        case 2:
            draw_num2(display, x, y);
            break;
        case 3:
            draw_num3(display, x, y);
            break;
        case 4:
            draw_num4(display, x, y);
            break;
        case 5:
            draw_num5(display, x, y);
            break;
        case 6:
            draw_num6(display, x, y);
            break;
        case 7:
            draw_num7(display, x, y);
            break;
        case 8:
            draw_num8(display, x, y);
            break;
        case 9:
            draw_num9(display, x, y);
            break;
    }
    ANIMATION_DELAY = old_ANIMATION_DELAY;
}
