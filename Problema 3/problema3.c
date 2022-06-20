/* -------------------------------------------------------------------------------------------------------------------------------------            
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

------------------------------------------------------------------------------------------------------------------------------------------*/

#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/*-------------------PINAGENS------------------------*/
//LCD 
#define LCD_RS  6               //Register select pin
#define LCD_E   31               //Enable Pin
#define LCD_D4  26               //Data pin 4
#define LCD_D5  27               //Data pin 5
#define LCD_D6  28               //Data pin 6
#define LCD_D7  29               //Data pin 7
//DHT11
#define DHTPIN 5  //wiringPi = 8 -> 23 fisíco

//botao
#define botao1 21
#define botao2 24
#define botao3 25

//dipSwitch
#define dipSwitch0 7
#define dipSwitch1 0
#define dipSwitch2 2
#define dipSwitch3 3

#define led 23

//OUTRAS CONSTANTES
#define MAXTIMINGS 85 //máximo de tempo
#define LINHAS 10 //Quantidade de linhas para historico

/*--------------------Funções-------------------*/
void read_dht11_dat();
void printaLCD(char linhaSup[], char linhaInf[]);

/*---------------Struct - Histórico-------------*/
struct historico{
        int dht11_dat[5];
        char horario[9];
};
/*----------------------MAIN------------------------*/
int main(){
	//setlocale(LC_ALL,"Portuguese");

        int lcd, v =0;
        struct historico novoHist, atualHist;

        wiringPiSetup();                     // inicia biblioteca wiringPi
        pinMode(dipSwitch0, INPUT);                // configura pino da led como saida
        pinMode(dipSwitch1, INPUT);                // configura pino da led como saida
        pinMode(dipSwitch2, INPUT);                // configura pino da led como saida
        pinMode(dipSwitch3, INPUT);                // configura pino da led como saida
        pinMode(led, OUTPUT);                // configura pino da led como saida
        int estadoLED = 0;                   //variável para a verificação da led 
    
        //pullUpDnControl(botao, PUD_UP);      // configura resistor pull-up no pino 7
        
        while(1){  //Looping enquanto for verdadeiro
                if(digitalRead(dipSwitch0) == HIGH){          // detecta se a chave foi pressionada
                        estadoLED = 1;                       // inverte o estado do LED
                        digitalWrite(1, estadoLED); 
                
                //imprime estado do LED
                if(estadoLED == 1)                               
                        printf("LED aceso\n");
                if(estadoLED == 0)    
                        printf("LED apagado\n");

                delay(20);                                    // aguarda 20 ms
                estadoLED = 0;
                }   
        }
       

        /*while (1){ //Looping para leitura

                /*if (dipSwitch0 ==HIGH){
                        //read_dht11_dat();
                        printaLCD("Leu","Dht11");
                }

                //else if(dipSwitch1 == HIGH)
                        //read_lumi_pres();
                else{
                        printaLCD("PBL - 3","SISTEMAS DIGITAIS");
                }
                
                //novoHist.dht11_dat = 
                //read_dht11_dat(); //Chama a função do DHT11
        	//strftime(novoHist.horario);
                delay(1000);
        }*/

        return(0);
}


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
                //f = dht11_dat[2] * 9. / 5. + 32;
        
                /*Escrita no LCD*/
                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                lcdPrintf(lcd, "Umid: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);//Leitura de Umidade em %;

                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]); //Leitura em Celsius;
                

                /*Escrita no terminal*/
                printf("Umidade: %d.%d %%\n", dht11_dat[0], dht11_dat[1]); //Leitura de Umidade
                printf("Temperatura: %d.0 C \n", dht11_dat[2]); //Leitura em Celsius

        }
        //return dht11_dat;
}

void printaLCD(char dadoSup[],char dadoInf[]){ //Impressão no lcd
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

        lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
        lcdPrintf(lcd, "%s", dadoSup);

        lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
        lcdPrintf(lcd, "%s", dadoInf);
}

/*
int criarHist (historico novo){ //Cria arquivo para histórico
	FILE *file;
        file = fopen ("historico.txt","w"); // o ponteiro abre o arquivo com a funcão = "w" - criar ou anexar.
	if (file == NULL){
                printf("Erro ao criar o arquivo\n");
                return(0);
	}
	else{
                fprintf(file,"%d\t", novo.posicao);
                fprintf(file,"%s\t", novo.horario);
                fprintf(file,"%d", novo.dht11_dat[0]);
                fprintf(file,"%d", novo.dht11_dat[1]);
                fprintf(file,"%d", novo.dht11_dat[2]);
                fprintf(file,"%d", novo.dht11_dat[3]);
                fprintf(file,"%d\n", novo.dht11_dat[4]);
		printf("Arquivo Criado com sucesso\n");
	}
	fclose(file); //fecha a função.
	return (1);
}

int addHist (historico add){ //Add nova leitura no final do arquivo
	FILE *file;
        file = fopen ("historico.txt","at"); // o ponteiro abre o arquivo com a funcão = "a" - acrescentar.
	if (file == NULL){
                printf("Erro ao abrir o arquivo\n");
                return(0);
	}
	else{
                fprintf(file,"%d\t", add.posicao);
                fprintf(file,"%s\t", add.horario);
                fprintf(file,"%d", add.dht11_dat[0]);
                fprintf(file,"%d", add.dht11_dat[1]);
                fprintf(file,"%d", add.dht11_dat[2]);
                fprintf(file,"%d", add.dht11_dat[3]);
                fprintf(file,"%d\n", add.dht11_dat[4]);
		printf("Add no Arquivo com sucesso\n");
	}
	fclose(file); //fecha a função.
	return(1);
}

void lerHist (historico add){ // Lê arquivo com o hist. de leitura
        int i = 0;
        FILE *file;
        char conteudo[LINHAS*(16*2)];
        char *str[LINHAS];

        file = fopen("historico.txt", "rt"); 

        if (file == NULL) // Se nao conseguiu ler	
                printf("Problemas na leitura do arquivo\n");
        else
                fread(&conteudo, sizeof(char), (LINHAS*(16*2)), file);
        fclose(file);

        printf("%s",conteudo);

        //Separacao de strings = 
        for (i=0;i<LINHAS;i++){
                if(i ==0)
                        str[i] = strtok(conteudo,"\n");
                else{
                        str[i] = strtok(NULL,"\n");
                }
        }
        return *str;
}
*/