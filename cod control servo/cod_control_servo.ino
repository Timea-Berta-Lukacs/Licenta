#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver Servo = Adafruit_PWMServoDriver();

#define IN1  2
#define IN2  3
#define IN3  4
#define IN4  5

RF24 radio(9, 8); 

const uint64_t pipe_arduino2 = 0xE8E8F0F0E2LL;
unsigned char tipComanda = 0;
unsigned char buffer[2];
int starePrecedentaUnghi = LOW;
int stareCurentaUnghi;

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  radio.begin();
  radio.openReadingPipe(1, pipe_arduino2);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Servo.begin();
  Servo.setPWMFreq(60);
}

void loop() {
  if (radio.available())
  {  
    delay(1);
    radio.read(&buffer, 2);
    tipComanda = buffer[0];
    stareCurentaUnghi = buffer[1];
  }
  Serial.print(tipComanda);
  Serial.print("\n");
  if (starePrecedentaUnghi == LOW && stareCurentaUnghi == HIGH) {
    Serial.print("1\n");
    Servo.setPWM(9, 0, 125);
    delay(500);
    Servo.setPWM(9, 0, 0);
  }
  else if (starePrecedentaUnghi == HIGH && stareCurentaUnghi == LOW) {
    Serial.print("0\n");
    Servo.setPWM(9, 0, 575);
    delay(500);
    Servo.setPWM(9, 0, 0);
  }
  starePrecedentaUnghi = stareCurentaUnghi;

  switch (tipComanda) {
    case 1:
        Servo.setPWM(1, 0, 125);
        delay(10);
        Servo.setPWM(1, 0, 0);
        break;
    case 2:
        Servo.setPWM(1, 0, 575);
        delay(10);
        Servo.setPWM(1, 0, 0);
        break;
    case 3:
        Servo.setPWM(8, 0, 125);
        delay(10);
        Servo.setPWM(8, 0, 0);
        break;
    case 4:
        Servo.setPWM(8, 0, 575);
        delay(10);
        Servo.setPWM(8, 0, 0);
        break;
    default:
        break;
  }
}
