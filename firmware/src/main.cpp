#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include "config.h"
#include "wifi_controller.h"
#include "bluetooth_controller.h"
#include "temperature_controller.h"
#include "time_controller.h"
#include "display.h"

DisplayController& displayController = DisplayController::getInstance();
MatrixPanel_I2S_DMA *display = nullptr;

// Обновление яркости дисплея
void updateBrightness() {
    static int lastBrightness = -1;
    int currentBrightness = displayController.getBrightness();
    
    if (lastBrightness != currentBrightness) {
        display->setBrightness8(currentBrightness);
        lastBrightness = currentBrightness;
    }
}

// Обновление отображаемого изображения
void updateImage() {
    static ImageType lastImage = ImageType::NOT_IMAGE;
    ImageType currentImage = displayController.getImage();
    
    if (lastImage != currentImage) {
        Serial.println("Changing image");
        Serial.printf("Current image: %d\n", currentImage);
        
        display->clearScreen();
        image_draws[currentImage](display, true);
        lastImage = currentImage;
    }
    
    image_draws[currentImage](display, false);
}

void setup() {
    Serial.begin(115200);
    
    // Инициализация дисплея
    displayController.begin();
    display = displayController.getDisplay();
    display->setBrightness8(displayController.getBrightness());

    // Создание задач для Bluetooth и WiFi
    xTaskCreate(bluetoothSetup, "bluetoothSetup", TASK_STACK_SIZE, nullptr, 1, nullptr);
    xTaskCreate(wifiSetup, "wifiSetup", TASK_STACK_SIZE, nullptr, 1, nullptr);

    // Ожидание подключения к WiFi
    while (!isWifiConnected()) {
        vTaskDelay(WIFI_CHECK_DELAY / portTICK_PERIOD_MS);
    }

    timeSetup();
}

void loop() {
    Serial.printf("Free heap loop: %d\n", ESP.getFreeHeap());
    updateBrightness();
    updateImage();
    vTaskDelay(MAIN_LOOP_DELAY / portTICK_PERIOD_MS);
}
