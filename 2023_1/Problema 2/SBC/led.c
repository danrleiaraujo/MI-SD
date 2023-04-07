#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

/*-------------Constantes-----------*/
//  Voltagens:
#define HIGH 1;
#define LOW 0;

//  Pinos:
//PA8 -> 8 GPIO -> 20 WPi
#define PA8 20;

//PA9 -> 9 GPIO -> 22 WPi
#define PA9 22;
/*---------------------------------*/
int main()
{
    wiringPiSetup();            // inicia biblioteca wiringPi
    pinMode(PA8, OUTPUT);         // configura pino 1 como saida
    pinMode(PA9, OUTPUT);         // configura pino 1 como saida
    
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