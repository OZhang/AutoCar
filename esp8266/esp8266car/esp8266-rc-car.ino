#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char* ssid = "ty200824G"; //Enter your wifi network SSID
const char* password = "hongjie@1982"; //Enter your wifi network password

//Motor B
int PWMB = 16; //Speed control-d0
int BIN1 = 5; //Direction-d1
int BIN2 = 4; //Direction-d2
//Motor A
int PWMA = 2; //Speed control -D4
int AIN1 = 14; //Direction -D5
int AIN2 = 12; //Direction-D6

const int SERVER_PORT = 1111;
const int BAUD_RATE = 9600;

byte incomingByte = 0;

bool forwardsPressed = false;
bool backwardsPressed = false;
bool rightPressed = false;
bool leftPressed = false;

const int FORWARDS_PRESSED = 1;
const int FORWARDS_RELEASED = 2;
const int BACKWARDS_PRESSED = 3;
const int BACKWARDS_RELEASED = 4;
const int RIGHT_PRESSED = 5;
const int RIGHT_RELEASED = 6;
const int LEFT_PRESSED = 7;
const int LEFT_RELEASED = 8;

byte packetBuffer[512];

WiFiUDP Udp;

void initOutputs() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);  
}

void connectWifi() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WIFI network");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Udp.begin(SERVER_PORT);
}

void moveForwards() {
  //MotorA
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 100);
  //MotorB
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 100);
  delay(50);
}

void moveBackwards() {
  //MotorA
  digitalWrite(AIN1,LOW );
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 100);
  //MotorB
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 100);
  delay(50);
}

void turnRight() {
  //MotorA
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA,  150);
  //MotorB
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 50);
  delay(50);
}

void turnLeft() {
  //MotorA
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 50);
  //MotorB
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 150);
  delay(50);
}

void resetSteering() {
  //MotorA
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  //MotorB
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void resetEngine() {
  //MotorA
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  //MotorB
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void detectKeyPresses() {
  if (incomingByte == FORWARDS_PRESSED) {
      forwardsPressed = true;
      backwardsPressed = false;
      rightPressed = false;
      leftPressed = false;
    }
    else if (incomingByte == BACKWARDS_PRESSED) {
      backwardsPressed = true;
      forwardsPressed = false;
      rightPressed = false;
      leftPressed = false;
    }
   else  if (incomingByte == FORWARDS_RELEASED) {
      forwardsPressed = false;
    }
    else if (incomingByte == BACKWARDS_RELEASED) {
      backwardsPressed = false;
    }
    else if (incomingByte == RIGHT_PRESSED) {
      forwardsPressed = false;
      backwardsPressed = false;
      rightPressed = true;
      leftPressed = false;
    }
    else if (incomingByte == LEFT_PRESSED) {
      forwardsPressed = false;
      backwardsPressed = false;
      rightPressed = false;
      leftPressed = true;
    }
    else if (incomingByte == RIGHT_RELEASED) {
      rightPressed = false;
    }
    else if (incomingByte == LEFT_RELEASED) {
      leftPressed = false;
    }
    delay(50);
}

void handlePinOutputs() {
  if (forwardsPressed == true) {
    moveForwards();
  }
  else if (backwardsPressed == true) {
    moveBackwards();
  }
  else if (rightPressed == true) {
    turnRight();
  }
  else if (leftPressed == true) {
    turnLeft();
  }
  else {
    resetSteering();
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  delay(10);

  initOutputs();
  connectWifi();
}

void loop() {
  int noBytes = Udp.parsePacket();
  String received_command = "";

  if ( noBytes ) {
    Serial.print(millis() / 1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());

    Udp.read(packetBuffer,noBytes);
    Serial.println();

    Serial.println(packetBuffer[0]);
    incomingByte = packetBuffer[0];
    Serial.println();
  }
  detectKeyPresses();
  handlePinOutputs();
}
