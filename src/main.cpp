#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "sd_card.h"

#include "ADS1X15.h"
#include "i2c_scanner.h"
#include "ads1115_16bit.h"
#include "IO_expansion.h"
#include <TaskScheduler.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_BusIO_Register.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define CODE_TIMER 600
#define DEVICE_TIMER 600
#define SMALLEST_CODE 10000000
#define GET_DEVICE_NUM 10000000
#define CODE_AMOUNT 10

#define INIT 1
#define CORRECT_LCD 2
#define INCORRECT_LCD 3
#define PRINT_STAR 4

#define COMPARE_SET 5

#define DEVICE_1 13
#define DEVICE_2 14
#define DEVICE_3 15
#define DEVICE_4 16
#define DEVICE_5 17
#define DEVICE_6 18
#define DEVICE_7 19
#define DEVICE_8 23
#define SD_CS 5
// Varies for server and WiFi connect
AsyncWebServer server(80);
const char *ssid = "AN COFFEE TRET";
const char *password = "Ancoffeexinchao";
const char* PARAM_MESSAGE = "message";

unsigned long previousMillis = 0;
unsigned long interval = 5000;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
int countCode = 0;
String serverCode[CODE_AMOUNT];
String responseCode;
int servercode[CODE_AMOUNT];
int timercode[CODE_AMOUNT];
int st = 0;

LiquidCrystal_I2C lcd(0x25, 16, 2);

// varies for MPR121
uint16_t currTouched, lastTouched = 0;
Adafruit_MPR121 cap = Adafruit_MPR121();
int test;
String code_from_keypad = "";
int keycode, keycode1;
// 12 status for 12 state machine
int status1, status2, status3, status4, status5, status6, status7, status8, status9, status10, status11, status12 = 0;
int compare_status;
// active_flag for 8 device
int device_flag[8];

// active flag for lcd
int lcd_flag;
int lcd_timer;

String device1Send = "11111111";
String device2Send = "22222222";
String device3Send = "33333333";
String device4Send = "44444444";
String device5Send = "55555555";
String device6Send = "66666666";
String device7Send = "77777777";
String device8Send = "88888888";
String message1 = "{\"status\":\"OK\",\"message\":\"" + device1Send + "\"}";
String message2 = "{\"status\":\"OK\",\"message\":\"" + device2Send + "\"}";
String message3 = "{\"status\":\"OK\",\"message\":\"" + device3Send + "\"}";
String message4 = "{\"status\":\"OK\",\"message\":\"" + device4Send + "\"}";
String message5 = "{\"status\":\"OK\",\"message\":\"" + device5Send + "\"}";
String message6 = "{\"status\":\"OK\",\"message\":\"" + device6Send + "\"}";
String message7 = "{\"status\":\"OK\",\"message\":\"" + device7Send + "\"}";
String message8 = "{\"status\":\"OK\",\"message\":\"" + device8Send + "\"}";

float currentSample[9][100];
float curr[9] = {0,0,0,0,0,0,0,0,0};
int sample[9] = {0,0,0,0,0,0,0,0,0};
int deviceTimer[9];
int waitConnectTimer = 300;
// Hàm ghi file mới
void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Hàm đọc file
void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);
  File file = fs.open(path, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File content:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

// Hàm ghi thêm dữ liệu vào file
void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

// Hàm kết nối Wi-Fi
void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}
int keyNum(int n){
  switch (n)
  {
    case 0:
      return 99999999;
      break;
    case 1:
      return 7;
      break;
    case 2:
      return 4;
      break;
    case 3:
      return 1;
      break;
    case 4:
      return 0;
      break;
    case 5:
      return 8;
      break;
    case 6:
      return 5;
      break;
    case 7:
      return 2;
      break;
    case 8:
      return 99999999;
      break;
    case 9:
      return 9;
      break;
    case 10:
      return 6;
      break;
    case 11:
      return 3;
      break;
    default:
      break;
  }
  return -1;
}
String keyNum_iTos(int n){
  switch (n)
  {
    case 0:
      return "0";
      break;
    case 1:
      return "1";
      break;
    case 2:
      return "2";
      break;
    case 3:
      return "3";
      break;
    case 4:
      return "4";
      break;
    case 5:
      return "5";
      break;
    case 6:
      return "6";
      break;
    case 7:
      return "7";
      break;
    case 8:
      return "8";
      break;
    case 9:
      return "9";
      break;
    default:
      break;
  }
  return "";
}
int keyNum_sToi(const char n){
  if(n == '0') return 0;
  if(n == '1') return 1;
  if(n == '2') return 2;
  if(n == '3') return 3;
  if(n == '4') return 4;
  if(n == '5') return 5;
  if(n == '6') return 6;
  if(n == '7') return 7;
  if(n == '8') return 8;
  if(n == '9') return 9;
  return 0;
}
void keypad(){
    currTouched = cap.touched();
    if (keycode < SMALLEST_CODE){
        for (uint8_t i=0; i<12; i++) 
        {
          int checkKey1, checkKey2 = 0;
            // it if *is* touched and *wasnt* touched before, alert!
            if ((currTouched & _BV(i)) && !(lastTouched & _BV(i)) ) {
                checkKey1++;
            }
            // if it *was* touched and now *isnt*, alert!
            if (!(currTouched & _BV(i)) && (lastTouched & _BV(i)) ) {
                checkKey2++;
            }
            if ((checkKey1 >= 1) && (checkKey2 == 1))
            {
                lcd.clear();
                Serial.print(keyNum(i)); 
                if (keyNum(i) >= 0 && keyNum(i) < 10)
                {
                  keycode = keycode*10 + keyNum(i);
                  checkKey1 = 0; checkKey2 = 0;
                }
                else if (keyNum(i) == 10)
                {
                  checkKey1 = 0; checkKey2 = 0;
                  lcd_flag = 2;
                  break;
                }
            }
        }
    }
    else {
        Serial.println();
        keycode1 = keycode;
        for(int i = 1; i < 9; i++)
        {
          code_from_keypad = keyNum_iTos(keycode%10) + code_from_keypad;
          keycode = keycode/10;
        }
        keycode = 0;
    }
    lastTouched = currTouched;

    return;
}
void compare(){
  switch (compare_status)
  {
  case INIT:
    compare_status = COMPARE_SET;
    break;
  case COMPARE_SET:
    int x = keycode1/GET_DEVICE_NUM;
    for (int i = 0; i < CODE_AMOUNT; i++)
    {
      if(code_from_keypad == serverCode[i] && code_from_keypad != "")
      {
        Serial.println(x);
        device_flag[x] = 1;
        deviceTimer[x] = DEVICE_TIMER;
        lcd_flag = CORRECT_LCD;
        lcd_timer = 1;
        code_from_keypad = "";          
        serverCode[i] = "";
      }
      else if (code_from_keypad != serverCode[i] && code_from_keypad != "" && i == (CODE_AMOUNT - 1))
      {
        code_from_keypad = "";
        lcd_flag = INCORRECT_LCD;
        lcd_timer = 1;
      }
    }
    break;
  default:
    break;
  }
}


void lcd_task(){
  int x = 0;
  switch (lcd_flag)
  {
    case INIT:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Nhap code");
      break;
    case PRINT_STAR:
      lcd.setCursor(1, x++);
      lcd.print("*");
      break;
    case CORRECT_LCD:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Hop le");
      if (lcd_timer == 0) lcd_flag = 0;
      break;
    case INCORRECT_LCD:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Khong hop le");
      if (lcd_timer == 0) lcd_flag = 0;
      break;
    default:
      break;
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void server_func(){
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("body", true)) {
        String body = request->getParam("body", true)->value();
        serverCode[countCode] = body;
        timercode[countCode] = CODE_TIMER;
        responseCode = body;
        countCode++;
        if(countCode == CODE_AMOUNT) countCode = 0;
        Serial.print("RCV:");
        Serial.println(body);
    }
    String response = "{\"code\":[";
    for (int i = 0; i < 8; i++) {
        response += responseCode[i];
        if (i < 7) response += ",";
    }
    response += "]}";

    request->send(200, "application/json", response);
  });
  

  server.begin();
}
void timer(){
  for (int i = 0; i < CODE_AMOUNT; i++){
    if(timercode[i] == 0) serverCode[i] = "";
    else timercode[i]--;
    if(lcd_timer > 0) lcd_timer--;
  }
  for (int i = 0; i < 9; i++)
  {
    if(deviceTimer[i] > 0) deviceTimer[i]--;
  }
}


void deviceProcess1(){
  if(device_flag[1]){
    if(deviceTimer[1] > 0){
      curr[1] += doc_ADS(1);
      sample[1]++;
      digitalWrite(DEVICE_1, 1);
      Serial.print("1 ");
      Serial.print(curr[1]);
      Serial.print(" ");
      Serial.println(sample[1]);
    }
    else {
      float power = (curr[1]/sample[1])*1000;
      
      String num = "1 " + String(power) + "\n";

      // if(!SD.begin(SD_CS))
      // {
      //   Serial.println();
      //   appendFile(SD, "/sample.txt", num.c_str());
      //   readFile(SD, "/sample.txt");
      // }

      if(WiFi.status() == WL_CONNECTED){

        message1 = "{\"status\":\"OK\",\"message\":\"" + num + "\"}";
        digitalWrite(DEVICE_1, 0);
        device_flag[1] = 0;
        Serial.println("Sending to server");
        // API trả về thông tin cơ bản
        server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
        {
          request->send(200, "application/json", message1);
        }
        );

        curr[1] = 0;
        sample[1] = 0;
      }
      else {
        waitConnectTimer--;
        WiFi.reconnect();
      }
    }
  }
  server.begin();
}
void deviceProcess2(){}
void deviceProcess3(){}
void deviceProcess4(){}
void deviceProcess5(){}
void deviceProcess6(){}
void deviceProcess7(){}
void deviceProcess8(){}
Scheduler ts;

Task keypad_schedule            (1 * TASK_MILLISECOND, TASK_FOREVER, &keypad, &ts);
Task compare_schedule           (1 * TASK_MILLISECOND, TASK_FOREVER, &compare, &ts);
Task lcd_task_schedule          (1000 * TASK_MILLISECOND, TASK_FOREVER, &lcd_task, &ts);
Task timer_schedule             (1000 * TASK_MILLISECOND, TASK_FOREVER, &timer, &ts);
Task device_process1_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess1, &ts);
Task device_process2_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess2, &ts);
Task device_process3_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess3, &ts);
Task device_process4_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess4, &ts);
Task device_process5_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess5, &ts);
Task device_process6_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess6, &ts);
Task device_process7_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess7, &ts);
Task device_process8_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess8, &ts);



void setup(){ 
  Serial.begin(9600);
  Serial.println("Scheduling Options: setup()");
  lcd.init();           
  lcd.backlight();
  Wire.begin();
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);
  ADS.begin();
  ADS1.begin();
  pinMode(DEVICE_1, OUTPUT);
  pinMode(DEVICE_2, OUTPUT);
  pinMode(DEVICE_3, OUTPUT);
  pinMode(DEVICE_4, OUTPUT);
  pinMode(DEVICE_5, OUTPUT);
  pinMode(DEVICE_6, OUTPUT);
  pinMode(DEVICE_7, OUTPUT);
  pinMode(DEVICE_8, OUTPUT);
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");  

  initWiFi();

  server_func();
  
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");


  Serial.println("keypad start");
  keypad_schedule.enable();

  Serial.println("process timer start");
  timer_schedule.enable();

  Serial.println("compare start");
  compare_schedule.enable();

  Serial.println("lcd start");
  lcd_task_schedule.enable();


  Serial.println("prc1 start ");
  device_process1_schedule.enable();
  Serial.println("prc2 start ");
  device_process2_schedule.enable();
  Serial.println("prc3 start ");
  device_process3_schedule.enable();
  Serial.println("prc4 start ");
  device_process4_schedule.enable();
  Serial.println("prc5 start ");
  device_process5_schedule.enable();
  Serial.println("prc6 start ");
  device_process6_schedule.enable();
  Serial.println("prc7 start ");
  device_process7_schedule.enable();
  Serial.println("prc8 start");
  device_process8_schedule.enable();

  server.onNotFound(notFound);
}
void loop(){ 
    ts.execute();
}






  // // Tạo file và ghi dữ liệu
  // writeFile(SD, "/sample.txt", "this is a file for storing samples\n");

  // // Đọc nội dung file
  // readFile(SD, "/example.txt");

  // // Ghi thêm dữ liệu vào file
  // appendFile(SD, "/example.txt", "This is a second line.\n");

  // // Đọc lại file để kiểm tra
  // readFile(SD, "/example.txt");



// // i2c scanner code

// void setup()
// {
//   Wire.begin();
//   Serial.begin(9600);
//   Serial.println("\nI2C Scanner");
// }

// void loop()
// {
//   byte error, address;
//   int nDevices;
//   Serial.println("Scanning...");
//   nDevices = 0;
//   for (address = 1; address < 127; address++)
//   {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address < 16)
//       {
//         Serial.print("0");
//       }
//       Serial.println(address, HEX);
//       nDevices++;
//     }
//     else if (error == 4)
//     {
//       Serial.print("Unknow error at address 0x");
//       if (address < 16)
//       {
//         Serial.print("0");
//       }
//       Serial.println(address, HEX);
//     }
//   }
//   if (nDevices == 0)
//   {
//     Serial.println("No I2C devices found\n");
//   }
//   else
//   {
//     Serial.println("done\n");
//   }
//   delay(700);
// }







































