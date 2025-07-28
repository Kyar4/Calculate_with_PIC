#use delay(clock=20000000)

// I/O d?nh nghia cho keypad 5x5
#define ROW1 PIN_D0
#define ROW2 PIN_D1
#define ROW3 PIN_D2
#define ROW4 PIN_D3
#define ROW5 PIN_E0

#define COL1 PIN_D4
#define COL2 PIN_D5
#define COL3 PIN_D6
#define COL4 PIN_D7
#define COL5 PIN_E1

#ifndef __KEY_5X5_
#define __KEY_5X5_

// Ma tr?n phím 5x5
static const char KEY_5X5[5][5] = {
   {'`','s','c','t','^'},
   {'7','8','9','/','a'},
   {'4','5','6','*','q'},
   {'1','2','3','-','l'},
   {'#','0','=','+','m'}
};

// Kh?i t?o các chân
void key_5x5_init(void) {
   output_drive(ROW1);
   output_drive(ROW2);
   output_drive(ROW3);
   output_drive(ROW4);
   output_drive(ROW5);

   output_float(COL1);
   output_float(COL2);
   output_float(COL3);
   output_float(COL4);
   output_float(COL5);
}

// Ki?m tra xem có phím nào dang nh?n
int1 button5x5_is_pressed(void) {
   output_low(ROW1);
   output_low(ROW2);
   output_low(ROW3);
   output_low(ROW4);
   output_low(ROW5);

   if (
      !input(COL1) || !input(COL2) || !input(COL3) ||
      !input(COL4) || !input(COL5)
   )
      return 1;
   else
      return 0;
}

// Ch?n hàng d? quét
void check_row5x5(int row) {
   output_high(ROW1);
   output_high(ROW2);
   output_high(ROW3);
   output_high(ROW4);
   output_high(ROW5);

   switch(row) {
      case 0: output_low(ROW1); break;
      case 1: output_low(ROW2); break;
      case 2: output_low(ROW3); break;
      case 3: output_low(ROW4); break;
      case 4: output_low(ROW5); break;
   }
}

// Tr? v? ký t? phím n?u có nh?n
char get_key_5x5(void) {
   int row;

   if (button5x5_is_pressed()) {
      delay_ms(5);
      if (button5x5_is_pressed()) {
         for (row = 0; row < 5; row++) {
            check_row5x5(row);
            if (!input(COL1)) return KEY_5X5[4 - row][0];
            if (!input(COL2)) return KEY_5X5[4 - row][1];
            if (!input(COL3)) return KEY_5X5[4 - row][2];
            if (!input(COL4)) return KEY_5X5[4 - row][3];
            if (!input(COL5)) return KEY_5X5[4 - row][4];
         }
      }
   }
   return 0;  // Không có phím nào du?c nh?n
}

#endif

