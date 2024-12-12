#include "wifi.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Wire.h>






// void setup()
// {
// 	Serial.begin(9600);
// 	initWiFi();
// 	Serial.print("RSSI: ");
// 	Serial.println(WiFi.RSSI());
// }

// void loop()
// {
// 	unsigned long currentMillis = millis();
// 	if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
// 	{
// 		Serial.print(millis());
// 		Serial.println("Reconnecting to WiFi...");
// 		WiFi.disconnect();
// 		WiFi.reconnect();
// 		previousMillis = currentMillis;
// 	}
// 	if (WiFi.status() == WL_CONNECTED)
// 	{
// 		Serial.print("RSSI: ");
// 		Serial.println(WiFi.RSSI());	
// 	}
// }