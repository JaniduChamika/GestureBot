#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  char carmove[20];
  char action[20];
  char armmove[20];
} struct_message;

struct_message gestureData;

//LEFT
int IN1 = 16;
int IN2 = 17;
int ENA = 4;
//RIGHT
int IN3 = 18;
int IN4 = 19;
int ENB = 5;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&gestureData, data, sizeof(gestureData));

  String gesture = String(gestureData.carmove);
  String action = String(gestureData.action);
  Serial.print("Action : ");
  Serial.print(action);
  Serial.print("\t Received Gesture: ");
  Serial.println(gesture);
  if (action == "car") {
    if (gesture == "R-250") {
      turnRight(250);
    } else if (gesture == "L-250") {
      turnLeft(250);
    } else if (gesture == "B-250") {
      moveBackward(250);
    } else if (gesture == "F-250") {
      moveForward(250);
    } else if (gesture == "R-200") {
      turnRight(200);
    } else if (gesture == "L-200") {
      turnLeft(200);
    } else if (gesture == "B-200") {
      moveBackward(200);
    } else if (gesture == "F-200") {
      moveForward(200);
    } else if (gesture == "R-150") {
      turnRight(150);
    } else if (gesture == "L-150") {
      turnLeft(150);
    } else if (gesture == "B-150") {
      moveBackward(150);
    } else if (gesture == "F-150") {
      moveForward(150);
    } else if (gesture == "STOP") {
      stopMotors();
    }
  } else {
    stopMotors();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

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

void moveForward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moveBackward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}