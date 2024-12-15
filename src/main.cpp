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
// Varies for server and WiFi connect
AsyncWebServer server(80);
const char *ssid = "Cham Cafe";
const char *password = "Chamduocuocthay77";
const char* PARAM_MESSAGE = "message";

unsigned long previousMillis = 0;
unsigned long interval = 5000;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
int countCode = 0;
String serverCode[CODE_AMOUNT];
String responseCode;
int timercode[CODE_AMOUNT];
int device[CODE_AMOUNT];

LiquidCrystal_I2C lcd(0x25, 16, 2);

// varies for MPR121
uint16_t currTouched, lastTouched = 0;
Adafruit_MPR121 cap = Adafruit_MPR121();
int test;
String code_from_keypad = "";
int keycode, keycode1;
// 12 status for 12 state machine
int status1, status2, status3, status4, status5, status6, status7, status8, status9, status10, status11, status12 = 0;
int compare_status = COMPARE_INIT;
// active_flag for 8 device
int device_flag[8];

// keypad num
int keyin = 0;


// active flag for lcd
int lcd_flag = LCD_INIT;
int lcd_timer = 0;

String message;
String message1 = "";

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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("WiFi...");
    Serial.println("Connecting to WiFi...");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(WiFi.localIP());
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

int keyNum(int n){

  switch (n)
  {
    // case 0:
    //   return 99999999;
    //   break;
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
    // case 8:
    //   return 99999999;
    //   break;
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
int keyNum_xToi(String n){
  if(n == "0") return 0;
  if(n == "1") return 1;
  if(n == "2") return 2;
  if(n == "3") return 3;
  if(n == "4") return 4;
  if(n == "5") return 5;
  if(n == "6") return 6;
  if(n == "7") return 7;
  if(n == "8") return 8;
  if(n == "9") return 9;
  return 0;
}
int x = 0;
void keypad(){
      if(lcd_timer == 0) currTouched = cap.touched();
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
            if ((checkKey1 > 0) && (checkKey2 > 0) && (keyNum(i) != -1) && (lcd_timer == 0))
            {
                
                lcd.setCursor(x++, 1);
                lcd.print("*");

                if(x == 8) 
                { 
                  lcd.setCursor(0, 1);
                  lcd.print("********        ");
                  lcd_timer = 50;
                  x = 0;
                }
                Serial.print(keyNum(i)); 
                if (keyNum(i) >= 0 && keyNum(i) < 10)
                {
                  keycode = keycode*10 + keyNum(i);
                  checkKey1 = 0; checkKey2 = 0;
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
          // Serial.print("code from keypad: ");
          // Serial.println(code_from_keypad);
          keycode = 0;
      }
      lastTouched = currTouched;
    return;
}
int temp[8];
int stringToInt(String str) {
    for (int i = 0; i < str.length(); i++)
    {
      switch (str[i])
      {
      case '0':
         temp[i] = 0;
        break;
      case '1':
         temp[i] = 1;
        break;
      case '2':
         temp[i] = 2;
        break;
      case '3':
         temp[i] = 3;
        break;
      case '4':
         temp[i] = 4;
        break;
      case '5':
         temp[i] = 5;
        break;
      case '6':
         temp[i] = 6;
        break;
      case '7':
         temp[i] = 7;
        break;
      case '8':
         temp[i] = 8;
        break;
      case '9':
         temp[i] = 9;
        break;
      default:
        break;
      }
    }
  int number = 0;
  for (int i = 0; i < str.length(); i++)
  {
    number += temp[i] * pow(10, str.length()-1-i);
  }
  return number;
}

void compare(){

  if(!code_from_keypad.isEmpty())
  {
      for (int i = 0; i < CODE_AMOUNT; i++)
      {
            if (code_from_keypad == serverCode[i]) {
                Serial.println("TUYET");
                int x = device[i];
                Serial.println(x);
                device_flag[x] = 1;
                deviceTimer[x] = timercode[i];    // xoá servercode sau khi so sánh xong 
                lcd_flag = CORRECT_LCD;
                
                // lcd_timer = 100;
                message += "{";
                message += "\"data\": {";
                message += "\"id\": \"" + String(device[i]) + "\",";
                message += "\"code\": \"" + serverCode[i] + "\",";
                message += "\"totalTime\": \"" + String(timercode[i]) + "\",";
                message += "\"validTime\": \"" + String("FALSE") + "\"";
                message += "},";
                message += "\"message\": \"success\"";
                message += "}";
                server.on("/get/input", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                  request->send(200, "application/json", message);
                  message = "";
                }
                );
                serverCode[i] = "";
                timercode[i] = 0;
                device[i] = 0;
                break;
            }
            else if ((i == (CODE_AMOUNT - 1)) && (code_from_keypad != serverCode[i])) {
                lcd_flag = INCORRECT_LCD;
                // lcd_timer = 100;
            }
      }
      for(int i = 0; i < CODE_AMOUNT; i++)
      {
        if(!serverCode[i].isEmpty()) 
        {
          Serial.print("servercode "); Serial.print(i); Serial.print(" ");
          Serial.println(serverCode[i]);
        }
      }
      
      Serial.print("Keycode: ");
      Serial.println(code_from_keypad);
      code_from_keypad = "";
  }
}


void lcd_task(){
  switch (lcd_flag)
  {
    case LCD_INIT:
      lcd.setCursor(0, 0);
      lcd.print("Nhap code       ");
      
      break;

    case CORRECT_LCD:
      lcd.setCursor(0, 0);
      lcd.print("Hop le          ");

      if (lcd_timer <= 0) 
      {
        lcd.clear();
        lcd_flag = LCD_INIT;
      }
      else lcd_timer--;
      
      break;
    case INCORRECT_LCD:
      lcd.setCursor(0, 0);
      lcd.print("Khong hop le    ");
      if (lcd_timer <= 0) 
      {
        lcd.clear();
        lcd_flag = LCD_INIT;
      }
      else lcd_timer--;

      break;
    default:
      break;
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void server_func() {
  server.on("/abiquencwiue", HTTP_POST, [](AsyncWebServerRequest *request) {
    String response = "{";

    if (request->hasParam("code", true) && 
        request->hasParam("id", true) &&
        request->hasParam("totalTime", true) &&
        request->hasParam("validTime", true)
    ) {

        String id = request->getParam("id", true)->value();
        String code = request->getParam("code", true)->value();
        String totalTime = request->getParam("totalTime", true)->value();
        String valid = request->getParam("validTime", true)->value();

        if(valid == "TRUE")
        {
            serverCode[countCode] = code;
            timercode[countCode] = stringToInt(totalTime);
            
            Serial.println(stringToInt(totalTime));

            device[countCode] = keyNum_xToi(id);
            countCode++;
        }
        else if(valid == "FALSE" || lcd_flag == CORRECT_LCD)
        {
          for (int i = 0; i < CODE_AMOUNT; i++)
          {
            if (code == serverCode[i])
            {
              serverCode[i] = "";
              timercode[i] = 0;
              device[i] = 0;
            }
          }
        }

      
        if (countCode == CODE_AMOUNT) countCode = 0;

        response += "\"data\": {";
        response += "\"id\": \"" + id + "\",";
        response += "\"code\": \"" + code + "\",";
        response += "\"totalTime\": \"" + totalTime + "\",";
        response += "\"validTime\": \"" + valid + "\"";
        response += "},";
        response += "\"message\": \"success\"";
    } 
    else {
        response += "\"data\": null,";
        response += "\"message\": \"fail\"";
    }

    response += "}";

    request->send(200, "application/json", response);
  });

  server.begin();
}
void timer(){
  for (int i = 0; i < 9; i++)
  {
    if(deviceTimer[i] > 0) deviceTimer[i]--;
  }
  //if(lcd_timer > 0)lcd_timer--;
  
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

      float power = curr[1]/sample[1];
      String valid = "FALSE";
      // if(!SD.begin(SD_CS))
      // {
      //   Serial.println();
      //   appendFile(SD, "/sample.txt", num.c_str());
      //   readFile(SD, "/sample.txt");
      // }

      if(WiFi.status() == WL_CONNECTED){

        digitalWrite(DEVICE_1, 0);
        device_flag[1] = 0;
        Serial.println("Sending to server");
        message1 += "{";
        message1 += "\"ID\": \"" + String(1) + "\",";  
        message1 += "\"Current\": \"" + String(power) + "\","; 
        message1 += "\"message\": \"success\"";  
        message1 += "}";  

        server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
        {
          request->send(200, "application/json", message1);
          message1 = "";
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
void deviceProcess2(){
  if(device_flag[2]){
    if(deviceTimer[2] > 0){
      curr[2] += doc_ADS(2);
      sample[2]++;
      digitalWrite(DEVICE_2, 1);
      Serial.print("2 ");
      Serial.print(curr[2]);
      Serial.print(" ");
      Serial.println(sample[2]);
    }
    else {

      float power = curr[2]/sample[2]; 
      
      // if(!SD.begin(SD_CS))
      // {
      //   Serial.println();
      //   appendFile(SD, "/sample.txt", num.c_str());
      //   readFile(SD, "/sample.txt");
      // }

      if(WiFi.status() == WL_CONNECTED){

        digitalWrite(DEVICE_2, 0);
        device_flag[2] = 0;
        Serial.println("Sending to server");
        message1 += "{";
        message1 += "\"ID\": \"" + String(2) + "\",";  
        message1 += "\"Current\": \"" + String(power) + "\","; 
        message1 += "\"message\": \"success\"";  
        message1 += "}";  

        server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
        {
          request->send(200, "application/json", message1);
          message1 = "";
        }
        );
        
        curr[2] = 0;
        sample[2] = 0;
      }
      else {
        waitConnectTimer--;
        WiFi.reconnect();
      }
    }
  }
  server.begin();
}
void deviceProcess3(){
  if(device_flag[3]) Serial.println("device 3 opened");
}
void deviceProcess4(){
  if(device_flag[4]) Serial.println("device 4 opened");
}



Scheduler ts;

Task keypad_schedule            (1 * TASK_MILLISECOND, TASK_FOREVER, &keypad, &ts);
Task compare_schedule           (1 * TASK_MILLISECOND, TASK_FOREVER, &compare, &ts);
Task lcd_task_schedule          (1 * TASK_MILLISECOND, TASK_FOREVER, &lcd_task, &ts);
Task timer_schedule             (1000 * TASK_MILLISECOND, TASK_FOREVER, &timer, &ts);
Task device_process1_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess1, &ts);
Task device_process2_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess2, &ts);
Task device_process3_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess3, &ts);
Task device_process4_schedule   (1000 * TASK_MILLISECOND, TASK_FOREVER, &deviceProcess4, &ts);




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
  pinMode(DEVICE_1, OUTPUT);
  pinMode(DEVICE_2, OUTPUT);
  pinMode(DEVICE_3, OUTPUT);
  pinMode(DEVICE_4, OUTPUT);
  
  initWiFi();
  server_func();

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");  
  
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");


  Serial.println("keypad online");
  keypad_schedule.enable();

  Serial.println("process timer online");
  timer_schedule.enable();

  Serial.println("compare online");
  compare_schedule.enable();

  Serial.println("lcd online");
  lcd_task_schedule.enable();


  Serial.println("prc1 online ");
  device_process1_schedule.enable();
  Serial.println("prc2 online ");
  device_process2_schedule.enable();
  Serial.println("prc3 online ");
  device_process3_schedule.enable();
  Serial.println("prc4 online ");
  device_process4_schedule.enable();

  server.onNotFound(notFound);
}
void loop(){ 
    ts.execute();
}





// {
//   "data": {
//     "code": "12345679",
//     "id_machine": "1",
//     "time_start": "2024-12-13T10:00:00",
//     "time_end": "2024-12-13T12:00:00"
//   },
//   "message": "success"
// }





















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
































