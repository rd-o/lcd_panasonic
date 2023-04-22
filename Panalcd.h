#ifndef Panalcd_h
#define Panalcd_h

#include "Arduino.h"

class Panalcd

{
  public:

    Panalcd(int cs, int dc, int res);
    void init_lcd();
    void clean_screen();
    void test_screen();
    void set_pixel(int x, int y, int value);
    void refresh_screen();

  private:

};


#endif
