#include "global.h"

// Định nghĩa WiFi
const char *ssid = "Kingshouse-1B";
const char *password = "kingshouse2020";
const char* PARAM_MESSAGE = "message";
AsyncWebServer server(8080);


LiquidCrystal_I2C lcd(0x25, 16, 2);

String serverCode[CODE_AMOUNT] = {""};
int timercode[CODE_AMOUNT]= {0};
int device[CODE_AMOUNT] = {0}; 
int lcd_flag = LCD_INIT;
int lcd_timer = 0;
String code_from_keypad = "";

int countCode = 0;
String responseCode = "";

unsigned long previousMillis = 0;
unsigned long interval = 5000;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// varies for MPR121
uint16_t currTouched, lastTouched = 0;
Adafruit_MPR121 cap = Adafruit_MPR121();
int keycode, keycode1;
int keyin = 0;


// active_flag for 8 device
int device_flag[8] = {0};

String message = "";
String message1 = "";

float currentSample[9][100] = {0};
float curr[9] = {0};
int sample[9] = {0};
int deviceTimer[9];
int waitConnectTimer = 300;

ADS1115 ADS(0x48);
