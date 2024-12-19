#include "wifi_connect.h"



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