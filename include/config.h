#pragma once

#include <DHT.h>

#define DHT_PIN 21
#define DHT_TYPE DHT22

static String BT_NAME = "Smart Display SIUP";

static const char* TYPE_MSG_WIFI       = "wifi";
static const char* TYPE_MSG_BRIGHTNESS = "brightness";
static const char* TYPE_MSG_IMAGE      = "image";
static const char* TYPE_MSG_TIME       = "time";
static const char* TYPE_MSG_COLOR      = "color";

// Константы для временных интервалов (в миллисекундах)
constexpr static uint32_t WIFI_CHECK_DELAY      = 500;
constexpr static uint32_t BLUETOOTH_CHECK_DELAY = 250;

constexpr static uint32_t MAIN_LOOP_DELAY = 50;

constexpr static uint32_t TASK_STACK_SIZE = 4096;

constexpr static uint32_t ANIMATION_DELAY_QUICK = 5;
constexpr static uint32_t ANIMATION_DELAY       = 40;

//Цвет по умолчанию
static const int DEFAULT_COLOR_R = 204;
static const int DEFAULT_COLOR_G = 187;
static const int DEFAULT_COLOR_B = 152;
