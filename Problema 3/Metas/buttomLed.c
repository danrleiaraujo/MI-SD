// Código para ligar led com um buttom;
#include <stdio.h>
#include <wiringPi.h>
 
#define led  1               //definindo a pinagem da led;
#define botao 7              //definindo a pinagem do botão;

int main(){

    wiringPiSetup();                     // inicia biblioteca wiringPi
    pinMode(led, OUTPUT);                // configura pino da led como saida
    pinMode(botao, INPUT);               // configura pino do botão como entrada
    pullUpDnControl(botao, PUD_UP);      // configura resistor pull-up no pino 7
     
    int estadoLED = 0;                   //variável para a verificação da led 
 
    printf("Iniciando teste:\n");        //imprime mensagem na tela
     
    while(1){  //Looping enquanto for verdadeiro
        if(digitalRead(botao) == LOW){                        // detecta se a chave foi pressionada
                estadoLED = !estadoLED;                       // inverte o estado do LED
                digitalWrite(1, estadoLED);                   // controla o LED conforme estado

                /*imprime estado do LED*/
                if(estadoLED == 1)                               
                        printf("LED aceso\n");
                if(estadoLED == 0)    
                        printf("LED apagado\n");

                delay(20);                                    // aguarda 20 ms

                while(digitalRead(botao) == LOW);             // aguarda enquanto chave ainda esta pressionada           
                delay(20);                                    // aguarda 20 ms
        }   
    }
    return 0;

                /*--------------------------Teste com LED'S--------------------------------------
                if(digitalRead(dipSwitch0) == LOW){          // detecta se a chave foi acionada
                        estadoLED = 1;                       // estado do led é igual a 1 (liga)
                }
                else{
                        estadoLED = 0;                  // estado do led é igual a 0 (Desliga)
                }
                digitalWrite(1, estadoLED); //Manda o sinal pra led 
                /*-------------------------imprime estado do LED------------------------
                if(estadoLED == 1)                               
                        printf("LED aceso\n");
                if(estadoLED == 0)    
                        printf("LED apagado\n");
                delay(200);                                    // aguarda 200 ms
                ----------------------------------------------------------------------------*/


}