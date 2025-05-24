#include <ESP32Servo.h>
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

//arm pins
const int legServoPin = 27;  // You can use any PWM-capable GPIO
const int soulderServoPin = 26;
const int elboServoPin = 32;
const int gripServoPin = 33;
//for arms
Servo legServo;
Servo SoulderServo;
Servo elboServo;
Servo gripServo;
// arm initial position
int legPos = 90;
int soulderPos = 80;
int elbowPos = 90;
int grapperPos = 2;

//for battery percentage
const int analogPin = 34;
const float voltageDividerRatio = 4.74;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&gestureData, data, sizeof(gestureData));

  String gesture = String(gestureData.carmove);
  String armGesture = String(gestureData.armmove);
  String action = String(gestureData.action);
  Serial.print("Action : ");
  Serial.print(action);

  if (action == "car") {
    Serial.print("\t Received Gesture: ");
    Serial.println(gesture);
    if (gesture == "R-225") {
      turnRight(225);
    } else if (gesture == "L-225") {
      turnLeft(225);
    } else if (gesture == "B-225") {
      moveBackward(225);
    } else if (gesture == "F-225") {
      moveForward(225);
    } else if (gesture == "R-150") {
      turnRight(150);
    } else if (gesture == "L-150") {
      turnLeft(150);
    } else if (gesture == "B-150") {
      moveBackward(150);
    } else if (gesture == "F-150") {
      moveForward(150);
    } else if (gesture == "R-100") {
      turnRight(100);
    } else if (gesture == "L-100") {
      turnLeft(100);
    } else if (gesture == "B-100") {
      moveBackward(100);
    } else if (gesture == "F-100") {
      moveForward(100);
    } else if (gesture == "R-70") {
      turnRight(70);
    } else if (gesture == "L-70") {
      turnLeft(70);
    } else if (gesture == "B-70") {
      moveBackward(70);
    } else if (gesture == "F-70") {
      moveForward(70);
    } else if (gesture == "STOP") {
      stopMotors();
    }
  } else if (action == "arm") {
    Serial.print("\t leg: ");
    Serial.print(legPos);
    Serial.print("\t soulder: ");
    Serial.print(soulderPos);
    Serial.print("\t elbo: ");
    Serial.print(elbowPos);
    Serial.print("\t grip: ");
    Serial.println(grapperPos);
     Serial.print("\t Received Gesture: ");
  Serial.println(armGesture);
    if (armGesture == "LEFT") {
      turnArmRight();
    } else if (armGesture == "RIGHT") {
      turnArmLeft();
    } else if (armGesture == "BACKWARD-S") {
      moveArmBackwardS();
    } else if (armGesture == "FORWARD-S") {
      moveArmForwardS();
    } else if (armGesture == "OPEN") {
      openGripper();
    } else if (armGesture == "CLOSE") {
      closeGripper();
    } else if (armGesture == "BACKWARD-E") {
      moveArmBackwardE();
    } else if (armGesture == "FORWARD-E") {
      moveArmForwardE();
    }
  } else {
    stopMotors();
  }
}

void setup() {
  Serial.begin(115200);
  //for car
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //for arm
  legServo.setPeriodHertz(50);
  legServo.attach(legServoPin);

  SoulderServo.setPeriodHertz(50);
  SoulderServo.attach(soulderServoPin);

  elboServo.setPeriodHertz(50);
  elboServo.attach(elboServoPin);

  gripServo.setPeriodHertz(50);
  gripServo.attach(gripServoPin);

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
  int rawADC = analogRead(analogPin);
  float voltage = (rawADC / 4095.0) * 3.3 * voltageDividerRatio;

  int batteryPercent = getBatteryPercent(voltage);

  // Serial.print("Battery Voltage: ");
  // Serial.print(voltage);
  Serial.print(" V\tBattery %: ");
  Serial.println(batteryPercent);
  delay(1000);
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

int getBatteryPercent(float voltage) {
  if (voltage >= 16.8) return 100;
  if (voltage <= 12.0) return 0;
  return (int)((voltage - 12.0) * 100 / (16.8 - 12.0));
}

void turnArmRight() {
  if (legPos < 180) {
    legPos++;
    legServo.write(legPos);
    delay(30);
  }
}
void turnArmLeft() {
  if (legPos > 0) {
    legPos--;
    legServo.write(legPos);
    delay(30);
  }
}

void moveArmBackwardS() {
  if (soulderPos > 80) {
    soulderPos--;
    SoulderServo.write(soulderPos);
    delay(30);
  }
}
void moveArmForwardS() {
  if (soulderPos < 174) {
    soulderPos++;
    SoulderServo.write(soulderPos);
    delay(30);
  }
}
void moveArmBackwardE() {
  if (elbowPos < 180) {
    elbowPos++;
    elboServo.write(elbowPos);
    delay(30);
  }
}
void moveArmForwardE() {
  if (elbowPos > 90) {
    elbowPos--;
    elboServo.write(elbowPos);
    delay(30);
  }
}
void openGripper() {

  if (grapperPos < 58) {
    grapperPos++;
    gripServo.write(grapperPos);
    delay(30);
  }
}
void closeGripper() {
  if (grapperPos > 2) {
    grapperPos--;
    gripServo.write(grapperPos);
    delay(30);
  }
}
