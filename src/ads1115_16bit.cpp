#include "ads1115_16bit.h"
#include "ADS1X15.h"

ADS1115 ADS(0x48);
ADS1115 ADS1(0x49);

float doc_ADS(int n)
{
  	ADS.setGain(2);
	ADS1.setGain(2);

	int16_t val_0 = ADS.readADC(0);  
  	int16_t val_1 = ADS.readADC(1);  
  	int16_t val_2 = ADS.readADC(2);  
  	int16_t val_3 = ADS.readADC(3);  
  	int16_t val1_0 = ADS1.readADC(0);  
  	int16_t val1_1 = ADS1.readADC(1);  
  	int16_t val1_2 = ADS1.readADC(2);  
  	int16_t val1_3 = ADS1.readADC(3);

  	float f = ADS.toVoltage(1);  //  voltage factor
	
    float may1 = ((val_0* f)/sqrt(2))*30;
    float may2 = ((val_1 * f)/sqrt(2))*30;
    float may3 = ((val_2 * f)/sqrt(2))*30;
    float may4 = ((val_3 * f)/sqrt(2))*30;
    float may11 = ((val1_0 * f)/sqrt(2))*30;
    float may12 = ((val1_1 * f)/sqrt(2))*30;
    float may13 = ((val1_2 * f)/sqrt(2))*30;
    float may14 = ((val1_3 * f)/sqrt(2))*30;

    float a[9];
   	a[1] = may1;
 	a[2] = may2;
  	a[3] = may3;
  	a[4] = may4;
  	a[5] = may11;
  	a[6] = may12;
  	a[7] = may13;
  	a[8] = may14;

	return a[n];

	// if (n == 1) return may1;
	// else if (n == 2) return may2;
	// else if (n == 3) return may3;
	// else if (n == 4) return may4;
	// else if (n == 5) return may11;
	// else if (n == 6) return may12;
	// else if (n == 7) return may13;
	// else if (n == 8) return may14;
}


// //ADS1115 
// #include <Arduino.h>


// void setup() 
// {
// 	   Wire.begin();

//   	Serial.begin(9600);
//   	Serial.println(__FILE__);
//   	Serial.print("ADS1X15_LIB_VERSION: ");
//   	Serial.println(ADS1X15_LIB_VERSION);

//   	ADS.begin();

// }


