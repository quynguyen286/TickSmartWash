// global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
//  #include <WiFiClientSecure.h>
// #include <WebSockets.h>
// #include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include "str_prc.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "sd_card.h"
#include "wifi_connect.h"
#include "mpr121.h"
#include "lcd.h"
#include "compare.h"
#include "dvc_prc.h"
#include "server.h"
#include "ADS1X15.h"
// #include "i2c_scanner.h"
#include "ads1115_16bit.h"
// #include "IO_expansion.h"
#include <Adafruit_MPR121.h>
#include <Adafruit_BusIO_Register.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Định nghĩa hằng số
#define CODE_TIMER 600
#define DEVICE_TIMER 600
#define SMALLEST_CODE 10000000
#define GET_DEVICE_NUM 10000000
#define CODE_AMOUNT 100

#define LCD_INIT 900
#define CORRECT_LCD 901
#define INCORRECT_LCD 902
#define LCD_CONNECTING_WIFI 904


#define COMPARE_INIT 200
#define COMPARE_SET 201

#define DEVICE_1 13
#define DEVICE_2 14
#define DEVICE_3 15
#define DEVICE_4 16
#define SD_CS 5

extern const char *ssid;
extern const char *password;
extern const char* PARAM_MESSAGE;
extern AsyncWebServer server;

// Biến toàn cục
extern LiquidCrystal_I2C lcd;

extern String serverCode[CODE_AMOUNT];
extern int timercode[CODE_AMOUNT];
extern int device[CODE_AMOUNT];
extern int lcd_flag;
extern int lcd_timer;
extern String code_from_keypad;

extern int countCode;
extern String responseCode;

extern unsigned long previousMillis;
extern unsigned long interval;
extern unsigned long lastTime;
extern unsigned long timerDelay;

// varies for MPR121
extern uint16_t currTouched, lastTouched;
extern Adafruit_MPR121 cap;
extern int keycode, keycode1;
extern int keyin;

// active_flag for 8 device
extern int device_flag[8];

extern ADS1115 ADS;

extern String message;
extern String message1;

extern float currentSample[9][100];
extern float curr[9];
extern int sample[9];
extern int deviceTimer[9];
extern int waitConnectTimer;



#endif