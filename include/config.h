#pragma once

#include <DHT.h>

#define DHT_PIN 21
#define DHT_TYPE DHT22

static String BT_NAME = "Smart Display SIUP";

static const char* TYPE_MSG_WIFI       = "wifi";
static const char* TYPE_MSG_BRIGHTNESS = "brightness";
static const char* TYPE_MSG_IMAGE      = "image";
static const char* TYPE_MSG_TIME       = "time";

// Константы для временных интервалов (в миллисекундах)
constexpr uint32_t WIFI_CHECK_DELAY = 500;
constexpr uint32_t MAIN_LOOP_DELAY = 100;
constexpr uint32_t TASK_STACK_SIZE = 4096;