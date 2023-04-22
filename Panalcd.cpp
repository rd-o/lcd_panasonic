#include <SPI.h>
#include "Arduino.h"
#include "Panalcd.h"

const char config_lcd[] = {0xA0 , 0xC0 , 0xA2 , 0x2C , 0x2E , 0x2F , 0x25 , 0x81 , 0x26 , 0xE4 , 0xA0 , 0xC0 , 0xA2 , 0x2C , 0x2E , 0x2F , 0x25 , 0x81 , 0x26 , 0xE4 , 0xAF };

const char data2[] = {0xFF, 0x7E, 0xF0, 0x0F};
const char flisol[] = {0x7C,0x14,0x04,0x00,0x7C,0x40,0x40,0x00,0x00,0x7C,0x00,0x48,0x54,0x24,0x00,0x7C,0x44,0x7C,0x00,0x7C,0x40,0x40};

//#define MAX_X 132
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
  int size = 103;

  for(int j = 0; j <= 8; j++) {
    send_command(0xB0 + j);
    send_command(0x10);
    send_command(0x00);

    digitalWrite(pinRes, HIGH);
    digitalWrite(pinCS, LOW);
    digitalWrite(pinDC, HIGH);
    delayMicroseconds(5);

    for(int i = 0; i < size; i++) {
      //SPI.transfer(0x00);
      display[i][j] = 0x00;
    }
  }
}

void send_custom_data(){
  int size = sizeof(flisol) / sizeof(flisol[0]);

  digitalWrite(pinRes, HIGH);
  digitalWrite(pinCS, LOW);
  digitalWrite(pinDC, HIGH);
  delayMicroseconds(5);

  for(int i = 0; i < size; i++) {
    Serial.println(flisol[i], HEX);
    SPI.transfer(flisol[i]);
    delay(500);
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
  //1-8 -> 0 
  //9-16 -> 1
  //17-24 -> 2
  //25-32 -> 3
  //33-40 -> 4
  //41-48 -> 5
  //49-56 -> 6
  //57-64 -> 7
  //65-72 -> 8
  int page = (y - 1) / 8;
/*
  if(y >= 1 && y <=8) {
    page = 0;
  } else if (y >= 9 && y <= 16) {
    page = 1;
  } else if (y >= 17 && y <= 24) {
    page = 2;
  } else if (y >= 25 && y <= 32) {
    page = 3;
  } else if (y >= 33 && y <= 40) {
    page = 4;
  } else if (y >= 41 && y <= 48) {
    page = 5;
  } else if (y >= 49 && y <= 56) {
    page = 6;
  } else if (y >= 57 && y <= 64) {
    page = 7;
  } else if (y >= 65 && y <= 72) {
    page = 8;
  }
  */
  int bit = y - page * 8 - 1;

  if (value) {
    display[x - 1][page] = display[x - 1][page] | (1 << bit);
  } else {
    display[x - 1][page] = display[x - 1][page] & ~(1 << bit);
  }
  //refresh_screen();
}

void Panalcd::test_screen(){
  clean_screen();
  //display[0][0] = 0xA9;
  /*
  draw_pixel(0, 0, 1);
  draw_pixel(0, 1, 1);
  draw_pixel(0, 2, 1);
  draw_pixel(0, 3, 1);
  draw_pixel(0, 4, 1);
  draw_pixel(0, 5, 1);
  draw_pixel(0, 6, 1);
  draw_pixel(0, 7, 1);
  draw_pixel(0, 8, 1);
  draw_pixel(0, 9, 1);
  */
  //draw_pixel(0, 10, 1);
  /*
  draw_pixel(1, 0, 1);
  draw_pixel(2, 0, 1);
  draw_pixel(3, 0, 1);
  draw_pixel(4, 0, 1);
  draw_pixel(5, 0, 1);
  */
  /*
  send_command(0xB1);
  send_command(0x10);
  send_command(0x00);
  send_custom_data();
  */
}

