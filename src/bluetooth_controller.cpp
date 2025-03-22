#include <BluetoothSerial.h>
#include <ArduinoNvs.h>
#include <ArduinoJson.h>
#include <string.h>

#include "config.h"
#include "display.h"
#include "bluetooth_controller.h"
#include "time_controller.h"

static void wifiMsgHandler(JsonDocument& jsonMsg);
static void timeMsgHandler(JsonDocument& jsonMsg);
static void brightnessMsgHandler(JsonDocument& jsonMsg);
static void imageMsgHandler(JsonDocument& jsonMsg);

static void wifiMsgHandler(JsonDocument& jsonMsg) {
    const char* ssid = jsonMsg["ssid"];
    const char* password = jsonMsg["password"];
    if (ssid && password) {
        if (NVS.begin()) {
            Serial.println("wifiMsgHandler: NVS opened");
            NVS.setString("wifi_ssid", ssid);
            NVS.setString("wifi_password", password);
            NVS.close();
        } else {
            Serial.println("wifiMsgHandler: Error opening NVS");
        }
    }
}

static void timeMsgHandler(JsonDocument& jsonMsg) {
    const char* timezone = jsonMsg["timezone"];
    if (timezone) {
        if (NVS.begin()) {
            Serial.println("timeMsgHandler: NVS opened");
            NVS.setString("timezone", timezone);
            NVS.close();
        } else {
            Serial.println("timeMsgHandler: Error opening NVS");
        }
        configTime(atoi(timezone) * 3600, 0, "pool.ntp.org", "time.nist.gov");
    }
}

static void brightnessMsgHandler(JsonDocument& jsonMsg) {
    const char* value = jsonMsg["value"];
    if (value) {
        if (NVS.begin()) {
            Serial.println("brightnessMsgHandler: NVS opened");
            NVS.setInt("brightness", atoi(value));
            NVS.close();
        } else {
            Serial.println("brightnessMsgHandler: Error opening NVS");
        }
        DisplayController& displayController = DisplayController::getInstance();
        displayController.setBrightness(atoi(value));
    }
}

static void imageMsgHandler(JsonDocument& jsonMsg) {
    const char* tmp = jsonMsg["value"];
    if (tmp) {
        int value = atoi(tmp);
        DisplayController& displayController = DisplayController::getInstance();
        if (value == ImageType::CLOCK_IMAGE) {
            displayController.setImage(ImageType::CLOCK_IMAGE);
        } else if (value == ImageType::DHT_IMAGE) {
            displayController.setImage(ImageType::DHT_IMAGE);
        } else if (value == ImageType::BEAUTIFUL_CLOCK_IMAGE) {
            displayController.setImage(ImageType::BEAUTIFUL_CLOCK_IMAGE);
        }
    }
}

void bluetoothSetup(void* pvParameters) {
    BluetoothSerial SerialBT;
    SerialBT.begin(BT_NAME);
    while (1) {
        if (SerialBT.available()) {
            Serial.println("Bluetooth message");
            String strMsg = SerialBT.readString();
            JsonDocument jsonMsg;
            if (!deserializeJson(jsonMsg, strMsg)) {
                const char* type = jsonMsg["type"];
                if (type) {
                    if (strcmp(type, TYPE_MSG_WIFI) == 0) {
                        Serial.println("Wifi message");
                        wifiMsgHandler(jsonMsg);
                    } else if (strcmp(type, TYPE_MSG_TIME) == 0) {
                        Serial.println("Time message");
                        timeMsgHandler(jsonMsg);
                    } else if (strcmp(type, TYPE_MSG_BRIGHTNESS) == 0) {
                        Serial.println("Brightness message");
                        brightnessMsgHandler(jsonMsg);
                    } else if (strcmp(type, TYPE_MSG_IMAGE) == 0) {
                        Serial.println("Image message");
                        imageMsgHandler(jsonMsg);
                    }
                }   
            }
            strMsg.clear();
            jsonMsg.clear();
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
