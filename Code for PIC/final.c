#include <18F4550.h>
#fuses HS, NOWDT, NOLVP, NOMCLR
#use delay(clock=20000000)

#define LCD_ENABLE_PIN  PIN_B0
#define LCD_RS_PIN      PIN_B1
#define LCD_RW_PIN      PIN_B2
#define LCD_DATA4       PIN_B4
#define LCD_DATA5       PIN_B5
#define LCD_DATA6       PIN_B6
#define LCD_DATA7       PIN_B7

#include <lcd.c>
#include <math.h>
#include "keypad_5x5.c"

float result = 0;
float current_input = 0;
float previous_value = 0;
char current_op = 0;
int1 calculated = 0;
int1 lcd_enabled = 0;
int1 in_function = 0;
char function_mode = 0;

void reset_calc() {
   lcd_putc('\f');
   current_input = 0;
   previous_value = 0;
   current_op = 0;
   calculated = 0;
   in_function = 0;
   function_mode = 0;
   result=0;
}


float apply_function(char func, float val) {
   switch(func) {
      case 's': return sin(val * 3.141592 / 180.0);
      case 'c': return cos(val * 3.141592 / 180.0);
      case 't': return tan(val * 3.141592 / 180.0);
      case 'q': return sqrt(val);
      case 'l': return log10(val);
   }
   return val;
}

float compute(float a, float b, char op) {
   switch(op) {
      case '+': return a + b;
      case '-': return a - b;
      case '*': return a * b;
      case '/': return (b != 0) ? a / b : 0;
      case '^': return pow(a, b);
   }
   return b;
}

void show_result(float val) {
   lcd_gotoxy(1, 2);
   printf(lcd_putc, "=%.2f", val);
}

void main() {
   char key;
   set_tris_b(0b00000100);
   output_low(PIN_B3);
   lcd_init();
   key_5x5_init();
   reset_calc();
   //lcd_putc("Press ` to start");

   while(TRUE) {
      key = get_key_5x5();

      if(key != 0) {
         if(!lcd_enabled && key == '`') 
         {
            output_high(PIN_B3);
            lcd_enabled = 1;
            reset_calc();
            lcd_putc("Ready");
            delay_ms(400);
            lcd_putc('\f');
         }
         else if(lcd_enabled) {
            if(key >= '0' && key <= '9') {
            if(calculated) {
               reset_calc();         // xoá phép tính cu
               //lcd_enabled = 1;      
               }
               current_input = current_input * 10 + (key - '0');
               printf(lcd_putc, "%c", key);
               }
            else if(key == 'a') {
            if(calculated) {
               lcd_putc('\f');
               current_input = result;
               lcd_putc("Ans");
               //lcd_enabled = 1;      
               }
             if(!calculated)
             {
               current_input = result;
               lcd_putc("Ans");
             }
            }
            else if(key == '#' || key == 'm') {
               reset_calc();
            }
            else if(key == '=') {
               if(in_function) {
                  current_input = apply_function(function_mode, current_input);
                  in_function = 0;
                  function_mode = 0;
               }
            
               if(current_op != 0) {
                  result = compute(previous_value, current_input, current_op);
               } else {
                  result = current_input;
               }
               if (result > 21474834.0) {
                  lcd_putc('\f');
                  lcd_gotoxy(1,1);
                  lcd_putc("Out of range");
                  lcd_gotoxy(1,2);
                  lcd_putc("Max is 21474836");
               }
               if( result< -21474836.0)
               {
                  lcd_putc('\f');
                  lcd_gotoxy(1,1);
                  lcd_putc("Out of range");
                  lcd_gotoxy(1,2);
                  lcd_putc("Min is -21474836");
               }
               if ((result < 21474834.0) && (result > -21474836.0)) {
                  show_result(result);
               }
               calculated = 1;
               current_input = 0;
               current_op = 0;
            }
            
            else if(key == '+' || key == '-' || key == '*' || key == '/' || key == '^') {
                  if(in_function) {
                     current_input = apply_function(function_mode, current_input);
                     in_function = 0;
                     function_mode = 0;
                  }
               
                  if(current_op != 0) {
                     previous_value = compute(previous_value, current_input, current_op);
                  } else if(calculated) {
                     previous_value = result;
                     lcd_putc('\f');
                     lcd_putc("Ans");
                  } else {
                     previous_value = current_input;
                  }
               
                  current_input = 0;
                  current_op = key;
                  calculated = 0;
               lcd_putc(key);                 
               }
                     
                     else if(key == 's' || key == 'c' || key == 't' || key == 'q' || key == 'l') {
                     if(in_function) {
                     current_input = apply_function(function_mode, current_input);
                     in_function = 0;
                     function_mode = 0;
                  }
                        if(calculated) {
                           reset_calc();
                        }
                     
                        current_input = 0;
                        in_function = 1;
                        function_mode = key;
                     
                        switch(key) 
                        {
                           case 's': lcd_putc("sin"); break;
                           case 'c': lcd_putc("cos"); break;
                           case 't': lcd_putc("tan"); break;
                           case 'q': lcd_putc("sqrt"); break;
                           case 'l': lcd_putc("log"); break;
                        }                      
                     }                                
         }

         while(get_key_5x5() != 0); 
      }
   }
}

