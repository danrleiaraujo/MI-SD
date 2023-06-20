#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h> // Uso da  GPIO
#include <lcd.h> // http://wiringpi.com/dev-lib/lcd-library/
#include <unistd.h> // para usar o sys
#include <fcntl.h> // A biblioteca deve definir as seguintes solicitações e argumentos para uso pelas funções fcntl () e open () .
#include <termios.h> // A biblioteca contém as definições usadas pelas interfaces de E/S do terminal 
#include <time.h>   // Uso da funcao delay
#include <string.h> //Manipulacao de string
#include <locale.h>
#include "MQTTClient.h"

/*-------------------------MACROS--------------------*/
/* Tabela de selecao de unidade*/
#define unidade_1 0b11000001
#define unidade_2 0b11000010
#define unidade_3 0b11000011
#define unidade_4 0b11000100
#define unidade_5 0b11000101
#define unidade_6 0b11000110
#define unidade_7 0b11000111
#define unidade_8 0b11001000
#define unidade_9 0b11001001
#define unidade_10 0b11001010
#define unidade_11 0b11001011
#define unidade_12 0b11001100
#define unidade_13 0b11001101
#define unidade_14 0b11001110
#define unidade_15 0b11001111
#define unidade_16 0b11010000
#define unidade_17 0b11010001
#define unidade_18 0b11010010
#define unidade_19 0b11010011
#define unidade_20 0b11010100
#define unidade_21 0b11010101
#define unidade_22 0b11010110
#define unidade_23 0b11010111
#define unidade_24 0b11011000
#define unidade_25 0b11011001
#define unidade_26 0b11011010
#define unidade_27 0b11011011
#define unidade_28 0b11011100
#define unidade_29 0b11011101
#define unidade_30 0b11011110
#define unidade_31 0b11011111
#define unidade_32 0b11100000

// Mensagem de erro.
#define erro 0b11111111

/* Tabela de REQUISICAO*/
#define situacao_atual  0b00000001
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

/* -- Menu -- */
#define next 25
#define enter 23
#define previous 19

/*-- MQTT --*/
#define ADDRESS     "tcp://10.0.0.101:1883@@luno*123"
#define CLIENTID    "SBC"
#define TOPICPUB    "requisicoes"
#define TOPICSUB    "respostas"
#define TOPICFRONT  "front"
#define TOPICFRONT  "front"
#define QOS         1
#define TIMEOUT     10000L
#define USERNAME	"aluno"
#define PASSWORD	"@luno*123"

/* ================================== MQTT ========================================*/
/*-------------- Criacao do objeto -----------*/
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int rc;
char respostaMQTT[100] = "0";
/*-----------------------------------------------------*/

/*--------------------------Funcoes--------------------*/
/* ======================== LCD ==================== */
void printaLCD(char linhaSup[], char linhaInf[], int lcd); //printa o lcd
void printaLCDInt(char linhaSup[], int valorAnalog, int lcd); //printa o lcd
void printaLCDHexa(char dadoSup[],char dadoInf, int lcd); //printa o lcd
/* ======================== MQTT ==================== */
void publisher (char topico[], char msg[]);
void subscriber (void);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
volatile MQTTClient_deliveryToken deliveredtoken;
/* ======================== UART =================== */
void writeUart (int uart0_filestream, unsigned char dado);
void readUart (int uart0_filestream, unsigned char resposta[]);
/* ================ Auxiliares do menu ============== */
void atualizaLCD (char fraseSup[], char fraseInf[], int lcd);
void atualizaLCDVetor (char fraseSup[], char fraseInf[], int valor[], int lcd);
void nextValor(int v[]);
void previousValor(int v[]);
void limpaVetor(unsigned char v[], int tamanho);
 /* ===================================================*/
/*-------------------------------------MAIN------------------------------------------------------------*/
int main(){
    wiringPiSetup();            // inicia biblioteca wiringPi
    /*======================== CONFIGURACAO DE PINOS =====================================*/
    /*=============================== OUTPUT ============================================*/
    pinMode(LCD_D4, OUTPUT);         // configura pino como saida
    pinMode(LCD_D5, OUTPUT);         // configura pino como saida
    pinMode(LCD_D6, OUTPUT);         // configura pino como saida
    pinMode(LCD_D7, OUTPUT);         // configura pino como saida
    pinMode(LCD_E, OUTPUT);         // configura pino como saida
    pinMode(LCD_RS, OUTPUT);         // configura pino como saida
    /*=============================== INPUT ============================================*/
    pinMode(next, INPUT);           // configura pino como entrada
    pinMode(previous, INPUT);           // configura pino como entrada
    pinMode(enter, INPUT);           // configura pino como entrada
	/*==================================================================================*/
    /*============================== Variaveis =================================*/
    /*Type: int*/
	int valor[5] = {0,0,0,1}; //Dezena, Unidade, Dezena Futura, Unidade Futura;
    int opcoesSensores = 0, sensoresDigitais = 0;
	int op = 0, op2 = 0, unid = 0, lcd, valorAnalog = 0;
    int unidadeSelecionada = 0, opcaoSelecionada = 0;
    int valorLED, mqtt=0; 
    int /*n = 0, j=0,*/ i = 0, qtdOnline = 0, qtdOnlineMqtt = 0,tds_unidades = 0;   //Auxiliares
    /*Type: char*/
	char uniSel[16] = "UniSelecionada", opSel[16] = "OpSelecionada";
	char unidade[16]= "Unidade = ";
    char opcao0[16] = "Situacaoatual", opcao1[16]="ValorAnalogic"; 
    char opcao2[16] = "ValorDigital", opcao3[16] ="Acende_Led", opcao4[16] ="Monitoramento";
    char u, d, msg[16], entradaD[10];;
    /*Type: unsigned char*/
    unsigned char online[32] ={}, resposta[8];
    char onlineMqtt[32][100] ={};
    unsigned char codigo, codigoUni, dest[3];
    /*Type: outros*/
    time_t data_hora_segundos; // guarda os segundos deste 01/01/1970
    struct tm *timeinfo; // declara uma estrutura tm
    
    /* Vetor com os codigos de unidades*/
    unsigned char codigo_unidades[32] = {   
		unidade_1, unidade_2, unidade_3, unidade_4, unidade_5, unidade_6, unidade_7, unidade_8,
		unidade_9, unidade_10, unidade_11, unidade_12, unidade_13, unidade_14, unidade_15, unidade_16,
		unidade_17, unidade_18, unidade_19, unidade_20, unidade_21, unidade_22, unidade_23, unidade_24,
		unidade_25, unidade_26, unidade_27, unidade_28, unidade_29, unidade_30, unidade_31, unidade_32
	};
    /* Vetor com os codigos de unidades em String */
    char codigo_unidade[32][9] = {
        {"11000001"}, {"11000010"}, {"11000011"}, {"11000100"}, {"11000101"}, {"11000110"},
        {"11000111"}, {"11001000"}, {"11001001"}, {"11001010"}, {"11001011"}, {"11001100"},
        {"11001101"}, {"11001110"}, {"11001111"}, {"11010000"}, {"11010001"}, {"11010010"},
        {"11010011"}, {"11010100"}, {"11010101"}, {"11010110"}, {"11010111"}, {"11011000"},
        {"11011001"}, {"11011010"}, {"11011011"}, {"11011100"}, {"11011101"}, {"11011110"},
        {"11011111"}, {"11100000"}
	}; 
    char respostas[6][8] = {   
        {"0x01"}, {"0x02"}, {"0x11"}, {"0x21"},{"0x00"}
	}; 
	/*==================================================================================*/
    /* ---------------- Abertura e configuracao do arquivo da UART -------------------- */
    int uart0_filestream = -1; // Retorno de erro da função Open - 
    /*
        FLAGS:
        O_RDWR -> Lê e escreve
        O_NOCTTY -> Identifica o dispositivo como dispositivo de terminal
        O_NDELAY -> Sem delay, para acesso imediato
    */
    //Para o open usaremos a uart 3: /dev/ttyS3
    uart0_filestream = open("/dev/ttyS3", O_RDWR | O_NOCTTY | O_NDELAY);  

    if (uart0_filestream == -1){ //Verifica se deu erro na abertura da UART
        printf("Erro na abertura da UART\n");
    }
    else{
        printf("Abertura do arquivo ttyS3 com êxito\n");
    }
    /* ================================== UART ======================================== */
    /* -------------------------- Configuração da UART -------------------------------- */
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //BaudRate, tamanho da palavra (CSIZE) -> 9600 8-N-1
    options.c_iflag = IGNPAR; //ignora a paridade
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIOFLUSH); // Limpeza de dados recebidos e não lidos pela serial
    tcsetattr(uart0_filestream, TCSANOW, &options); // Faz com que seja feita as mudancas de imediato

    /*============================== INICIA LCD =================================*/
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
	/*==================================================================================*/
    /*============================== BEM VINDO =================================*/
    printaLCD("Menu-TEC499-P2", "Escolha a Opcao:", lcd);    // imprime estado do LED]
    delay(2000); // Espera 2 segundos para iniciar
    lcdClear(lcd); // Limpa o LCD
	/*==================================================================================*/
    /* ================================== MQTT ========================================*/
    /*----------------------------- Criando o client ----------------------------------*/
    MQTTClient_create (&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    /* =============================================================================== */
    /*-------------------------------- Conexao ----------------------------------------*/
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
	conn_opts.username = USERNAME;
	conn_opts.password = PASSWORD;
    MQTTClient_setCallbacks(client, NULL, NULL, msgarrvd, NULL);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
        printf("Falha para se conectar\n");
    }
    subscriber();
    /*============================== PROGRAMA PRINCIPAL =================================*/
    while(1){
		// Se nao foi selecionada uma unidade: ==============================================================
		if(unidadeSelecionada == 0){
			atualizaLCDVetor(unidade, unidade, valor, lcd); //Inicia o LCD
			// Se o botão Previous for apertado:
			if(digitalRead(previous) == LOW){
				previousValor(valor); //Função para valor anterior
                delay(300); //tempo para tirar o dedo do botão
                lcdClear(lcd); //Limpa o lcd
			}
			//==========================================
			// Se o botão Enter for apertado:
			else if(digitalRead(enter) == LOW){
                lcdClear(lcd); //Limpa o lcd

                unid = (valor[0]*10)+(valor[1]); //Salva o valor em uma variavel
                d = valor[0] +'0'; // dezena em char
                u = valor[1] +'0'; // unidade em char
                char unidadeEscolhida[16] = {d,u}; // Valor da unidade escolhida em char

                /*Se a unidade selecionada for 00, vai para todas unidades*/
                if(valor[0] == 0 && valor[1] == 0){ // Faz uma verificação do valor 00 e se for verdadeira
                	strcpy(unidadeEscolhida,"->Todas Unid.s"); //Muda o conteudo da unidade escolhida para todas as unidades
                    tds_unidades = 1;
				}
                else{             
                    codigoUni = codigo_unidades[unid-1]; // Guarda o valor correspondente ao código do vetor na variavel
                    strcpy(msg, codigo_unidade[unid-1]); 
                }
                
                if(tds_unidades){
                    /* Mostra na LCD a unidade selecionada*/
                    printaLCD(uniSel, unidadeEscolhida, lcd);
                    delay(1000); // Espera 1s

                    /* ================================== UART ======================================== */
                    for(i=0; i<32; i++){
                        /* Manda código*/
                        writeUart(uart0_filestream, codigo_unidades[i]);                

                        delay(30); // Tempo minimo para retorno
                        
                        /*Recebe codigo*/
                        readUart(uart0_filestream, resposta);
                        
                        if(resposta[0] == codigo_unidades[i]){
                            online[qtdOnline] = codigo_unidades[i];
                            qtdOnline++;
                            
                            printaLCDHexa("Node UART Selec", resposta[0], lcd);
                            printf("Node UART: Selec.\n");
                            unidadeSelecionada = 1;
                            writeUart(uart0_filestream, codigo_unidades[i]);  
                            delay(1500);
                            
                        }
		            }
                    /* =================================================================================*/
                    
                    /* ================================== MQTT ======================================== */
                    for(i=0; i<32; i++){
                        strcpy(msg, codigo_unidade[i]); 
                    
                        /* Manda código*/
                        publisher(TOPICPUB, msg);               

                        delay(100); // Tempo minimo para retorno
                        printf("Resposta MQTT = %s\t",respostaMQTT);
                        printf("CODIGO = %s\n", codigo_unidade[i]);
                        if (strcmp(respostaMQTT, codigo_unidade[i]) == 0){
                            strcpy(onlineMqtt[qtdOnlineMqtt], respostaMQTT);
                            qtdOnlineMqtt++;
                            printaLCDHexa("Node MQTT Selec", resposta[0], lcd);
                            printf("Node MQTT: Selec.\n");
                            unidadeSelecionada = 1;
                            publisher(TOPICPUB, msg);  
                            delay(1500);
                        }
                        
		            }
                    if(qtdOnline == 0){
                        printf("Node UART: Sem resposta.\n");
                        printaLCD("Node UART:","Sem resposta.", lcd);
                    }
                    if(qtdOnlineMqtt == 0){
                        printf("Node MQTT: Sem resposta.\n");
                        printaLCD("Node MQTT:","Sem resposta.", lcd);
                    }
                    if(qtdOnlineMqtt == 0 && qtdOnline == 0){
                        tds_unidades = 0;
                    }
                    /* ================================================================================= */
                }
                else{
                    /* Mostra na LCD a unidade selecionada*/
                    printaLCD(uniSel, unidadeEscolhida, lcd);
                    delay(1000); // Espera 1s
                    
                    /* ================================== MQTT ========================================*/
                    publisher(TOPICPUB, msg);
                    printf("Mensagem enviada: %s\n",msg);
                    /* =================================================================================*/

                    /* =============================== UART ====================================*/
                    /* Manda código*/
                    writeUart(uart0_filestream, codigoUni);                

                    delay(40); // Tempo minimo para retorno
                    
                    limpaVetor(resposta, 8);
                    /*Recebe codigo*/
                    readUart(uart0_filestream, resposta);
                    
                    lcdClear(lcd); //Limpa o lcd
                    /* ==========================================================================*/
                    if (resposta[0] != codigo_unidades[i]){ // Caso não tenha resposta da node
                        printf("Node UART: Sem resposta.\n");
                        printaLCD("Node UART:","Sem resposta.",lcd);
                        delay(1500);
                    }
                    else{ // Caso tenha resposta da node
                        printaLCDHexa("Node UART Selec", resposta[0], lcd);
                        printf("Node UART: Selec.\n");
                        unidadeSelecionada = 1;
                        mqtt = 0;
                        delay(1500);
                    } 
                
                    printf("Resposta MQTT = %s",respostaMQTT);
                    printf("CODIGO = %s", codigo_unidade[unid-1]);
                    if (unidadeSelecionada==0 && strcmp(respostaMQTT, codigo_unidade[unid-1]) != 0){ // Caso não tenha resposta da node
                        printf("Node MQTT: Sem resposta.\n");
                        printaLCD("Node MQTT:","Sem resposta.",lcd);
                        delay(1500);
                    }
                    else{ // Caso tenha resposta da node
                        printaLCD("Node MQTT Selec", respostaMQTT, lcd);
                        printf("Node MQTT: Selec.\n");
                        unidadeSelecionada = 1;
                        mqtt = 1;
                        delay(1500);
                    }
                }
                lcdClear(lcd); //Limpa o lcd
                limpaVetor(resposta, 8);  //Limpa o vetor de resposta
			}
			//==========================================
			// Se o botão Next for apertado:
			else if(digitalRead(next) == LOW){
				nextValor(valor); // Atualiza o valor do vetor do numero de unidades
                delay(300); //tempo para tirar o dedo do botão
                lcdClear(lcd); //Limpa o lcd
			}
			//==========================================
		}
		/*====================== Caso uma unidade tenha sido selecionada: ======================================*/
		else if(unidadeSelecionada == 1 && opcaoSelecionada == 0){
			// ===================> Primeira opcao = Situacao atual
			if(op == 0){
				atualizaLCD(opcao0, opcao1, lcd);
				// Se o botão Previous for apertado:	
				if(digitalRead(previous) == LOW){ 
					op = 4; // Vai até o final da lista
                    delay(300); //tempo para tirar o dedo do botão
                    lcdClear(lcd); //Limpa o lcd
				}
				/*==========================================*/
				// Se o botão Enter for apertado:
				else if(digitalRead(enter) == LOW){
                    printaLCD(opSel, opcao0, lcd); // Mostra que foi selecionada a opcao
                    delay(1000); // Espera 1s
                    lcdClear(lcd); // Limpa o lcd
					opcaoSelecionada = 1; // Coloca opcao selecionada como true
                    codigo = situacao_atual; // guarda o codigo de situacao atual na variavel
                    strcpy(msg, "0x02");
                    if (tds_unidades){
                        for(i=0; i < qtdOnline; i++){
                            /* =================================================================================*/
                            /*Ativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta

                            /* =================================================================================*/
                            /*Manda o codigo*/
                            writeUart(uart0_filestream, codigo); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido

                            if(resposta[0] == 0x02){ // Caso seja o codigo de funcionando:
                                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                                lcdPrintf(lcd, "Node: %s", online[i]);

                                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                                lcdPrintf(lcd, "Funcionando");
                            }
                            else if(resposta[0] == 0x01 || strcmp(respostaMQTT, respostas[0]) == 0){ // Caso seja o código de Problema
                                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                                lcdPrintf(lcd, "Node: %s", online[i]);

                                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                                lcdPrintf(lcd, "Com problemas");
                            }
                            else{ // Caso não tenha resposta:
                                printaLCD("NodeMCU", "Sem resposta",lcd);
                            }
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta
                            /* =================================================================================*/
                            /*Desativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(2000); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta
                        }
                        for(i=0; i < qtdOnlineMqtt; i++){
                            /* =================================================================================*/
                            /*Ativa node mqtt*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                            /* =================================================================================*/
                            /*Manda a mensagem*/
                            publisher(TOPICPUB, msg);
                            delay(100); // Aguarda um tempo para retorno da Orange
                            /* =================================================================================*/
                            if(strcmp(respostaMQTT, respostas[1]) == 0){ // Caso seja o codigo de funcionando:
                                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                                lcdPrintf(lcd, "Node: 0x%x", online[i]);

                                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                                lcdPrintf(lcd, "Funcionando");
                            }
                            else if(strcmp(respostaMQTT, respostas[0]) == 0){ // Caso seja o código de Problema
                                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                                lcdPrintf(lcd, "Node: 0x%x", online[i]);

                                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                                lcdPrintf(lcd, "Com problemas");
                            }
                            else{ // Caso não tenha resposta:
                                printaLCD("NodeMCU","Sem resposta",lcd);
                            }
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(2000); // Aguarda um tempo para retorno da Orange
                        }
                    }
                    else{
                        /* ================================== MQTT ========================================*/
                        publisher(TOPICPUB, msg);
                        delay(60); // Aguarda um tempo para retorno da Orange
                        /* =================================================================================*/
                        /* ================================== UART ========================================*/
                        writeUart(uart0_filestream, codigo); // Manda o codigo
                        delay(40); // Aguarda um tempo para retorno da Orange
                        readUart(uart0_filestream, resposta); // Le o que foi recebido
                        /* =================================================================================*/
                        if(resposta[0] == 0x02 || strcmp(respostaMQTT, respostas[1]) == 0){ // Caso seja o codigo de funcionando:
                            printaLCD("NodeMCU:","Funcionando",lcd);
                        }
                        else if(resposta[0] == 0x01 || strcmp(respostaMQTT, respostas[0]) == 0){ // Caso seja o código de Problema
                            printaLCD("NodeMCU","Com problemas",lcd);
                        }
                        else{ // Caso não tenha resposta:
                            printaLCD("NodeMCU","Sem resposta",lcd);
                        }
                        delay(2000); //tempo para tirar o dedo do botão
                    }
                    limpaVetor(resposta, 8); //Limpa o vetor de resposta
				}
				/*==========================================*/
				// Se o botão Next for apertado:
				else if(digitalRead(next) == LOW){ 
					op++; // Aumenta o valor da opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				/*==========================================*/
			}
			//==========================================================
			//Segunda opcao = Valor Analogico
			else if(op == 1){
				atualizaLCD(opcao1, opcao2, lcd); // Atualiza o lcd com a nova opcao

				if(digitalRead(previous) == LOW){ // Caso a opcao de previous seja selecionada
                    lcdClear(lcd); //Limpa o lcd
					op--; // Diminui o valor da opcao
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){ // Caso a opcao seja selecionada
                    printaLCD(opSel, opcao1, lcd); // Mostra que foi selecionada a opcao
					opcaoSelecionada = 1; // Coloca opcao selecionada como true
                    codigo = entrada_analogica; // guarda o codigo de entrada analogica na variavel
                    strcpy(msg, "0x11");
                    if(tds_unidades){
                        for(i=0; i < qtdOnline; i++){
                            /* =================================================================================*/
                            /*Ativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta

                            /* =================================================================================*/
                            /*Manda o codigo*/
                            writeUart(uart0_filestream, codigo); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido

                            /* Vai ser recebido 3 bytes*/
                            dest[0] = resposta[0]; 
                            dest[1] = resposta[1];
                            dest[2] = resposta[2];

                            // É feito um Logic shitf left para a multiplicacao no final eh somado todos os valores dos vetores
                            valorAnalog = (dest[2] << 16) + (dest[1] << 8) + dest[0];
                
                            lcdClear(lcd); //Limpa o lcd
                            printaLCDInt("Valor A0:", valorAnalog, lcd); // Mostra o valor da entrada analógica
                            /* =================================================================================*/
                            /*Desativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta
                        }
                        for(i=0; i < qtdOnlineMqtt; i++){
                            /* =================================================================================*/
                            /*Ativa node mqtt*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                            /* =================================================================================*/
                            /*Manda a mensagem*/
                            publisher(TOPICPUB, msg);
                            delay(100); // Aguarda um tempo para retorno da Orange
                            lcdClear(lcd); //Limpa o lcd
                            printaLCD("Valor A0:", respostaMQTT, lcd); // Mostra o valor da entrada analógica
                            /* =================================================================================*/
                            /*Desativa a node*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                        }
                    }
                    else{
                        /* Manda código*/
                        writeUart(uart0_filestream, codigo);
                        /* ================================== MQTT ========================================*/
                        publisher(TOPICPUB, msg);
                        /* =================================================================================*/
                        delay(100); // Tempo minimo para recepcao
                        
                        /*Recebe codigo*/
                        readUart(uart0_filestream, resposta);

                        if(mqtt){
                            lcdClear(lcd); //Limpa o lcd
                            printaLCD("Valor A0:", respostaMQTT, lcd); // Mostra o valor da entrada analógica
                        }
                        else{
                            /* Vai ser recebido 3 bytes*/
                            dest[0] = resposta[0]; 
                            dest[1] = resposta[1];
                            dest[2] = resposta[2];

                            // É feito um Logic shitf left para a multiplicacao no final eh somado todos os valores dos vetores
                            valorAnalog = (dest[2] << 16) + (dest[1] << 8) + dest[0];
                
                            lcdClear(lcd); //Limpa o lcd
                            printaLCDInt("Valor A0:", valorAnalog, lcd); // Mostra o valor da entrada analógica
                        }

                    }
                    limpaVetor(resposta, 8); //Limpa o vetor de resposta
                    delay(2000); // Espera 2s

                    /* ========================= MQTT -  ENVIO PARA O FRONT =============================
                    time(&data_hora_segundos); // preenche a variável data_hora_segundos
                    timeinfo = localtime(&data_hora_segundos);
                    strftime(data_hora, 100, "%d.%m.%Y-%H:%M", timeinfo);

                    //itoa(unidadeSelecionada,aux_unidEscolhida,10);
                    //itoa(valorSensor,valorAnalog,10);
                    sprintf(aux_unidEscolhida,"%d",unidadeSelecionada);
                    sprintf(valorSensor,"%d",valorAnalog);
                    
                    strcat(msg_front, data_hora);
		            strcat(msg_front, ",");
                    strcat(msg_front, aux_unidEscolhida);
                    strcat(msg_front, ",A0,");
                    strcat(msg_front, valorSensor);
                    strcat(msg_front, ",");

                    publisher(TOPICFRONT, msg_front);

                    memset(msg_front,'\0',100);
                    memset(data_hora,'\0',100);
                    memset(aux_unidEscolhida,'\0',10);
                    memset(valorSensor,'\0',10);

                     =================================================================================*/
				}
				else if(digitalRead(next) == LOW){ 
					op++; // Aumenta o valor da opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
			}
			//==========================================================
			//Terceira opcao = Valor Digital
			else if(op == 2){
				atualizaLCD(opcao2, opcao3, lcd);
				if(digitalRead(previous) == LOW){
					op--; //Retorna o valor anterior de opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){ // Caso seja selecionado:
					opcaoSelecionada = 1;  // Coloca opcao selecionada como true
                    op2 = 3; // Opcao de condicao para entrar no terceiro SubMenu
                    lcdClear(lcd); //Limpa a lcd
                    delay(300); // tempo para o botao
				}
				else if(digitalRead(next) == LOW){ 
					op++; // Aumenta o valor da opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
			}
			//==========================================================
			//Quarta opcao = AcendimentoLed
			else if(op == 3){
				atualizaLCD(opcao3, opcao4, lcd);	
				if(digitalRead(previous) == LOW){
					op--; // Diminui o valor da opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){
                    printaLCD(opSel, opcao3, lcd); // Mostra que foi selecionada a opcao
                    delay(1000); // espera 1s
					opcaoSelecionada = 1; // Coloca opcao selecionada como true
                    codigo = acende_led; // guarda o codigo de acender led
                    strcpy(msg, "0x21");
                    if(tds_unidades){
                        for(i=0; i < qtdOnline; i++){
                            /* =================================================================================*/
                            /*Ativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta

                            /* =================================================================================*/
                            /*Manda o codigo*/
                            writeUart(uart0_filestream, codigo); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            lcdClear(lcd); //Limpa o lcd

                            if(resposta[0] == 0x01){ // Caso a resposta seja 0x01 = High
                                printaLCD("Led","Acesa",lcd);
                            }
                            else if(resposta[0] == 0x00){ // Caso a resposta seja 0x0 = LOW
                                printaLCD("Led","Apagada",lcd);
                            }
                            else{ // Caso não tenha resposta
                                printaLCD("Led","Sem resposta",lcd);
                            }
                            delay(1500); // Aguarda um tempo para retorno da Orange

                            /* =================================================================================*/
                            /*Desativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta
                        }
                        for(i=0; i < qtdOnlineMqtt; i++){
                            /* =================================================================================*/
                            /*Ativa node mqtt*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                            /* =================================================================================*/
                            /*Manda a mensagem*/
                            publisher(TOPICPUB, msg);
                            lcdClear(lcd); //Limpa o lcd
                            if(strcmp(respostaMQTT, respostas[0]) == 0){ // Caso a resposta seja 0x01 = High
                                printaLCD("Led","Acesa",lcd);
                            }
                            else if(strcmp(respostaMQTT, respostas[4]) == 0){ // Caso a resposta seja 0x0 = LOW
                                printaLCD("Led","Apagada",lcd);
                            }
                            else{ // Caso não tenha resposta
                                printaLCD("Led","Sem resposta",lcd);
                            }
                            delay(1500);
                            /* =================================================================================*/
                            /*Desativa a node*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                        }
                    }
                    else{
                        /* ================================== MQTT ========================================*/
                        publisher(TOPICPUB, msg);
                        /* =================================================================================*/
                        /* Manda código*/
                        writeUart(uart0_filestream,codigo);

                        delay(100); // Tempo minimo para recepcao

                        /*Recebe codigo*/
                        readUart(uart0_filestream, resposta);
                        lcdClear(lcd); //Limpa o lcd
                        if(resposta[0] == 0x01 || strcmp(respostaMQTT, respostas[0]) == 0){ // Caso a resposta seja 0x01 = High
                            printaLCD("Led","Acesa",lcd);
                        }
                        else if(resposta[0] == 0x00 || strcmp(respostaMQTT, respostas[4]) == 0){ // Caso a resposta seja 0x0 = LOW
                            printaLCD("Led","Apagada",lcd);
                        }
                        else{ // Caso não tenha resposta
                            printaLCD("Led","Sem resposta",lcd);
                        }

                    }

                    limpaVetor(resposta, 8); //Limpa o vetor de resposta
                    delay(2000); // Espera 2s

                    /* ========================= MQTT -  ENVIO PARA O FRONT =============================
                    time(&data_hora_segundos); // preenche a variável data_hora_segundos
                    timeinfo = localtime(&data_hora_segundos);
                    strftime(data_hora, 100, "%d.%m.%Y-%H:%M", timeinfo);

                    //itoa(unidadeSelecionada,aux_unidEscolhida,10);
                    //itoa(valorSensor,valorAnalog,10);
                    sprintf(aux_unidEscolhida,"%d",unidadeSelecionada);
                    sprintf(valorSensor,"%d",valorLED);
                    
                    strcat(msg_front, data_hora);
		            strcat(msg_front, ",");
                    strcat(msg_front, aux_unidEscolhida);
                    strcat(msg_front, ",LED,");
                    strcat(msg_front, valorSensor);
                    strcat(msg_front, ",");

                    publisher(TOPICFRONT, msg_front);

                    memset(msg_front,'\0',100);
                    memset(data_hora,'\0',100);
                    memset(aux_unidEscolhida,'\0',10);
                    memset(valorSensor,'\0',10);

                     =================================================================================*/

				}
				else if(digitalRead(next) == LOW){ 
					op++; // Aumenta o valor da opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
			}
			//==========================================================
            //Quinta opcao = monitoramento
			else if(op == 4){
				atualizaLCD(opcao4, opcao0, lcd);	 // Atualiza o lcd com a nova opcao
				if(digitalRead(previous) == LOW){
					op--; // Diminui o valor da opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){ // Caso a opcao seja selecionada
                    lcdClear(lcd); //Limpa o lcd
                    printaLCD(opSel, opcao4, lcd); // Mostra que foi selecionada a opcao
                    delay(1000); // Espera 1s
                    opcaoSelecionada = 1; // Coloca opcao selecionada como true
                    op2 = 2; // Opcao de condicao para entrar no segundo SubMenu
                    lcdClear(lcd); //Limpa o lcd
				}
				else if(digitalRead(next) == LOW){ 
					op = 0; // Volta pra primeira opcao
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
			}
			//==========================================================
		}
        //=============== Em caso de Já ter mandado uma requisição a uma unidade ============================
        else if(unidadeSelecionada == 1 && opcaoSelecionada == 1){
            /*========= Primeiro submenu Nova Requisicao ou Nova Unidade ===================*/
            if(op2 == 0){ 
                printaLCD("->Outra Unid.", "Nova Requisicao", lcd);  
                // Se selecionado, as variaveis resetam e volta pro menu de unidades          
                if(digitalRead(enter) == LOW){
                    opcaoSelecionada = 0;
                    unidadeSelecionada =0;
                    mqtt = 0;
                    if(tds_unidades){
                        memset(online, 0, 32);
                        qtdOnline = 0;
                        memset(onlineMqtt, 0, 32);
                        qtdOnlineMqtt = 0;
                        tds_unidades=0;
                    }else{
                        writeUart(uart0_filestream, codigoUni);
                        strcpy(msg, codigo_unidade[unid]);
                        /* ================================== MQTT ========================================*/
                        publisher(TOPICPUB, msg);
                        delay(100);
                        /* =================================================================================*/
                        printf("Node deselecionada MQTT: %s\n",respostaMQTT);
                        printf("Node deselecionada UART: 0x%x\n",resposta[0]);

                    }

                    delay(300); //tempo para tirar o dedo do botão
                    lcdClear(lcd);
                }
                else if(digitalRead(previous) == LOW){
                    op2 ++; // Vai pra proxima unidade do submenu
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
                else if(digitalRead(next) == LOW){
                    op2 ++; // Vai pra proxima unidade do submenu
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
            }
            else if(op2 == 1){
                printaLCD("->Requisicao", "Outra Unid.", lcd);
                // Se selecionado, só a variavel de opcao eh resetada, voltando pro menu de requisicoes
                if(digitalRead(enter) == LOW){
                    opcaoSelecionada = 0;
                    delay(300);
                    lcdClear(lcd);
                }
                else if(digitalRead(previous) == LOW){
                    op2 --; // Vai pra unidade anterior do submenu
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
                else if(digitalRead(next) == LOW){
                    op2 --; // Vai pra unidade anterior do submenu
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
            }
            /*========================== Menu de monitoramento ======================*/
            if(op2 == 2){
                if(opcoesSensores == 0){ // Caso Seja os sensores Analogicos:
                    printaLCD("Qual sensor?","-> A0", lcd);
                    if(digitalRead(enter) == LOW){
                        delay(400); // Espera um tempo de 400ms por conta do botao
                        while (digitalRead(enter) == HIGH){ // Enquanto o botão enter não for precionado
                            codigo = entrada_analogica;
                            strcpy(msg, "0x11");

                            if(tds_unidades){
                                for(i=0; i < qtdOnline; i++){
                                    /* =================================================================================*/
                                    /*Ativa a node*/
                                    writeUart(uart0_filestream, online[i]); // Manda o codigo
                                    delay(30); // Aguarda um tempo para retorno da Orange
                                    readUart(uart0_filestream, resposta); // Le o que foi recebido
                                    limpaVetor(resposta, 8); //Limpa o vetor de resposta

                                    /* =================================================================================*/
                                    /*Manda o codigo*/
                                    writeUart(uart0_filestream, codigo); // Manda o codigo
                                    delay(30); // Aguarda um tempo para retorno da Orange
                                    readUart(uart0_filestream, resposta); // Le o que foi recebido

                                    /* Vai ser recebido 3 bytes*/
                                    dest[0] = resposta[0]; 
                                    dest[1] = resposta[1];
                                    dest[2] = resposta[2];

                                    // É feito um Logic shitf left para a multiplicacao no final eh somado todos os valores dos vetores
                                    valorAnalog = (dest[2] << 16) + (dest[1] << 8) + dest[0];
                        
                                    lcdClear(lcd); //Limpa o lcd
                                    printaLCDInt("Valor A0:", valorAnalog, lcd); // Mostra o valor da entrada analógica
                                    delay(1000);
                                    /* =================================================================================*/
                                    /*Desativa a node*/
                                    writeUart(uart0_filestream, online[i]); // Manda o codigo
                                    delay(30); // Aguarda um tempo para retorno da Orange
                                    readUart(uart0_filestream, resposta); // Le o que foi recebido
                                    limpaVetor(resposta, 8); //Limpa o vetor de resposta
                                }
                                for(i=0; i < qtdOnlineMqtt; i++){
                                    /* =================================================================================*/
                                    /*Ativa node mqtt*/
                                    publisher(TOPICPUB, onlineMqtt[i]);
                                    delay(100); // Aguarda um tempo para retorno da Orange
                                    /* =================================================================================*/
                                    /*Manda a mensagem*/
                                    publisher(TOPICPUB, msg);
                                    delay(100); // Aguarda um tempo para retorno da Orange
                                    lcdClear(lcd); //Limpa o lcd
                                    printaLCD("Valor A0:", respostaMQTT, lcd); // Mostra o valor da entrada analógica
                                    delay(1000);
                                    /* =================================================================================*/
                                    /*Desativa a node*/
                                    publisher(TOPICPUB, onlineMqtt[i]);
                                    delay(100); // Aguarda um tempo para retorno da Orange
                                }
                            }
                            else{
                                /* Manda código*/
                                writeUart(uart0_filestream, codigo);
                                /* ================================== MQTT ========================================*/
                                publisher(TOPICPUB, msg);
                                /* =================================================================================*/
                                delay(100); // Tempo minimo para recepcao
                                
                                /*Recebe codigo*/
                                readUart(uart0_filestream, resposta);

                                if(mqtt){
                                    lcdClear(lcd); //Limpa o lcd
                                    printaLCD("Valor A0:", respostaMQTT, lcd); // Mostra o valor da entrada analógica
                                }
                                else{
                                    /* Vai ser recebido 3 bytes*/
                                    dest[0] = resposta[0]; 
                                    dest[1] = resposta[1];
                                    dest[2] = resposta[2];

                                    // É feito um Logic shitf left para a multiplicacao no final eh somado todos os valores dos vetores
                                    valorAnalog = (dest[2] << 16) + (dest[1] << 8) + dest[0];
                        
                                    lcdClear(lcd); //Limpa o lcd
                                    printaLCDInt("Valor A0:", valorAnalog, lcd); // Mostra o valor da entrada analógica
                                    delay(1000);
                                }

                            }
                            /* ========================= MQTT -  ENVIO PARA O FRONT =============================
                            time(&data_hora_segundos); // preenche a variável data_hora_segundos
                            timeinfo = localtime(&data_hora_segundos);
                            strftime(data_hora, 100, "%d.%m.%Y-%H:%M", timeinfo);

                            //itoa(unidadeSelecionada,aux_unidEscolhida,10);
                            //itoa(valorSensor,valorAnalog,10);
                            sprintf(aux_unidEscolhida,"%d",unidadeSelecionada);
                            sprintf(valorSensor,"%d",valorAnalog);
                            
                            strcat(msg_front, data_hora);
                            strcat(msg_front, ",");
                            strcat(msg_front, aux_unidEscolhida);
                            strcat(msg_front, ",A0,");
                            strcat(msg_front, valorSensor);
                            strcat(msg_front, ",");

                            publisher(TOPICFRONT, msg_front);

                            memset(msg_front,'\0',100);
                            memset(data_hora,'\0',100);
                            memset(aux_unidEscolhida,'\0',10);
                            memset(valorSensor,'\0',10);

                             =================================================================================*/                           

                        }
                        limpaVetor(resposta, 8);
                        op2 =0;
                        lcdClear(lcd); 
                    }
                    else if(digitalRead(previous) == LOW){ 
                        opcoesSensores ++;
                        lcdClear(lcd); //Limpa o lcd
                        delay(300); //tempo para tirar o dedo do botão
                    }
                    else if(digitalRead(next) == LOW){ 
                        opcoesSensores ++;
                        lcdClear(lcd); //Limpa o lcd
                        delay(300); //tempo para tirar o dedo do botão
                    }
                }
                else if(opcoesSensores == 1){ // Caso Seja os sensores Digitais:
                    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                    lcdPrintf(lcd, "Qual Sensor?");

                    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                    lcdPrintf(lcd,"-> D%i",sensoresDigitais); // Mostra o sensor para selecao

                    if(digitalRead(enter) == LOW){
                        delay(300); //tempo para tirar o dedo do botão
                        lcdClear(lcd); // Limpa o lcd
                        switch (sensoresDigitais){ // Faz um switch com o valor selecionado
                            case 0:
                                codigo = entrada_digital_0;
                                strcpy(msg, "0x12");
                                strcpy(entradaD, "D0");
                                break;
                            case 1:
                                codigo = entrada_digital_1;
                                strcpy(msg, "0x13");
                                strcpy(entradaD, "D1");
                                break;
                            case 2:
                                codigo = entrada_digital_2;
                                strcpy(msg, "0x14");
                                strcpy(entradaD, "D2");
                                break;
                            case 3:
                                codigo = entrada_digital_3;
                                strcpy(msg, "0x15");
                                strcpy(entradaD, "D3");
                                break;
                            case 4:
                                codigo = entrada_digital_4;
                                strcpy(msg, "0x16");
                                strcpy(entradaD, "D4");
                                break;
                            case 5:
                                codigo = entrada_digital_5;
                                strcpy(msg, "0x17");
                                strcpy(entradaD, "D5");
                                break;
                            case 6:
                                codigo = entrada_digital_6;
                                strcpy(msg, "0x18");
                                strcpy(entradaD, "D6");
                                break;
                            case 7:
                                codigo = entrada_digital_7;
                                strcpy(msg, "0x19");
                                strcpy(entradaD, "D7");
                                break;
                            case 8:
                                codigo = entrada_digital_8;
                                strcpy(msg, "0x1A");
                                strcpy(entradaD, "D8");
                                break;
                        }
                        while (digitalRead(enter) == HIGH){ // Enquanto o botão enter não for precionado
                            if(tds_unidades){
                                for(i=0; i < qtdOnline; i++){
                                    /* =================================================================================*/
                                    /*Ativa a node*/
                                    writeUart(uart0_filestream, online[i]); // Manda o codigo
                                    delay(30); // Aguarda um tempo para retorno da Orange
                                    readUart(uart0_filestream, resposta); // Le o que foi recebido
                                    limpaVetor(resposta, 8); //Limpa o vetor de resposta

                                    /* =================================================================================*/
                                    /*Manda o codigo*/
                                    writeUart(uart0_filestream, codigo); // Manda o codigo
                                    delay(30); // Aguarda um tempo para retorno da Orange
                                    readUart(uart0_filestream, resposta); // Le o que foi recebido
                                    lcdClear(lcd); //Limpa o lcd
                                    printaLCDInt("Valor:", resposta[0], lcd);
                                    delay(1500); // Espera 2s

                                    /* =================================================================================*/
                                    /*Desativa a node*/
                                    writeUart(uart0_filestream, online[i]); // Manda o codigo
                                    delay(30); // Aguarda um tempo para retorno da Orange
                                    readUart(uart0_filestream, resposta); // Le o que foi recebido
                                    limpaVetor(resposta, 8); //Limpa o vetor de resposta
                                }
                                for(i=0; i < qtdOnlineMqtt; i++){
                                    /* =================================================================================*/
                                    /*Ativa node mqtt*/
                                    publisher(TOPICPUB, onlineMqtt[i]);
                                    delay(100); // Aguarda um tempo para retorno da Orange
                                    /* =================================================================================*/
                                    /*Manda a mensagem*/
                                    publisher(TOPICPUB, msg);
                                    lcdClear(lcd); //Limpa o lcd
                                    printaLCD("Valor:", respostaMQTT, lcd);
                                    delay(1500); // Espera 2s
                                    /* =================================================================================*/
                                    /*Desativa a node*/
                                    publisher(TOPICPUB, onlineMqtt[i]);
                                    delay(100); // Aguarda um tempo para retorno da Orange
                                }
                            }
                            else{
                                /* ================================== MQTT ========================================*/
                                publisher(TOPICPUB, msg);
                                /* =================================================================================*/
                                /* Manda código*/
                                writeUart(uart0_filestream, codigo);
                                delay(100); // Tempo minimo para recepcao
                                /*Recebe codigo*/
                                readUart(uart0_filestream, resposta);

                                lcdClear(lcd); //Limpa o lcd
                                if(mqtt){
                                    printaLCD("Valor:", respostaMQTT, lcd);
                                }
                                else{
                                    printaLCDInt("Valor:", resposta[0], lcd);
                                }
                                delay(2000); // Espera 2s
                            }

                            /* ========================= MQTT -  ENVIO PARA O FRONT =============================
                            time(&data_hora_segundos); // preenche a variável data_hora_segundos
                            timeinfo = localtime(&data_hora_segundos);
                            strftime(data_hora, 100, "%d.%m.%Y-%H:%M", timeinfo);

                            //itoa(unidadeSelecionada,aux_unidEscolhida,10);
                            //itoa(valorSensor,valorAnalog,10);
                            sprintf(aux_unidEscolhida,"%d",unidadeSelecionada);
                            sprintf(valorSensor,"%d",resposta[0]);
                            
                            strcat(msg_front, data_hora);
                            strcat(msg_front, ",");
                            strcat(msg_front, aux_unidEscolhida);
                            strcat(msg_front, ",");
                            strcat(msg_front, entradaD);
                            strcat(msg_front, ",");
                            strcat(msg_front, valorSensor);
                            strcat(msg_front, ",");

                            publisher(TOPICFRONT, msg_front);

                            memset(msg_front,'\0',100);
                            memset(data_hora,'\0',100);
                            memset(aux_unidEscolhida,'\0',10);
                            memset(valorSensor,'\0',10);

                             =================================================================================*/                           
                        }
                        limpaVetor(resposta, 8);
                        op2 =0; // Volta para o primeiro submenu
                        lcdClear(lcd); // Limpa o LCD
                    }                    
                    else if(digitalRead(previous) == LOW && sensoresDigitais ==0){ 
                        opcoesSensores = 0; // Volta para a opcao do sensor analogico
                        lcdClear(lcd); //Limpa o lcd
                        delay(300); //tempo para tirar o dedo do botão
                    }
                    else if(digitalRead(next) == LOW && sensoresDigitais ==8){ 
                        opcoesSensores = 0; // Volta para a opcao do sensor analogico
                        lcdClear(lcd); //Limpa o lcd
                        delay(300); //tempo para tirar o dedo do botão
                    }
                    else if(digitalRead(previous) == LOW && sensoresDigitais > 0){ 
                        sensoresDigitais--; // Sensor digital anterior
                        lcdClear(lcd); //Limpa o lcd
                        delay(300); //tempo para tirar o dedo do botão
                    }
                    else if(digitalRead(next) == LOW && sensoresDigitais < 8){ 
                        sensoresDigitais++; // Proximo Sensor digital 
                        lcdClear(lcd); //Limpa o lcd
                        delay(300); //tempo para tirar o dedo do botão
                    }
                }
            }
            /*========================== SubMenu de sensores digitais ======================*/
            if(op2 == 3){
                
                lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
                lcdPrintf(lcd, "Qual Sensor?");

                lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
                lcdPrintf(lcd,"-> D%i",sensoresDigitais); // Mostra o sensor para selecao

                if(digitalRead(enter) == LOW){ // Caso o sensor seja selecioando
                    switch (sensoresDigitais){ // Faz um switch com o valor
                        case 0:
                            codigo = entrada_digital_0;
                            strcpy(msg, "0x12");
                            strcpy(entradaD, "D0");
                            break;
                        case 1:
                            codigo = entrada_digital_1;
                            strcpy(msg, "0x13");
                            strcpy(entradaD, "D1");
                            break;
                        case 2:
                            codigo = entrada_digital_2;
                            strcpy(msg, "0x14");
                            strcpy(entradaD, "D2");
                            break;
                        case 3:
                            codigo = entrada_digital_3;
                            strcpy(msg, "0x15");
                            strcpy(entradaD, "D3");
                            break;
                        case 4:
                            codigo = entrada_digital_4;
                            strcpy(msg, "0x16");
                            strcpy(entradaD, "D4");
                            break;
                        case 5:
                            codigo = entrada_digital_5;
                            strcpy(msg, "0x17");
                            strcpy(entradaD, "D5");
                            break;
                        case 6:
                            codigo = entrada_digital_6;
                            strcpy(msg, "0x18");
                            strcpy(entradaD, "D6");
                            break;
                        case 7:
                            codigo = entrada_digital_7;
                            strcpy(msg, "0x19");
                            strcpy(entradaD, "D7");
                            break;
                        case 8:
                            codigo = entrada_digital_8;
                            strcpy(msg, "0x1A");
                            strcpy(entradaD, "D8");
                            break;
                    }
                    if(tds_unidades){
                        for(i=0; i < qtdOnline; i++){
                            /* =================================================================================*/
                            /*Ativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta

                            /* =================================================================================*/
                            /*Manda o codigo*/
                            writeUart(uart0_filestream, codigo); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            lcdClear(lcd); //Limpa o lcd
                            printaLCDInt("Valor:", resposta[0], lcd);
                            delay(1500); // Espera 2s

                            /* =================================================================================*/
                            /*Desativa a node*/
                            writeUart(uart0_filestream, online[i]); // Manda o codigo
                            delay(30); // Aguarda um tempo para retorno da Orange
                            readUart(uart0_filestream, resposta); // Le o que foi recebido
                            limpaVetor(resposta, 8); //Limpa o vetor de resposta
                        }
                        for(i=0; i < qtdOnlineMqtt; i++){
                            /* =================================================================================*/
                            /*Ativa node mqtt*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                            /* =================================================================================*/
                            /*Manda a mensagem*/
                            publisher(TOPICPUB, msg);
                            lcdClear(lcd); //Limpa o lcd
                            printaLCD("Valor:", respostaMQTT, lcd);
                            delay(1500); // Espera 2s
                            /* =================================================================================*/
                            /*Desativa a node*/
                            publisher(TOPICPUB, onlineMqtt[i]);
                            delay(100); // Aguarda um tempo para retorno da Orange
                        }
                    }
                    else{
                        /* ================================== MQTT ========================================*/
                        publisher(TOPICPUB, msg);
                        /* =================================================================================*/
                        /* Manda código*/
                        writeUart(uart0_filestream, codigo);
                        delay(100); // Tempo minimo para recepcao
                        /*Recebe codigo*/
                        readUart(uart0_filestream, resposta);

                        lcdClear(lcd); //Limpa o lcd
                        if(mqtt){
                            printaLCD("Valor:", respostaMQTT, lcd);
                        }
                        else{
                            printaLCDInt("Valor:", resposta[0], lcd);
                        }
                        delay(2000); // Espera 2s
                    }

                    /* ========================= MQTT -  ENVIO PARA O FRONT =============================
                    time(&data_hora_segundos); // preenche a variável data_hora_segundos
                    timeinfo = localtime(&data_hora_segundos);
                    strftime(data_hora, 100, "%d.%m.%Y-%H:%M", timeinfo);

                    //itoa(unidadeSelecionada,aux_unidEscolhida,10);
                    //itoa(valorSensor,valorAnalog,10);
                    sprintf(aux_unidEscolhida,"%d",unidadeSelecionada);
                    sprintf(valorSensor,"%d",resposta[0]);
                    
                    strcat(msg_front, data_hora);
                    strcat(msg_front, ",");
                    strcat(msg_front, aux_unidEscolhida);
                    strcat(msg_front, ",");
                    strcat(msg_front, entradaD);
                    strcat(msg_front, ",");
                    strcat(msg_front, valorSensor);
                    strcat(msg_front, ",");

                    publisher(TOPICFRONT, msg_front);

                    memset(msg_front,'\0',100);
                    memset(data_hora,'\0',100);
                    memset(aux_unidEscolhida,'\0',10);
                    memset(valorSensor,'\0',10);

                     =================================================================================*/
                    limpaVetor(resposta, 8); // Limpa o vetor de resposta
                    op2 =1; // Volta pro primeiro subMenu
                    delay(300); //Tempo para o botao
                }
                else if(digitalRead(previous) == LOW){ // Caso o botao de anterior seja pressionado
                    if (sensoresDigitais == 0){ // Se estiver no inicio da lista
                        sensoresDigitais = 8; // Volta pro final
                    }
                    else{
                        sensoresDigitais --; // Caso contrario retorna uma unidade
                    }
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
                else if(digitalRead(next) == LOW){  // Caso o botao de proximo seja pressionado
                    if (sensoresDigitais == 8){  // Se estiver no final da lista
                        sensoresDigitais = 0; // Volta pro inicio
                    }
                    else{
                        sensoresDigitais ++; // Caso contrario avanca uma unidade
                    }
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
            }
        }
    }
    /*-------------- Desconexao do MQTT -----------*/
    MQTTClient_destroy(&client);

    close(uart0_filestream); // Fecha o arquivo da uart
    return 0; // Finaliza o programa
}
//===================================================================================================
/*======================================== Funcoes ================================================*/
/* ========================================= MQTT =================================================*/
/* ------------------------------- Função do PUBLISHER MQTT -------------------------------------- */
void publisher (char topico[], char msg[]){
    /*Criando o formato de mensagem para o envio*/
    pubmsg.payload = msg;
    pubmsg.payloadlen = (int)strlen(msg);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    /* ------------------------------ Tentativa de publicacao ----------------------------------- */
    if (MQTTClient_publishMessage(client, TOPICPUB, &pubmsg, &token) != MQTTCLIENT_SUCCESS){
        printf("Falha no envio, tente novamente\n");    
        if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
            printf("Failed to connect, return code %d\n", rc);
        }
    }    
}
//===================================================================================================
/* ------------------------------- Função do SUBSCRIBE MQTT -------------------------------------- */
void subscriber (void){
    printf(" Subscrevendo no topico %s\n para o cliente %s usando QoS%d\n\n", TOPICSUB, CLIENTID, QOS);
    if (MQTTClient_subscribe(client, TOPICSUB, QOS) != MQTTCLIENT_SUCCESS){
        printf("Falha em realizar o subscribe!\n");
    }
    else{
        printf("Inscrito no topico!\n");
    }
}
/* =================================================================================================== */
/* --------------------------- Função mensagem recebida do SUBSCRIBE --------------------------------- */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message){
    // Copia o valor recebido para a variável respostaMQTT que é Global
    strncpy(respostaMQTT, (char*)message->payload, sizeof(respostaMQTT) - 1);
    respostaMQTT[sizeof(respostaMQTT) - 1] = '\0';
    printf("%s\n", respostaMQTT);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
/* =================================================================================================== */
/* =========================================== LCD =================================================== */
/* ------------------------------ Função para mostrar String na LCD ---------------------------------- */
void printaLCD(char dadoSup[],char dadoInf[], int lcd){ //ImpressÃ£o no lcd
    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
    lcdPrintf(lcd, "%s", dadoSup);

    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
    lcdPrintf(lcd, "%s", dadoInf);
}
/* =================================================================================================== */
/* --------------------------- Funcao para mostrar Hexadecimal na LCD -------------------------------- */
void printaLCDHexa(char dadoSup[],char dadoInf, int lcd){ //ImpressÃ£o no lcd
    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
    lcdPrintf(lcd, "%s", dadoSup);

    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
    lcdPrintf(lcd, "Codigo: 0x%x", dadoInf);
}
/* =================================================================================================== */
/* ------------------------------ Funcao para mostrar na int LCD ------------------------------------ */
void printaLCDInt(char dadoSup[],int valorAnalog, int lcd){ //ImpressÃ£o no lcd
    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
    lcdPrintf(lcd, "%s", dadoSup);

    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
    lcdPrintf(lcd, "%d", valorAnalog);
}
/* =================================================================================================== */
/* ---------------------------- Funcao para colocar a chave de seleção ------------------------------- */
void atualizaLCD (char fraseSup[], char fraseInf[], int lcd){
	// Variaveis auxiliares
	char selecao[16] = "->", prox[16]; 
	
	//Adiciona a seta na parte de selecao
	strcat(selecao,fraseSup); // Adiciona a Seta na opcao que pode ser selecionada
	strcpy(prox, fraseInf); // Copia a mensagem da frase para a variavel auxiliar

    printaLCD(selecao, prox, lcd); // Mostra na LCD
}
/* =================================================================================================== */
/* =========================================== UART ================================================== */
/* ---------------------------------- Funcao pra Escrever na UART ------------------------------------ */
void writeUart (int uart0_filestream, unsigned char dado){

    unsigned char tx_buffer[10]; // Variavel para buffer
    unsigned char *p_tx_buffer; // Ponteiro para o buffer

    p_tx_buffer = &tx_buffer[0]; // Aponta para o endereco do inicio do buffer

    *p_tx_buffer++ = dado; // adiciona ao ponteiro o dado que tem para envio
    
    // Envio do TX - Uart
    int count = write (uart0_filestream, &tx_buffer[0], (p_tx_buffer-&tx_buffer[0])); // escreve e retorna o tamanho da palavra escrita
    printf("Escrevendo na UART ...\n");
    if(count<0){ // caso nao escreva nada
        printf("Erro no envio de dados - TX\n"); // Mostra Erro no console
    }
}
/* =================================================================================================== */
/* ------------------------------------ Funcao pra Ler na UART --------------------------------------- */
void readUart(int uart0_filestream, unsigned char rx_buffer[]){
    // Recebimento do RX - Uart
    int rx_length = 0;

    rx_length = read (uart0_filestream, (void*) rx_buffer, 8); // Le o buffer e retorna o tamanho
    if(rx_length <0){ // Caso nao tenha resposta:
        rx_buffer[0] = erro; // Substitui por codigo de erro
    }
    else if (rx_length == 0){ // Caso nao tenha dado na resposta
        printf("Nenhum dado disponível\n");
    }
}
/* =================================================================================================== */
/* ===================================== Auxiliares do menu ========================================== */
/*Função para colocar a chave de seleção antes da unidade e acrescentar o valor da unidade no fim da linha*/
void atualizaLCDVetor (char fraseSup[], char fraseInf[], int valor[], int lcd){
	// Variaveis auxiliares
	int posSup = 0, posInf = 0;
	char k, l, m, n;
	char selecao[16] = "->", prox[16];
	
	//Adiciona a seta na parte de selecao
	strcat(selecao,fraseSup); // Adiciona a Seta na opcao que pode ser selecionada
	strcpy(prox, fraseInf); // Copia a mensagem da frase para a variavel auxiliar
	
		
	// Percorrendo a string
	posSup = strlen(selecao); // Guarda o tamanho da variavel
	posInf = strlen(prox); // Guarda o tamanho da variavel
	 
    //Frase superior
    k = valor[0] +'0'; // dezena em char
    l = valor[1] +'0'; // unidade em char
    selecao [posSup] = k; // adiciona o valor da dezena
    selecao [posSup + 1] = l; // adiciona o valor da unidade
    selecao [posSup + 2] = '\0'; // adiciona \0 para indicar fim da string

    // Frase inferior é feito o mesmo protocolo
    m = valor[2] +'0'; //Unidade Futura
    n = valor[3] +'0'; //Dezena Futura
    prox [posInf] = m;
    prox [posInf + 1] = n;
    prox [posInf + 2] = '\0';
    
    if(valor[0] == 0 && valor[1] == 0){ // Se a unidade for 00:
    	strcpy(selecao, "->TodasUnidades"); // Copia o valor para frase de selecao
	}
	else if(valor[2] == 0 && valor[3] == 0){ // Se a proxima unidade for 00:
    	strcpy(prox, "TodasUnidades");  // Copia o valor para proxima frase de selecao
	}
    printaLCD(selecao, prox, lcd); // Mostra na lcd
}
/* =================================================================================================== */
/* ----------------------------- Função para passar o valor do vetor --------------------------------- */
void nextValor(int v[]){
	//Dezena, Unidade, Dezena Futura, Unidade Futura; 
	v[1] = v[1] + 1;        
	v[3] = v[3] + 1;   
    /* Unidade Para selecao*/ 
	if (v[1] >= 10){ // Se unidade chegar a 10
	    v[0]= v[0]+1; // Adicione 1 em dezena
	    v[1]= 0; // resete a unidade
	}
	else if(v[0]==3 && v[1]==3){ // Se Dezena for igual 3 e unidade for igual a 3
	    v[0] = 0; // resete dezena
	    v[1] = 0; // resete unidade
	}	
    /* Unidade Futura*/
	if(v[3] >= 10){ // Se o valor for maior ou igual a 10
	    v[2] = v[2]+1;  // Adicione 1 em dezena
	    v[3] = 0;  // resete a unidade
	}	
	else if(v[2]==3 && v[3]==3){ // Se Dezena for igual 3 e unidade for igual a 3
	    v[2] = 0;// resete dezena
	    v[3] = 0;// resete unidade
	}
}
/* =================================================================================================== */
/* ----------------------------- Função para voltar o valor do vetor --------------------------------- */
void previousValor(int v[]){
	int negativo = -1, zero =0;
	//Valor Atual
	if((v[0]) == zero && (v[1] - 1) == negativo){ //Se virar negativo
		v[0] = 3; // Volta a dezena pra 3
		v[1] = 2; // Volta a unidade pra 2 = 32
	}
	else if ( (v[0]) >= zero && (v[1] - 1) >=  zero){ // Se unidade maior ou igual a zero e dezena -1 for maior ou igual a zero
		v[1] = v[1] - 1;  // Diminui 1 na unidade
	}
	else if ( (v[0]) > zero && (v[1] - 1) == negativo){ // Se unidade maior ou igual a zero e dezena -1 for maior ou igual a negativo
		v[1] = 9; // Unidade = 9
		v[0] = v[0] - 1; // Diminui 1 na dezena
	}
	
	// Valor Futuro = mesmo protocolo do valor atual
	if((v[2]) == zero && (v[3] - 1) == negativo){
		v[2] = 3;
		v[3] = 2;
	}	
	else if ( (v[2]) == zero && (v[3] - 1) ==  zero){
		v[3] = v[3] - 1;
	}
	else if ( (v[2]) >= zero && (v[3] - 1) >=  zero){
		v[3] = v[3] - 1;  
	}
	else if ( (v[2]) > zero && (v[3] - 1) == negativo){
		v[3] = 9;
		v[2] = v[2] - 1;
	}
}
/* =================================================================================================== */
/* ----------------------- Função para limpar vetor buffer de recebimento ---------------------------- */
void limpaVetor (unsigned char v[], int tamanho){
    int i;
    for(i=0; i < tamanho; i++){ // Enquanto i for menor que o tamanho passado como referencia:
        v[i] = 0b00000000; // acrescenta o valor 0  em binario dentro do vetor para reseta-lo
    }
}
/* =================================================================================================== */
