#include "mpr121.h"
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
          Serial.print("Code from keypad: ");
          Serial.println(code_from_keypad);
          keycode = 0;
      }
      lastTouched = currTouched;
    return;
}
