/* -------------------------------------------------------------------------------------------------------------------------------------            
        Problema 3: 
                Danrlei Almeida Araujo
                Evelyn Suzarte Fernandes

        * Leitura do DHT11;
        * Escrita no LCD;

        Funções para rodar:
                Compilação:
                        gcc -o problema3 problema3.c ads1115_rpi.c -lwiringPI -lwiringPiDev

                Execução:
                        sudo ./problema3        

                Para mapeamento dos pinos:
                        gpio readall  
------------------------------------------------------------------------------------------------------------------------------------------*/

#include <wiringPi.h>
#include <lcd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <stdint.h>

#include <time.h>

#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ads1115_rpi.h"

typedef struct {
        char linha[100];
        int nLinha;
        int dht11[5];
        float pres, lumi;
}historico;

/*-------------------PINAGENS------------------------*/
//LCD 
#define LCD_RS  6               //Register select pin
#define LCD_E   31               //Enable Pin
#define LCD_D4  26               //Data pin 4
#define LCD_D5  27               //Data pin 5
#define LCD_D6  28               //Data pin 6
#define LCD_D7  29               //Data pin 7

//DHT11
#define DHTPIN 4  //wiringPi = 8 -> 23 fisíco

//botao
#define botao1 21
#define botao2 24
#define botao3 25

//dipSwitch
#define dipSwitch0 7
#define dipSwitch1 0
#define dipSwitch2 2
#define dipSwitch3 3

//OUTRAS CONSTANTES
#define MAXTIMINGS 85 //máximo de tempo
#define LINHAS 10 //Quantidade de linhas para historico


/*--------------------Funções-------------------*/
void printaLCD(char linhaSup[], char linhaInf[]); //printa o lcd

void criarHist(historico hist[]); //cria o hist em um txt

void addHist(historico hist[]); //adiciona no final do arq txt

void carregarHistLP (historico hist[]); //Lê o historico da pressao e luminosidade
void carregarHistDht11 (historico hist[]); //Lê o historico do dht11

void atualizaHistPL (historico hist[],float pres, float lumi); //Atualiza oque leu do arq.
void atualizaHistDht11 (historico hist[], int dht11[]); //Atualiza oque leu do arq.

void read_pres_lumi (historico novaLeitura[]); //Le pressao e luminosidade
void read_dht11_dat(); //Lê o dht11


/*----------------------MAIN------------------------*/
int main(){
        historico *novaLeitura, *histDht11, *histPL;
        int lcd, v =0, tam_alloc=100, b = 0;
        wiringPiSetup();                     // inicia biblioteca wiringPi
        novaLeitura = (historico *) malloc (tam_alloc*sizeof(historico));
        histDht11 = (historico *) malloc (tam_alloc*sizeof(historico));
	histPL = (historico *) malloc (tam_alloc*sizeof(historico));

        /*Configurando os pinos do switch como entrada*/
        pinMode(dipSwitch0, INPUT);               
        pinMode(dipSwitch1, INPUT);                
        pinMode(dipSwitch2, INPUT);               
        pinMode(dipSwitch3, INPUT); 

        carregarHistLP(histPL);
        carregarHistDht11(histDht11);
        while(1){  //Looping enquanto for verdadeiro

                if (digitalRead(dipSwitch0) == LOW){
                        read_dht11_dat ();
                        printf("Leu DHT11\n");
                        // atualizaHistDht11 (histDht11, novaLeitura[0].dht11);
                        //criarHist (histDht11);
                        delay(1000);
                }
                else if(digitalRead(dipSwitch1)  == LOW){
                        printf("Leu pressao e luminosidade\n");
                        read_pres_lumi(novaLeitura);
                        atualizaHistPL(histPL, novaLeitura[0].pres, novaLeitura[0].lumi);
                        delay(1000);
                }
                else if(digitalRead(dipSwitch2)  == LOW){                        
                        printf("Histórico Luminosidade e Pressao\n");
                        printaLCD("Lumi e Press:", histPL[b].linha);
                        if(digitalRead(botao1) == LOW && b > 0 ){  
                                b--;
                        }
                        else if(digitalRead(botao2) == LOW && b > 9){
                                b++;
                        }
                        else if(b == 10){
                                b = 0;
                        }
                        delay(1000);

                }
                else if(digitalRead(dipSwitch3)  == LOW){
                        printf("Histórico DHT11\n");
                        printaLCD(histDht11[b].nLinha, histDht11[b].linha);
                        if(digitalRead(botao1) == LOW && b > 0 ){  
                                b--;
                        }
                        else if(digitalRead(botao2) == LOW && b > 9){
                                b++;
                        }
                        else if(b == 10){
                                b = 0;
                        }
                        delay(1000);
                }
                
                else{
                        printaLCD("PBL - 3","SistemasDigitais");
                }

                delay(1000);
        /*------------------------------------------------------------------------------------------*/
        }
	free(novaLeitura);
	free(histDht11);
	free(histPL);
        return(0);
}

/*-----------------------Função para ler o DHT11-----------------------------------*/
void read_dht11_dat (){
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
        if ( (j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ) { 
        /* Se j for maior ou igual à 40 e o indice 4 do vetor for igual ao indice 0 + indice 1 + indice 2 + indice 3 
        AND bit-a-bit 0xFF faça:*/
                //f = dht11_dat[2] * 9. / 5. + 32;
                
                /*Escrita no LCD*/
                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                lcdPrintf(lcd, "Umid: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);//Leitura de Umidade em %;
                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]); //Leitura em Celsius;
                

                /*Escrita no terminal*/
                printf("Umidade: %d.%d %%\n", dht11_dat[0], dht11_dat[1]); //Leitura de Umidade
                printf("Temperatura: %d.0 C \n", dht11_dat[2]); //Leitura em Celsius
                
                /*novaLeitura[0].dht11[0] = dht11_dat[0];
                novaLeitura[0].dht11[1] = dht11_dat[1];
                novaLeitura[0].dht11[2] = dht11_dat[2];
                novaLeitura[0].dht11[3] = dht11_dat[3];
                novaLeitura[0].dht11[4] = dht11_dat[4];*/
                delay (1000);
        }
        //return dht11_dat;
}

/*----------------Função para ler o imprimir no lcd------------------*/
void printaLCD(char dadoSup[],char dadoInf[]){ //Impressão no lcd
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

        lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
        lcdPrintf(lcd, "%s", dadoSup);

        lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
        lcdPrintf(lcd, "%s", dadoInf);
}

/*------------------Função para Criar o historico------------------*/
void criarHist (historico novo[]){ //Cria arquivo para histórico
	FILE *file;
        int x=0, i=0;
        file = fopen ("historico.txt","w"); // o ponteiro abre o arquivo com a funcão = "w" - criar ou anexar.
	if (file == NULL){
                printf("Erro ao criar o arquivo\n");
                return;
	}
	else{
                while(novo[i].nLinha == i){
                        i++;
                }

                while(x < i){
                        fprintf(file,"%s", novo[x].linha);
                        x++;
                }
                        printf("Arquivo Criado com sucesso\n");
	}
	fclose(file); //fecha a função.
}
/*------------------Função que adiciona no final do arq------------------*/
 void addHist (historico novo[]){ //Add nova leitura no final do arquivo
	FILE *file;
        int x=0, i=0;
        file = fopen ("historico.txt","at"); // o ponteiro abre o arquivo com a funcão = "a" - acrescentar.
	if (file == NULL){
                printf("Erro ao abrir o arquivo\n");
                return;
	}
	else{
                while(novo[i].nLinha == i){
                        i++;
                }
                fprintf(file,"%s", novo[i].linha);
	}
	fclose(file); //fecha a função.
}
/*------------------Função que lê o arq de histDht11------------------*/
void carregarHistDht11 (historico hist[]){ 
	int i = 0;
	FILE *file;
	file = fopen("historicodht11.txt", "r"); 

	if (file == NULL){ // Se nao conseguiu ler	
	    printf("Problemas na leitura do arquivo\n");
	}
	else{	
		while (feof(file) == 0){
			fgets (hist[i].linha, 100, file);
			hist[i].nLinha = i;
			i++;
		}
	}
	fclose(file);
}
/*------------------Função que lê o arq de hist da Lumi e Press------------------*/
void carregarHistLP (historico hist[]){ 
	int i = 0;
	FILE *file;
	file = fopen("historicolp.txt", "r"); 

	if (file == NULL){ // Se nao conseguiu ler	
	    printf("Problemas na leitura do arquivo\n");
	}
	else{	
		while (feof(file) == 0){
			fgets (hist[i].linha, 100, file);
			hist[i].nLinha = i;
			i++;
		}
	}
	fclose(file);
}
/*------------------Função que atualiza oq foi lido------------------*/
void atualizaHistPL (historico hist[], float pres, float lumi){
	int i=0, tam_alloc=100;
        char medidaLumi[] = " W", divisor[] = "_", medidaPres[]=" Pa";

        char linhaNova[16], pressao[5][5], lumin[5][5], conversao;
        historico *novoHist;
	novoHist = (historico *) malloc (tam_alloc*sizeof(historico)); //aloca espaço
	
        memset(linhaNova, 0, 16); //Limpa a variavel para criação;
	
	//Convencao de float pra char
        sprintf(pressao, "%.2f",pres);
        sprintf(lumin, "%.2f",lumi);
		
	//Confeccao da nova linha
	strcpy(linhaNova,lumin);
	strcpy(linhaNova,medidaLumi);
	strcpy(linhaNova,divisor);
	strcpy(linhaNova,pressao);
	strcpy(linhaNova,medidaPres);
	strcat(linhaNova,"\n");
	   
	//Salva na aux
        for (i=0;i<10;i++){
    	        if(i == 9){
			strcpy(novoHist[i].linha, linhaNova);
			novoHist[i].nLinha = i;
		}
		else{
			strcpy(novoHist[i].linha, hist[i+1].linha);
			novoHist[i].nLinha = i;
		}
        }
	    
        //Salva de volta na variavel hist
	for (i=0;i<10;i++){
                strcpy(hist[i].linha, novoHist[i].linha);
        }
    
	free(novoHist); //libera o espaco da auxiliar
	
        return;
}
void atualizaHistDht11 (historico hist[], int dht11_dat[]){
	int i=0, tam_alloc=100;
	char umidade[] = "UR ", divisor[]=".", percento[] = "% ", temperatura[] = "T ", celcius[] = "0C";
        char linhaNova[16], dht11[5][5], conversao;
        historico *novoHist;
	novoHist = (historico *) malloc (tam_alloc*sizeof(historico)); //aloca espaço
	memset(linhaNova, 0, 16); //Limpa a variavel para criação;
	
	//Convencao de int pra char
	for(i = 0; i<5; i++)
		sprintf(dht11[i], "%d", dht11_dat[i]);
		
	//Confeccao da nova linha
	strcpy(linhaNova,umidade);
	strcat(linhaNova,dht11[0]);
	strcat(linhaNova,divisor);
	strcat(linhaNova,dht11[1]);
	strcat(linhaNova,percento);
	strcat(linhaNova,temperatura);
	strcat(linhaNova,dht11[2]);
	strcat(linhaNova,divisor);
	strcat(linhaNova,celcius);
	strcat(linhaNova,"\n");
	   
	//Salva na aux
        for (i=0;i<10;i++){
    	        if(i == 9){
			strcpy(novoHist[i].linha, linhaNova);
			novoHist[i].nLinha = i;
		}
		else{
			strcpy(novoHist[i].linha, hist[i+1].linha);
			novoHist[i].nLinha = i;
		}
        }
	    
        //Salva de volta na variavel hist
	for (i=0;i<10;i++){
                strcpy(hist[i].linha, novoHist[i].linha);
        }
    
	free(novoHist); //libera o espaco da auxiliar
	
        return;
}

void read_pres_lumi (historico novaLeitura[]) {
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

	if(openI2CBus("/dev/i2c-1") == -1){
		return;
	}

        setI2CSlave(0x48);

        printf("Luminosidade= %.2f W", readVoltage(0));
        printf("Pressao = %.2f Pa\n", readVoltage(1));

        novaLeitura[0].lumi = readVoltage(0);
        novaLeitura[0].pres = readVoltage(1);

        lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
        lcdPrintf(lcd, "Luminosidade= %.2f W", readVoltage(0) ); //medida em Watts

        lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
        lcdPrintf(lcd, "Pressao = %.2f Pa", readVoltage(1) );  //Medida em Pascal
}