#include <esp_now.h>
#include <WiFi.h>
#include <nvs_flash.h>
#include <nvs.h>

#define PWM_PIN 5
#define PWM_FREQ 20000
#define PWM_RES 8

uint8_t brightness = 0, target = 0;
const uint8_t STEP = 5, DELAY = 30;
unsigned long lastFade = 0, lastSave = 0;

typedef struct { uint8_t cmd, val; } Msg;

nvs_handle_t nvsHandle;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  Msg msg;
  memcpy(&msg, data, sizeof(msg));
  
  switch(msg.cmd) {
    case 0: target = msg.val; break;
    case 1: target = (brightness + msg.val > 255) ? 255 : brightness + msg.val; break;
    case 2: target = (brightness < msg.val) ? 0 : brightness - msg.val; break;
  }
}

void saveBrightness() {
  nvs_set_u8(nvsHandle, "bright", brightness);
  nvs_commit(nvsHandle);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  nvs_flash_init();
  nvs_open("storage", NVS_READWRITE, &nvsHandle);
  nvs_get_u8(nvsHandle, "bright", &brightness);
  target = brightness;
  
  ledcAttach(PWM_PIN, PWM_FREQ, PWM_RES);
  ledcWrite(PWM_PIN, brightness);
  
  WiFi.mode(WIFI_STA);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  unsigned long now = millis();
  
  if (now - lastFade >= DELAY) {
    lastFade = now;
    
    if (brightness < target) {
      brightness += STEP;
      if (brightness > target) brightness = target;
    } else if (brightness > target) {
      brightness -= STEP;
      if (brightness < target) brightness = target;
    }
    ledcWrite(PWM_PIN, brightness);
  }
  
  if (now - lastSave > 5000 && brightness != target) {
    lastSave = now;
    saveBrightness();
  }
  
  delay(5);
}
