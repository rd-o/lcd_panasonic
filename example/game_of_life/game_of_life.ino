#include <SPI.h>
#include <Panalcd.h>
#define MAX_X 103
#define MAX_Y 72

Panalcd panalcd(5, 17, 21);

char game[MAX_X][MAX_Y];
char prev[MAX_X][MAX_Y];
int sum;

void setup() {
  randomSeed(analogRead(0));
  game[1][1] = 1;
  game[1][3] = 1;
  game[2][2] = 1;
  game[2][3] = 1;
  game[3][2] = 1;
  init_game();
}

void init_game() {
  int x,y;
  for(int i = 0; i < 2000; i++) {
    x = random(MAX_X) + 1;
    y = random(MAX_Y) + 1;
    game[x][y] = 1;
  }
  for(int i = 0; i < 50; i++) {
    x = random(MAX_X) + 1;
    y = random(MAX_Y) + 1;
    game[x][y] = 0;
  }
}

void calc_next() {
  for(int i = 0; i < MAX_X; i++) {
    for(int j = 0; j < MAX_Y; j++) {
      prev[i][j] = game[i][j];
    }
  }

  for(int i = 0; i < MAX_X; i++) {
    for(int j = 0; j < MAX_Y; j++) {
      sum = 0;
      if(i > 0) {
        if(j > 0) {
          sum += prev[i - 1][j - 1];
        }

        sum += prev[i - 1][j];

        if(j < MAX_Y - 1) {
          sum += prev[i - 1][j + 1];
        }
      }

      if(j > 0) {
        sum += prev[i][j - 1];
      }

      if(j < MAX_Y - 1) {
        sum += prev[i][j + 1];
      }

      if(i < MAX_X - 1) {
        if(j > 0) {
          sum += prev[i + 1][j - 1];
        }

        sum += prev[i + 1][j];
        
        if(j < MAX_Y - 1) {
          sum += prev[i + 1][j + 1];
        }
      }

      if(sum < 2) {
        //die
        game[i][j] = 0;
      } else if (sum > 3) {
        //die
        game[i][j] = 0;
      } else if (sum == 3) {
        //reborn
        game[i][j] = 1;
      }
    }
  }
  
}
 
void loop() {
  for(int i = 0; i < MAX_X; i++) {
    for(int j = 0; j < MAX_Y; j++) {
      if(game[i][j]){
        panalcd.set_pixel(i + 1, j + 1, 1);
      }
    }
  }
  panalcd.refresh_screen();
  delay(20);
  panalcd.clean_screen();
  calc_next();
}

