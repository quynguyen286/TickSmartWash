#include "dvc_prc.h"

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
void timer(){
  for (int i = 0; i < 9; i++)
  {
    if(deviceTimer[i] > 0) deviceTimer[i]--;
  }
  //if(lcd_timer > 0)lcd_timer--;
}
