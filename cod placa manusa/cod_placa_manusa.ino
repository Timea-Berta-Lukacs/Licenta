#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Wire.h"
#include "MPU6050.h"

#define CSN_GPIO    9
#define CE_GPIO     10

const int buttonPin = 2;
const int ledPin = 6;
int buttonState = 0;
int tiltsensor = 4;

RF24 radio(CE_GPIO, CSN_GPIO);
MPU6050 accelgyro;

int16_t accelX, accelY, accelZ;
int16_t giroX, giroY, giroZ;

const uint64_t pipe_arduino1 = 0xE8E8F0F0E1LL;
const uint64_t pipe_arduino2 = 0xE8E8F0F0E2LL;
unsigned char commandType = 0;
unsigned char speed = 0;
unsigned char isEnable = 0;
unsigned char buttonPress = 0;
unsigned char buffer[2];

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(tiltsensor, INPUT);
  
  radio.begin();
  radio.openWritingPipe(pipe_arduino1);
  radio.openWritingPipe(pipe_arduino2);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  radio.write(&commandType, sizeof(commandType));

  Wire.begin();
  accelgyro.initialize();

  buffer[0] = 0;
  buffer[1] = 0;  
}

void loop()
{
  accelgyro.getMotion6(&accelX, &accelY, &accelZ, &giroX, &giroY, &giroZ);
 if(!(PIND & 0x04)&&(buttonPress==0))
  {
      buttonPress = 1;
      if(isEnable==0)
      {
        isEnable = 1;
      }
      else
      {
        isEnable = 0;
        PORTD &= 0xF7;
      }
  }
  else if((PIND & 0x04)&&(buttonPress==1))
  {
    buttonPress = 0;
  }

  if((accelY<=-4000)||((accelY>=4000)))
  {
    if((accelX>=-4000)||((accelX<=4000)))
    {
      if((accelY<=-4000))
      {
          commandType = 1;
          speed = (accelY + 4000)/-2000 + 1;
          if(speed>5)
          {
            speed = 5;  
          }
      }

      if((accelY>=4000))
      {
          commandType = 2;
          speed = (accelY - 4000)/2000 + 1;
          if(speed>5)
          {
            speed = 5;  
          }
      }
    }
    else
    {
        commandType = 0;
        speed = 0;
    } 
  }
  else if((accelX<=-4000)||((accelX>=4000)))
  {
    if((accelY>=-4000)||((accelY<=4000)))
    {
      if((accelX<=-4000))
      {
          commandType = 4;
          speed = (accelX + 4000)/-2000 + 1;
          if(speed>5)
          {
            speed = 5;  
          }
      }

      if((accelX>=4000))
      {
          commandType = 3;
          speed = (accelX - 4000)/2000 + 1;
          if(speed>5)
          {
            speed = 5;  
          }
      }
    }
    else
    {
        commandType = 0;
        speed = 0;
    } 
  }
  else
  {
      commandType = 0;
      speed = 0;
  }
  if(isEnable)
  {
    radio.openWritingPipe(pipe_arduino1);
    buffer[0] = commandType;
    buffer[1] = speed;
    radio.write(&buffer, 2);
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    radio.openWritingPipe(pipe_arduino2);
    buffer[0] = commandType;
    buffer[1] = digitalRead(tiltsensor);
    radio.write(&buffer, 2);
    digitalWrite(ledPin, LOW);
  }
   delay(100);
}
