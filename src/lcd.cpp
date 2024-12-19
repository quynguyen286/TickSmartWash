#include "lcd.h"

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
