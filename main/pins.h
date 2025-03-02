#pragma once

#include <stdint.h>

#define WIFI_LoRa_32_V3 true
#define DISPLAY_HEIGHT  64
#define DISPLAY_WIDTH   128

#define USB_VID 0x303a
#define USB_PID 0x1001
#define I2C_HW_ADDR 0x3C

static const uint8_t VATS_LED_BUILTIN = 35;

static const uint8_t VATS_TX = 43;
static const uint8_t VATS_RX = 44;

static const uint8_t VATS_SDA = 41;
static const uint8_t VATS_SCL = 42;

static const uint8_t VATS_SS = 8;
static const uint8_t VATS_MOSI = 10;
static const uint8_t VATS_MISO = 11;
static const uint8_t VATS_SCK = 9;

static const uint8_t VATS_A0 = 1;
static const uint8_t VATS_A1 = 2;
static const uint8_t VATS_A2 = 3;
static const uint8_t VATS_A3 = 4;
static const uint8_t VATS_A4 = 5;
static const uint8_t VATS_A5 = 6;
static const uint8_t VATS_A6 = 7;
static const uint8_t VATS_A7 = 8;
static const uint8_t VATS_A8 = 9;
static const uint8_t VATS_A9 = 10;
static const uint8_t VATS_A10 = 11;
static const uint8_t VATS_A11 = 12;
static const uint8_t VATS_A12 = 13;
static const uint8_t VATS_A13 = 14;
static const uint8_t VATS_A14 = 15;
static const uint8_t VATS_A15 = 16;
static const uint8_t VATS_A16 = 17;
static const uint8_t VATS_A17 = 18;
static const uint8_t VATS_A18 = 19;
static const uint8_t VATS_A19 = 20;

static const uint8_t VATS_T1 = 1;
static const uint8_t VATS_T2 = 2;
static const uint8_t VATS_T3 = 3;
static const uint8_t VATS_T4 = 4;
static const uint8_t VATS_T5 = 5;
static const uint8_t VATS_T6 = 6;
static const uint8_t VATS_T7 = 7;
static const uint8_t VATS_T8 = 8;
static const uint8_t VATS_T9 = 9;
static const uint8_t VATS_T10 = 10;
static const uint8_t VATS_T11 = 11;
static const uint8_t VATS_T12 = 12;
static const uint8_t VATS_T13 = 13;
static const uint8_t VATS_T14 = 14;

static const uint8_t Vext = 36;
static const uint8_t LED = 35;
static const uint8_t VATS_RST_OLED = 21;
static const uint8_t VATS_SCL_OLED = 18;
static const uint8_t VATS_SDA_OLED = 17;

static const uint8_t RST_LoRa = 12;
static const uint8_t BUSY_LoRa = 13;
static const uint8_t DIO0 = 14;
