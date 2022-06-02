/* -----------------------------------------------------------------------------------------------------------------------------            
        Problema 3: 
                Danrlei Almeida Araujo
                Evelyn Suzarte Fernandes

        * Leitura do DHT11;
        * Escrita no LCD;

        Funções para rodar:
                Compilação:
                        gcc -o problema3 problema3.c -lwiringPi -lwiringPiDev

                Execução:
                        sudo ./problema3        

                Para mapeamento dos pinos:
                        gpio readall  

---------------------------------------------------------------------------------------------------------------------------------*/

#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//PINAGEM WIRINGPI PARA LCD 
#define LCD_RS  6               //Register select pin
#define LCD_E   31               //Enable Pin
#define LCD_D4  26               //Data pin 4
#define LCD_D5  27               //Data pin 5
#define LCD_D6  28               //Data pin 6
#define LCD_D7  29               //Data pin 7

#define DHTPIN 5  //pinagem do dht11 
#define MAXTIMINGS 85 //máximo de tempo




void read_dht11_dat(){
        int lcd, dht11_dat[5] = {0, 0, 0, 0, 0}; //dados do dht11 
        uint8_t laststate = HIGH, counter = 0, j = 0, i; //int sem sinal de 8 bits
        float f; 

        //Zerando o vetor de dados do DHT11:
        dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

        /*Pinagem DHT11*/
        pinMode(DHTPIN, OUTPUT);        // Declarando a pinagem do DHT11 como saída

        digitalWrite(DHTPIN, LOW);      // Colocando o valor como baixo
        delay(18);                      // Espera 18 ms
        
        digitalWrite(DHTPIN, HIGH);     // Colocando o valor como alto
        delayMicroseconds(40);          // Espera 18 ms
        
        pinMode(DHTPIN, INPUT);         // Declarando a pinagem do DHT11 como entrada

        for (i = 0; i < MAXTIMINGS; i++){
                counter = 0; //inicializa o contador em 0;

                while (digitalRead(DHTPIN) == laststate){   //Enquanto a leitura digital for igual ao ultimo estado:
                        counter++; //Acrescenta 1 no contador;
                        delayMicroseconds(1); //Espera 1 microsegundo;

                        if (counter == 255){ //Caso o contador for igual á 255
                                break;
                        }

                }

                laststate = digitalRead(DHTPIN);  //Sobrescreve o ultimo estado com a leitura da pinagem do dht11;

                if (counter == 255){ //Caso o contador for igual á 255
                        break;
                }

                if ((i >= 4) && (i % 2 == 0)){                  //  Se i for maior ou igual a que 4 e o resto de divisão de i por 2 for 0 faça:
                        dht11_dat[j / 8] <<= 1;                 //  <<=	atribuição com deslocamento à esquerda	
                        if (counter > 16)                       //  caso o contador for maior que 16:
                                dht11_dat[j / 8] |= 1;          //  |=	atribuição com OR binário
                        j++;
                }
         }

        if ( (j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))){ 
        // Se j for maior ou igual à 40 e o indice 4 do vetor for igual ao indice 0 + indice 1 + indice 2 + indice 3 AND bit-a-bit 0xFF faça:
                f = dht11_dat[2] * 9. / 5. + 32;
        
                /*Escrita no LCD*/
                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                lcdPrintf(lcd, "Humidity: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);//Leitura de Umidade;
                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]); //Leitura em Celsius;

                /*Escrita no terminal*/
                printf("Humidity: %d.%d %%\n", dht11_dat[0], dht11_dat[1]); //Leitura de Umidade
                printf("Temp: %d.0 C \n", dht11_dat[2]); //Leitura em Celsius

        }
}

int main(void)
{
        int lcd;
        wiringPiSetup(); // inicia biblioteca wiringPi
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
        
        while (1){ //Looping para leitura
                read_dht11_dat(); //Chama a função do DHT11
                delay(1000);
        }

        return(0);
}