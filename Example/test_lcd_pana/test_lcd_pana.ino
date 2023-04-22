#include <SPI.h>
#include <Panalcd.h>

Panalcd panalcd(5, 17, 21);

int x = 1, y = 1;
char input;
long randNumber;

void setup() {
  Serial.begin(9600);
  panalcd.test_screen(); 
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
  //panalcd.refresh_screen();
  //delay(500);
  //panalcd.clean_screen();
  panalcd.refresh_screen();
  /*
  if(Serial.available()){
    input = Serial.read();
    if(input == 'a') {
      x--;
    } else if(input == 's') {
      y++;
    } else if(input == 'd') {
      x++;
    } else if(input == 'w') {
      y--;
    }

    Serial.print(input, DEC);
    Serial.print(" x: " );
    Serial.print(x);
    Serial.print(" y: " );
    Serial.println(y);

    panalcd.set_pixel(x, y, 1);
    panalcd.refresh_screen();
  }
  */
}

