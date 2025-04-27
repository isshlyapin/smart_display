#pragma once

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define R1_PIN 22
#define G1_PIN 23
#define B1_PIN 21
#define R2_PIN 19
#define G2_PIN 18
#define B2_PIN 26
#define A_PIN 25
#define B_PIN 17
#define C_PIN 33
#define D_PIN 16
#define E_PIN -1 // required for 1/32 scan panels, like 64x64px. Any available pin would do, i.e. IO32
#define LAT_PIN 4
#define OE_PIN 14
#define CLK_PIN 32

static HUB75_I2S_CFG::i2s_pins my_pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};