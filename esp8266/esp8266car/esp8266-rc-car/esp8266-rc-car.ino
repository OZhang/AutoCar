#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char* ssid = "SSID"; //Enter your wifi network SSID
const char* password = "PASSWORD"; //Enter your wifi network password

// assign pin num
int right_pin = 5;//D1
int left_pin = 4;//D2
int forward_pin = 14;//D5
int reverse_pin = 12;//D6

const int SERVER_PORT = 1111;
const int BAUD_RATE = 115200;

byte incomingByte = 0;
byte packetBuffer[512];
// duration for output
int delaytime = 50;
// initial command
int command = 0;
WiFiUDP Udp;

void initOutputs() {
  pinMode(right_pin, OUTPUT);
  pinMode(left_pin, OUTPUT);
  pinMode(forward_pin, OUTPUT);
  pinMode(reverse_pin, OUTPUT);
  Serial.begin(115200);
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
  else{
    reset();
  }
  send_command(incomingByte,delaytime);
}
void right(int delaytime){
  digitalWrite(right_pin, LOW);
  delay(delaytime);
}

void left(int delaytime){
  digitalWrite(left_pin, LOW);
  delay(delaytime);
}

void forward(int delaytime){
  digitalWrite(forward_pin, LOW);
  delay(delaytime);
}

void reverse(int delaytime){
  digitalWrite(reverse_pin, LOW);
  delay(delaytime);
}

void forward_right(int delaytime){
  digitalWrite(forward_pin, LOW);
  digitalWrite(right_pin, LOW);
  delay(delaytime);
}

void reverse_right(int delaytime){
  digitalWrite(reverse_pin, LOW);
  digitalWrite(right_pin, LOW);
  delay(delaytime);
}

void forward_left(int delaytime){
  digitalWrite(forward_pin, LOW);
  digitalWrite(left_pin, LOW);
  delay(delaytime);
}

void reverse_left(int delaytime){
  digitalWrite(reverse_pin, LOW);
  digitalWrite(left_pin, LOW);
  delay(delaytime);
}

void reset(){
  digitalWrite(right_pin, HIGH);
  digitalWrite(left_pin, HIGH);
  digitalWrite(forward_pin, HIGH);
  digitalWrite(reverse_pin, HIGH);
}

void send_command(int command, int delaytime){
  switch (command){

     //reset command
     case 0: reset(); break;

     // single command
     case 1: forward(delaytime); break;
     case 2: reverse(delaytime); break;
     case 3: right(delaytime); break;
     case 4: left(delaytime); break;

     //combination command
     case 6: forward_right(delaytime); break;
     case 7: forward_left(delaytime); break;
     case 8: reverse_right(delaytime); break;
     case 9: reverse_left(delaytime); break;

     default: Serial.print("Inalid Command\n");
    }
}