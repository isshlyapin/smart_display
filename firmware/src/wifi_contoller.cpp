#include <Arduino.h>
#include <ArduinoNvs.h>
#include <WiFi.h>

#include "wifi_controller.h"
#include "display.h"

bool isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void wifiSetup(void* pvParameters) {
    Serial.println("WiFi setup task started");
    while (1) {
        if (!isWifiConnected()) {
            Serial.println("Wifi connection attempt ...");
            NVS.begin();
            String ssid = NVS.getString("wifi_ssid", "");
            String password = NVS.getString("wifi_password", "");
            
            if (ssid.length() && password.length()) {
                WiFi.mode(WIFI_STA);
                WiFi.begin(ssid, password);
                
                for (int i = 0; i < 5; ++i) {
                    if (isWifiConnected()) {
                        break;
                    }
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                }
            }
            ssid.clear();
            password.clear();
            NVS.close();        
        }
        else {
            Serial.println("WiFi connected");
            vTaskDelete(NULL);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
