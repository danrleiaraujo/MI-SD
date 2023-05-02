#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <lcd.h>
#include "uartOrangePi.h"

/*-------------------------Constantes--------------------*/
/*  Pinos do LCD:
    D4   -> PG8 -> 32 Fisico -> 21 WPi
    D5   -> PG9 -> 36 Fisico -> 24 WPi
    D6   -> PG6 -> 38 Fisico -> 26 WPi
    D7   -> PG7 -> 40 Fisico -> 27 WPi
    E    -> PA18-> 28 Fisico -> 18 WPi
    RS   -> PA2 -> 22 Fisico -> 13 WPi
*/
#define LCD_D4 21
#define LCD_D5 24
#define LCD_D6 26
#define LCD_D7 27
#define LCD_E 18
#define LCD_RS 13

/*  Pinos do LED:
    PA8 -> 8 GPIO -> 20 WPi
    PA9 -> 9 GPIO -> 22 WPi
*/
#define PA8 20
#define PA9 22
/*-----------------------------------------------------*/

/*--------------------------Funções--------------------*/
void printaLCD(char linhaSup[], char linhaInf[]); //printa o lcd

/*---------------------------------------------------------------------------------*/
int main()
{
    wiringPiSetup();            // inicia biblioteca wiringPi
    pinMode(LCD_D4, OUTPUT);         // configura pino 1 como saida
    pinMode(LCD_D5, OUTPUT);         // configura pino 1 como saida
    pinMode(LCD_D6, OUTPUT);         // configura pino 1 como saida
    pinMode(LCD_D7, OUTPUT);         // configura pino 1 como saida
    pinMode(LCD_E, OUTPUT);         // configura pino 1 como saida
    pinMode(LCD_RS, OUTPUT);         // configura pino 1 como saida
    //pinMode(PA8, OUTPUT);         // configura pino 1 como saida
    //pinMode(PA9, OUTPUT);         // configura pino 1 como saida
    
    int estadoLED = 0;
     
    while(1){
        {
			printaLCD(/*Sensor*/,/*Dado recebido*/);    // imprime estado do LED
        }   
    }
    return 0;
}

void printaLCD(char dadoSup[],char dadoInf[]){ //ImpressÃ£o no lcd
        int lcd;
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

        lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
        lcdPrintf(lcd, "%s", dadoSup);

        lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
        lcdPrintf(lcd, "%s", dadoInf);
}
