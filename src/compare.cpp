#include "compare.h"

void compare(){

  if(!code_from_keypad.isEmpty())
  {
      for (int i = 0; i < CODE_AMOUNT; i++)
      {
            if (code_from_keypad == serverCode[i]) {
                int x = device[i];
                Serial.print("Device ");
                Serial.print(x);
                Serial.println(" opened");
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
      

      code_from_keypad = "";
  }
}
