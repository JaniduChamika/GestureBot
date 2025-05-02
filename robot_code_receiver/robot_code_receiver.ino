#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  String  carmove;
  String action;
  String armmove;
} struct_message;

struct_message gestureData;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&gestureData, data, sizeof(gestureData));

  String gesture =gestureData.carmove;
  Serial.print("Received Gesture: ");
  Serial.println(gesture);

  // Here you can add actions like:
  if (gesture == "F-150") {
    // digitalWrite(motorPin1, HIGH); etc.
    Serial.println("Move Forward");
  } else if (gesture == "B-150") {
    Serial.println("Move Backward");
  } else if (gesture == "L-150") {
    Serial.println("Turn Left");
  } else if (gesture == "R-150") {
    Serial.println("Turn Right");
  } else if (gesture == "STOP") {
    Serial.println("Stop");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);
  Serial.println("ESP-NOW Receiver Ready");
}

void loop() {
  // Nothing here, just receiving
}