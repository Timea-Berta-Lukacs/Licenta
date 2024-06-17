#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Wire.h"
#include "MPU6050.h"

#define CSN_GPIO    9
#define CE_GPIO     10

const int pinButon = 2;
const int pinLED = 6;
int pinSenzorInclinare = 4;

RF24 radio(CE_GPIO, CSN_GPIO);
MPU6050 senzorAccelGiro;

int16_t accelX, accelY, accelZ;
int16_t giroX, giroY, giroZ;

const uint64_t pipe_arduino1 = 0xE8E8F0F0E1LL;
const uint64_t pipe_arduino2 = 0xE8E8F0F0E2LL;
unsigned char tipComanda = 0;
unsigned char viteza = 0;
unsigned char activ = 0;
unsigned char stareButon = 0;
unsigned char buffer[2];

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  pinMode(pinButon, INPUT);
  pinMode(pinSenzorInclinare, INPUT);
  
  radio.begin();
  radio.openWritingPipe(pipe_arduino1);
  radio.openWritingPipe(pipe_arduino2);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  radio.write(&tipComanda, sizeof(tipComanda));

  Wire.begin();
  senzorAccelGiro.initialize();

  buffer[0] = 0;
  buffer[1] = 0;  
}

void loop()
{
  senzorAccelGiro.getMotion6(&accelX, &accelY, &accelZ, &giroX, &giroY, &giroZ);
 if(!(PIND & 0x04)&&(stareButon==0))
  {
      stareButon = 1;
      if(activ==0)
      {
        activ = 1;
      }
      else
      {
        activ = 0;
        PORTD &= 0xF7;
      }
  }
  else if((PIND & 0x04)&&(stareButon==1))
  {
    stareButon = 0;
  }

  if((accelY<=-4000)||((accelY>=4000)))
  {
    if((accelX>=-4000)||((accelX<=4000)))
    {
      if((accelY<=-4000))
      {
          tipComanda = 1;
          viteza = (accelY + 4000)/-2000 + 1;
          if(viteza>5)
          {
            viteza = 5;  
          }
      }

      if((accelY>=4000))
      {
          tipComanda = 2;
          viteza = (accelY - 4000)/2000 + 1;
          if(viteza>5)
          {
            viteza = 5;  
          }
      }
    }
    else
    {
        tipComanda = 0;
        viteza = 0;
    } 
  }
  else if((accelX<=-4000)||((accelX>=4000)))
  {
    if((accelY>=-4000)||((accelY<=4000)))
    {
      if((accelX<=-4000))
      {
          tipComanda = 4;
          viteza = (accelX + 4000)/-2000 + 1;
          if(viteza>5)
          {
            viteza = 5;  
          }
      }

      if((accelX>=4000))
      {
          tipComanda = 3;
          viteza = (accelX - 4000)/2000 + 1;
          if(viteza>5)
          {
            viteza = 5;  
          }
      }
    }
    else
    {
        tipComanda = 0;
        viteza = 0;
    } 
  }
  else
  {
      tipComanda = 0;
      viteza = 0;
  }
  if(activ)
  {
    radio.openWritingPipe(pipe_arduino1);
    buffer[0] = tipComanda;
    buffer[1] = viteza;
    radio.write(&buffer, 2);
    digitalWrite(pinLED, HIGH);
  }
  else
  {
    radio.openWritingPipe(pipe_arduino2);
    buffer[0] = tipComanda;
    buffer[1] = digitalRead(pinSenzorInclinare);
    radio.write(&buffer, 2);
    digitalWrite(pinLED, LOW);
  }
   delay(100);
}
