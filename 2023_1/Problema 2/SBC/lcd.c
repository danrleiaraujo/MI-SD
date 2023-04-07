#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

/*        Funções para rodar:
    gcc -o problema3 problema3.c -lwiringPI -lwiringPiDev

    Execucao:
        sudo ./problema3 
*/

/*-------------------------Constantes--------------------*/
/*  Pinos do LCD:
    D4   -> PG8 -> 32 Fisico -> 21 WPi
    D5   -> PG9 -> 36 Fisico -> 24 WPi
    D6   -> PG6 -> 38 Fisico -> 26 WPi
    D7   -> PG7 -> 40 Fisico -> 27 WPi
    E    -> PA18-> 28 Fisico -> 18 WPi
    RS   -> PA2 -> 22 Fisico -> 13 WPi
*/
#define LCD_D4 21;
#define LCD_D5 24;
#define LCD_D6 26;
#define LCD_D7 27;
#define LCD_E 18;
#define LCD_RS 13;

/*  Pinos do LED:
    PA8 -> 8 GPIO -> 20 WPi
    PA9 -> 9 GPIO -> 22 WPi
*/
#define PA8 20;
#define PA9 22;
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
    
    int estadoLED = 0;
     
    while(1){
        {
            estadoLED = !estadoLED;                      // inverte o estado do LED
            digitalWrite(PA8, estadoLED);                  // controla o LED conforme estado
            digitalWrite(PA9, estadoLED);                  // controla o LED conforme estado
            if(estadoLED == 1) printf("LED em sinal alto\n");    // imprime estado do LED
            if(estadoLED == 0) printf("LED em sinal baixo\n");  // imprime estado do LED
            delay(1000); // Espera 1000ms = 1s       
        }   
    }
    return 0;
}

void printaLCD(char dadoSup[],char dadoInf[]){ //ImpressÃ£o no lcd
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

        lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
        lcdPrintf(lcd, "%s", dadoSup);

        lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
        lcdPrintf(lcd, "%s", dadoInf);
}