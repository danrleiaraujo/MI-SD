/*      Funções para rodar:
        gcc -o problema3 problema3.c -lwiringPi -lwiringPiDev
        sudo ./problema3
*/

#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//USE WIRINGPI PIN NUMBERS
#define LCD_RS  6               //Register select pin
#define LCD_E   31               //Enable Pin
#define LCD_D4  26               //Data pin 4
#define LCD_D5  27               //Data pin 5
#define LCD_D6  28               //Data pin 6
#define LCD_D7  29               //Data pin 7


int dht11_dat[5] = {0, 0, 0, 0, 0}; //dados do dht11
#define MAXTIMINGS 85 //maximo de tempo
#define DHTPIN 4  //pinagem do dht11 



void read_dht11_dat(){
        uint8_t laststate = HIGH, counter = 0, j = 0, i; //int sem sinal de 8 bits
        float f; 
        int lcd; 

        dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

        pinMode(DHTPIN, OUTPUT);
        digitalWrite(DHTPIN, LOW);
        delay(18);
        
        digitalWrite(DHTPIN, HIGH);
        delayMicroseconds(40);
        
        pinMode(DHTPIN, INPUT);
        for (i = 0; i < MAXTIMINGS; i++){
                counter = 0;
                while (digitalRead(DHTPIN) == laststate){ 
                        counter++;
                        delayMicroseconds(1);
                        if (counter == 255)
                        {
                                break;
                        }
                }
                laststate = digitalRead(DHTPIN);

                if (counter == 255)
                        break;

                if ((i >= 4) && (i % 2 == 0))
                {
                        dht11_dat[j / 8] <<= 1;
                        if (counter > 16)
                                dht11_dat[j / 8] |= 1;
                        j++;
                }
         }

        if ( (j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))){
                f = dht11_dat[2] * 9. / 5. + 32;
        
                lcdPosition(lcd, 0, 0);
                lcdPrintf(lcd, "Humidity: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);

                lcdPosition(lcd, 0, 1);
                lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]); //Celsius
                //lcdPrintf(lcd, "Temp: %.1f F", f); //Fahrenheit

                printf("Humidity: %d.%d %%\n", dht11_dat[0], dht11_dat[1]); //humidade
                printf("Temp: %d.0 C \n", dht11_dat[2]); //Celsius

        }
}

int main(void)
{
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
        
 /*     lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "Danrlei e Evelyn\n");
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "Hello World"); */
        
        while (1){
                read_dht11_dat();
                delay(1000);
        }

        return(0);
}