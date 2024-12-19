#include "global.h"
#include <TaskScheduler.h>



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
  server.begin();
}
void loop(){ 
    ts.execute();
}
























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
































