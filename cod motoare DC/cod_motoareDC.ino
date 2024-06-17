#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define IN1  2
#define IN2  3
#define IN3  4
#define IN4  5

RF24 radio(8, 9); 

const uint64_t pipe_arduino1 = 0xE8E8F0F0E1LL;
unsigned char tipComanda = 0;
unsigned char viteza = 0;
unsigned char stop = 0;
unsigned char buffer[2];
unsigned int contorOprire = 0;

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  radio.begin();
  radio.openReadingPipe(1, pipe_arduino1);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop()
{
  if (radio.available())
  {  
    delay(1);
    radio.read(&buffer, 2);
    tipComanda = buffer[0];
    viteza = buffer[1];
  }
  if(stop==0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    contorOprire++;
    if(contorOprire>=((5-viteza)*100))
   {
      contorOprire = 0;
      stop = 1;
    }
  }
  else if(stop==1)
  {
    contorOprire++;
    if(contorOprire>=((viteza)*100))
    {
      contorOprire = 0;
      stop = 0;
    }
    
    if(tipComanda == 1)
    {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

    }
    else if(tipComanda == 2)
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    else if(tipComanda == 4)
    {
       digitalWrite(IN1, HIGH);
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
    }
    else if(tipComanda == 3)
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    else if(tipComanda == 0)
    {      
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    else
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
  }
}


