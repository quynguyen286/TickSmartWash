#include "server.h"



void server_func() {
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request) {
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
        Serial.print("ID: ");
        Serial.println(id);
        Serial.print("Code: ");
        Serial.println(code);
        Serial.print("Total Time:");
        Serial.println(totalTime);
        Serial.print("Valid: ");
        Serial.println(valid);

        if(valid == "TRUE")
        {
            serverCode[countCode] = code;
            timercode[countCode] = stringToInt(totalTime);
            
            // Serial.println(stringToInt(totalTime));

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
