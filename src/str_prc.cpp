#include "str_prc.h"

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
