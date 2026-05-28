#include <esp_now.h>
#include <WiFi.h>
#include <nvs_flash.h>
#include <nvs.h>

#define PWR 9
#define UP 7
#define DN 8

uint8_t ctrl[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
uint8_t brightness = 128;
unsigned long lastActivity = 0;

typedef struct { uint8_t cmd, val; } Msg;

uint8_t lastStates[3] = {1, 1, 1};
nvs_handle_t nvsHandle;

void saveBrightness() {
  nvs_flash_init();
  nvs_open("storage", NVS_READWRITE, &nvsHandle);
  nvs_set_u8(nvsHandle, "bright", brightness);
  nvs_commit(nvsHandle);
  nvs_close(nvsHandle);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  nvs_flash_init();
  nvs_open("storage", NVS_READWRITE, &nvsHandle);
  nvs_get_u8(nvsHandle, "bright", &brightness);
  nvs_close(nvsHandle);
  
  pinMode(PWR, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DN, INPUT_PULLUP);
  
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_9, 0);
  WiFi.mode(WIFI_STA);
  
  esp_now_init();
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, ctrl, 6);
  peer.channel = 0;
  esp_now_add_peer(&peer);
  
  lastActivity = millis();
}

void send(uint8_t cmd, uint8_t val) {
  Msg msg = {cmd, val};
  esp_now_send(ctrl, (uint8_t*)&msg, sizeof(msg));
}

void loop() {
  uint8_t states[3] = {digitalRead(PWR), digitalRead(UP), digitalRead(DN)};
  
  if (lastStates[0] == 1 && states[0] == 0) {
    lastActivity = millis();
    brightness = (brightness == 0) ? 128 : 0;
    send(0, brightness);
    saveBrightness();
    delay(50);
  }
  if (lastStates[1] == 1 && states[1] == 0) {
    lastActivity = millis();
    brightness = (brightness + 25 > 255) ? 255 : brightness + 25;
    send(1, 25);
    saveBrightness();
    delay(50);
  }
  if (lastStates[2] == 1 && states[2] == 0) {
    lastActivity = millis();
    brightness = (brightness < 25) ? 0 : brightness - 25;
    send(2, 25);
    saveBrightness();
    delay(50);
  }
  
  memcpy(lastStates, states, 3);
  
  if (millis() - lastActivity > 10000) {
    delay(100);
    esp_deep_sleep_start();
  }
  
  delay(20);
}
