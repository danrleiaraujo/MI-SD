//Código para impressão na LCD:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <lcd.h>

// PINAGEM WIRINGPI PARA LCD;
#define LCD_RS  6               //Register select pin
#define LCD_E   31               //Enable Pin
#define LCD_D4  26               //Data pin 4
#define LCD_D5  27               //Data pin 5
#define LCD_D6  28               //Data pin 6
#define LCD_D7  29               //Data pin 7


int main(void){
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

        lcdPosition(lcd, 0, 0); //Linha superior
        lcdPrintf(lcd, "Danrlei e Evelyn\n");
        lcdPosition(lcd, 0, 1); //Linha inferior
        lcdPrintf(lcd, "Hello World"); 
}