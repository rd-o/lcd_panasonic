#include <SPI.h>
#include "Arduino.h"
#include "Panalcd.h"

const char config_lcd[] = {0xA0 , 0xC0 , 0xA2 , 0x2C , 0x2E , 0x2F , 0x25 , 0x81 , 0x26 , 0xE4 , 0xA0 , 0xC0 , 0xA2 , 0x2C , 0x2E , 0x2F , 0x25 , 0x81 , 0x26 , 0xE4 , 0xAF };

#define MAX_X 103
#define MAX_Y 9


char display[MAX_X][MAX_Y];

int pinCS;// = 5;
int pinDC;// = 17;
int pinRes;// = 21;


Panalcd::Panalcd(int cs, int dc, int res)
{
  pinCS = cs;
  pinDC = dc;
  pinRes = res;

  pinMode(pinCS, OUTPUT);
  pinMode(pinDC, OUTPUT);
  pinMode(pinRes, OUTPUT);
   
  digitalWrite(pinCS, HIGH);
  digitalWrite(pinDC, HIGH);
   
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  init_lcd();
}

void send_command(char cmd){
    digitalWrite(pinDC, LOW);
    delayMicroseconds(5);
    digitalWrite(pinCS, LOW);
    delayMicroseconds(4);
    SPI.transfer(cmd);
    delayMicroseconds(1);
    digitalWrite(pinCS, HIGH);
    delayMicroseconds(2);
    digitalWrite(pinDC, HIGH);
}

void Panalcd::init_lcd() {
  int config_size = sizeof(config_lcd) / sizeof(config_lcd[0]);

  digitalWrite(pinRes, HIGH);
  digitalWrite(pinCS, HIGH);
  digitalWrite(pinDC, HIGH);
  delayMicroseconds(5);

  for(int i = 0; i < config_size; i++) {
    send_command(config_lcd[i]);
  }
}

void Panalcd::clean_screen(){
  int size = MAX_X;

  for(int j = 0; j < MAX_Y; j++) {
    send_command(0xB0 + j);
    send_command(0x10);
    send_command(0x00);

    digitalWrite(pinRes, HIGH);
    digitalWrite(pinCS, LOW);
    digitalWrite(pinDC, HIGH);
    delayMicroseconds(5);

    for(int i = 0; i < size; i++) {
      display[i][j] = 0x00;
    }
  }
}

void Panalcd::refresh_screen() {
  for(int j = 0; j < MAX_Y; j++) {
    send_command(0xB0 + j);
    send_command(0x10);
    send_command(0x00);

    digitalWrite(pinRes, HIGH);
    digitalWrite(pinCS, LOW);
    digitalWrite(pinDC, HIGH);
    delayMicroseconds(5);

    for(int i = 0; i < MAX_X; i++) {
      SPI.transfer(display[i][j]);
    }
  }
}

void Panalcd::set_pixel(int x, int y, int value) {
  int page = (y - 1) / 8;
  int bit = y - page * 8 - 1;

  if (value) {
    display[x - 1][page] = display[x - 1][page] | (1 << bit);
  } else {
    display[x - 1][page] = display[x - 1][page] & ~(1 << bit);
  }
}


