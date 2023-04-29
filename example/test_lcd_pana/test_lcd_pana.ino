#include <SPI.h>
#include <Panalcd.h>

Panalcd panalcd(5, 17, 21);

int x = 1, y = 1;
char input;
long randNumber;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}
 
void loop() {
  for(int i = 0; i < 500; i++) {
    x = random(102) + 1;
    y = random(65) + 1;
    panalcd.set_pixel(x, y, 1);
  }
  for(int i = 0; i < 100; i++) {
    x = random(102) + 1;
    y = random(65) + 1;
    panalcd.set_pixel(x, y, 0);
  }
  panalcd.refresh_screen();
}

