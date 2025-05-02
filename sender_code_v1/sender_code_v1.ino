#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
typedef struct struct_message {
  String  carmove;
  String action;
  String armmove;
} struct_message;

struct_message gestureData;

// Replace with receiver ESP32 MAC Address
uint8_t receiverMac[] = { 0xF4, 0x65, 0x0B, 0x4A, 0x7C, 0x34 };  // example
void onSent(uint8_t *mac, uint8_t sendStatus) {
  Serial.print("Send Status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    while (1)
      ;
  }
  Serial.println("MPU6050 ready!");

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed!");
    return;
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: 8G");
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: 44Hz");

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_register_send_cb(onSent);
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print("/t, Y: ");
  Serial.print(a.acceleration.y);
  if (a.acceleration.y > 7) gestureData.carmove = "R-250";
  else if (a.acceleration.y < -7) gestureData.carmove = "L-250";
  else if (a.acceleration.x > 7) gestureData.carmove = "B-250";
  else if (a.acceleration.x < -7) gestureData.carmove = "F-250";
  else if (a.acceleration.y > 5) gestureData.carmove = "R-200";
  else if (a.acceleration.y < -5) gestureData.carmove = "L-200";
  else if (a.acceleration.x > 5) gestureData.carmove = "B-200";
  else if (a.acceleration.x < -5) gestureData.carmove = "F-200";
  else if (a.acceleration.y > 3) gestureData.carmove = "R-150";
  else if (a.acceleration.y < -3) gestureData.carmove = "L-150";
  else if (a.acceleration.x > 3) gestureData.carmove = "B-150";
  else if (a.acceleration.x < -3) gestureData.carmove = "F-150";
  else gestureData.carmove = "STOP";

  esp_now_send(receiverMac, (uint8_t *)&gestureData, sizeof(gestureData));
  Serial.print("/t Gesture: " + gestureData.carmove);
  Serial.println("");
  delay(2000);
}