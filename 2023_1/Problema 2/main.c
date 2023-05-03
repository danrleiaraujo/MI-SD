#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <lcd.h>
#include "uartOrangePi.h"

/*-------------------------MACROS--------------------*/
/* Tabela de selecao de unidade*/
#define unidade_0 0b11000001
#define unidade_1 0b11000010
#define unidade_2 0b11000011
#define unidade_3 0b11000100
#define unidade_4 0b11000101
#define unidade_5 0b11000110
#define unidade_6 0b11000111
#define unidade_7 0b11001000
#define unidade_8 0b11001001
#define unidade_9 0b11001010
#define unidade_10 0b11001011
#define unidade_11 0b11001100
#define unidade_12 0b11001101
#define unidade_13 0b11001110
#define unidade_14 0b11001111
#define unidade_15 0b11010000
#define unidade_16 0b11010001
#define unidade_17 0b11010010
#define unidade_18 0b11010011
#define unidade_19 0b11010100
#define unidade_20 0b11010101
#define unidade_21 0b11010110
#define unidade_22 0b11010111
#define unidade_23 0b11011000
#define unidade_24 0b11011001
#define unidade_25 0b11011010
#define unidade_26 0b11011011
#define unidade_27 0b11011100
#define unidade_28 0b11011101
#define unidade_29 0b11011110
#define unidade_30 0b11011111
#define unidade_31 0b11100000
#define todas_unidades 0b11111110

/* Tabela de REQUISICAO*/
#define situacao_sensor  0b00000001
#define entrada_analogica  0b00010001
#define entrada_digital_0  0b00010010
#define entrada_digital_1  0b00010011
#define entrada_digital_2  0b00010100
#define entrada_digital_3  0b00010101
#define entrada_digital_4  0b00010110
#define entrada_digital_5  0b00010111
#define entrada_digital_6  0b00011000
#define entrada_digital_7  0b00011001
#define entrada_digital_8  0b00011010
#define entrada_digital_9  0b00011011
#define entrada_digital_10  0b00011100
#define entrada_digital_11  0b00011101
#define entrada_digital_12  0b00011110
#define entrada_digital_13  0b00011111
#define acende_led  0b00100001

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
// #define PA8 20
// #define PA9 22
/*-----------------------------------------------------*/

/*--------------------------Funções--------------------*/
void printaLCD(char linhaSup[], char linhaInf[]); //printa o lcd

/*---------------------------------------------------------------------------------*/
int main(){
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
