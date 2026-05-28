#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  WiFi.mode(WIFI_STA);
  
  Serial.println("\n\nESP32-C3 MAC Address Finder");
  Serial.println("===========================\n");
  Serial.println("Formatted MAC Address:");
  Serial.println(WiFi.macAddress());
  
  Serial.println("\nHex Array Format (copy to remote code):");
  Serial.print("uint8_t ctrl[] = {");
  uint8_t mac[6];
  WiFi.macAddress(mac);
  for(int i = 0; i < 6; i++) {
    Serial.print("0x");
    if(mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if(i < 5) Serial.print(", ");
  }
  Serial.println("};");
  
  Serial.println("\nUsage: Copy the line above into esp32c3_led_remote.ino at line 13");
}

void loop() {
  delay(1000);
}
