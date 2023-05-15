#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h> // Uso da  GPIO
#include <lcd.h> // http://wiringpi.com/dev-lib/lcd-library/
#include <unistd.h> // para usar o sys
#include <fcntl.h> // A biblioteca deve definir as seguintes solicitações e argumentos para uso pelas funções fcntl () e open () .
#include <termios.h> // A biblioteca contém as definições usadas pelas interfaces de E/S do terminal 
#include <time.h>   // Uso da funcao delay
#include <string.h> //Manipulacao de string

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
#define todas_unidades 0b11111110

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

// Menu
#define next 25
#define enter 23
#define previous 19


//dipSwitchs
#define dipSwitch0 2
#define dipSwitch1 5
#define dipSwitch2 7
#define dipSwitch3 8

/*  Pinos do LED:
    PA8 -> 8 GPIO -> 20 WPi
    PA9 -> 9 GPIO -> 22 WPi
*/
// #define PA8 20
// #define PA9 22
/*-----------------------------------------------------*/

/*--------------------------Funções--------------------*/
void printaLCD(char linhaSup[], char linhaInf[], int lcd); //printa o lcd
void printaLCDInt(char linhaSup[], int valorAnalog, int lcd); //printa o lcd
void printaLCDHexa(char dadoSup[],char dadoInf, int lcd); //printa o lcd
void writeUart (int uart0_filestream, unsigned char dado);
void readUart (int uart0_filestream, unsigned char resposta[]);
void atualizaLCD (char fraseSup[], char fraseInf[], int lcd);
void atualizaLCDVetor (char fraseSup[], char fraseInf[], int valor[], int lcd);
void nextValor(int v[]);
void previousValor(int v[]);

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
	int valor[5] = {0,0,0,1}; //Dezena, Unidade, Dezena Futura, Unidade Futura; 
	int op = 0, op2 = 0, unid = 0, lcd, valorAnalog = 0; //Opcao e entradada de teclado
	char uniSel[16] = "UniSelecionada", opSel[16] = "OpSelecionada";
	char unidade[16]= "Unidade = ";
    char opcao0[16] = "Situacaoatual", opcao1[16]="ValorAnalogic"; 
    char opcao2[16] = "ValorDigital", opcao3[16] ="Acende_Led";
    char u, d;
    unsigned char resposta[2];
    unsigned char codigo, dest[4];
    int unidadeSelecionada = 0, opcaoSelecionada = 0, respostaC = 0; 
	/*==================================================================================*/
    int uart0_filestream = -1; //Retorno de erro da função Open - 

    //Para o open usaremos a uart 3: /dev/ttyS3
    uart0_filestream = open("/dev/ttyS3", O_RDWR | O_NOCTTY | O_NDELAY);  
    /*
        FLAGS:
        O_RDWR -> Lê e escreve
        O_NOCTTY -> Identifica o dispositivo como dispositivo de terminal
        O_NDELAY -> Sem delay, para acesso imediato
    */

    if (uart0_filestream == -1){ //Verifica se deu erro na abertura da UART
        printf("Erro na abertura da UART\n");
    }
    else{
        printf("Abertura do arquivo ttyS3 com êxito");
    }

    /* Configuração da uart*/
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //BaudRate, tamanho da palavra (CSIZE) -> 9600 8-N-1
    options.c_iflag = IGNPAR; //ignora a paridade
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    /*============================== INICIA LCD =================================*/
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
	/*==================================================================================*/
   
    /*============================== BEM VINDO =================================*/
    printaLCD("Menu-TEC499-P2", "Escolha a Opcao:", lcd);    // imprime estado do LED]
    delay(2000);
    lcdClear(lcd);
	/*==================================================================================*/

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
                char unidadeEscolhida[16] = {d,u};
                
                // Case com a unidade escolhida:
                switch(unid){
                    case 0:
                        codigo = todas_unidades;
                    break;
                    case 1:
                        codigo = unidade_1;
                    break;
                    case 2:
                        codigo = unidade_2;
                    break;
                    case 3:
                        codigo = unidade_3;
                    break;
                    case 4:
                        codigo = unidade_4;
                    break;
                    case 5:
                        codigo = unidade_5;
                    break;
                    case 6:
                        codigo = unidade_6;
                    break;
                    case 7:
                        codigo = unidade_7;
                    break;
                    case 8:
                        codigo = unidade_8;
                    break;
                    case 9:
                        codigo = unidade_9;
                    break;
                    case 10:
                        codigo = unidade_10;
                    break;
                    case 11:
                        codigo = unidade_11;
                    break;
                    case 12:
                        codigo = unidade_12;
                    break;
                    case 13:
                        codigo = unidade_13;
                    break;
                    case 14:
                        codigo = unidade_14;
                    break;
                    case 15:
                        codigo = unidade_15;
                    break;
                    case 16:
                        codigo = unidade_16;
                    break;
                    case 17:
                        codigo = unidade_17;
                    break;
                    case 18:
                        codigo = unidade_18;
                    break;
                    case 19:
                        codigo = unidade_19;
                    break;
                    case 20:
                        codigo = unidade_20;
                    break;
                    case 21:
                        codigo = unidade_21;
                    break;
                    case 22:
                        codigo = unidade_22;
                    break;
                    case 23:
                        codigo = unidade_23;
                    break;
                    case 24:
                        codigo = unidade_24;
                    break;
                    case 25:
                        codigo = unidade_25;
                    break;
                    case 26:
                        codigo = unidade_26;
                    break;
                    case 27:
                        codigo = unidade_27;
                    break;
                    case 28:
                        codigo = unidade_28;
                    break;
                    case 29:
                        codigo = unidade_29;
                    break;
                    case 30:
                        codigo = unidade_30;
                    break;
                    case 31:
                        codigo = unidade_31;
                    break;
                    case 32:
                        codigo = unidade_32;
                    break;
                    default:
                        printf("Valor invalido!\n");
                }
                if(valor[0] == 0 && valor[1] == 0){
                	strcpy(unidadeEscolhida, "->TodasUnidades");
				}
                printaLCD(uniSel,unidadeEscolhida, lcd);
                delay(500);
				unidadeSelecionada = 1;
                /* Manda código*/
                writeUart(uart0_filestream,codigo);
                delay(4); // Tempo minimo para recepcao

                //respostaC = strtol(resposta[0], NULL, 10);
                /*Recebe codigo*/
                readUart(uart0_filestream,resposta);
                lcdClear(lcd); //Limpa o lcd
                printf("%d \n", respostaC);
                respostaC =resposta[0] + '0';
                printf("0x%x 0x%x 0x%x\n", resposta[0], resposta[1], resposta[2]);
                printf("%d \n", respostaC);
                printaLCDInt("Resposta_node:", respostaC, lcd);
                delay(1000);
			}
			//==========================================
			// Se o botão Next for apertado:
			else if(digitalRead(next) == LOW){
				nextValor(valor);
                lcdClear(lcd); //Limpa o lcd
                delay(300); //tempo para tirar o dedo do botão
			}
			//==========================================
		}
		/*====================== Caso uma unidade tenha sido selecionada: ======================================*/
		else if(unidadeSelecionada == 1 && opcaoSelecionada == 0){
			//Primeira opcao = Situacao atual
			if(op == 0){
				atualizaLCD(opcao0, opcao1, lcd);
				// Se o botão Previous for apertado:	
				if(digitalRead(previous) == LOW){ 
					op = 3;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				/*==========================================*/
				// Se o botão Enter for apertado:
				else if(digitalRead(enter) == LOW){
                    printaLCD(opSel, opcao0, lcd);
					opcaoSelecionada = 1;
                    codigo = situacao_atual;
                    writeUart(uart0_filestream,codigo);
                    delay(1000);

				}
				/*==========================================*/
				// Se o botão Next for apertado:
				else if(digitalRead(next) == LOW){ 
					op++;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				/*==========================================*/
			}
			//==========================================================
			//Segunda opcao = Valor Analogic
			else if(op == 1){
				atualizaLCD(opcao1, opcao2, lcd);
				if(digitalRead(previous) == LOW){
                    lcdClear(lcd); //Limpa o lcd
					op--;
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){
                    printaLCD(opSel, opcao1, lcd);
					opcaoSelecionada = 1;
                    codigo = entrada_analogica;
                    
                    /* Manda código*/
                    writeUart(uart0_filestream,codigo);
                    delay(4); // Tempo minimo para recepcao
                    
                    /*Recebe codigo*/
                    readUart(uart0_filestream,resposta);
                    dest[0] = resposta[0];
                    delay(2);
                    
                    readUart(uart0_filestream,resposta);
                    dest[1] = resposta[0];
                    delay(2);
                    
                    readUart(uart0_filestream, resposta);
                    dest[2] = resposta[0];
                    delay(2);
                    
                    readUart(uart0_filestream,resposta);
                    dest[3] = resposta[0];

                    valorAnalog = (dest[3] << 24) + (dest[2] << 16) + (dest[1] << 8) + dest[0];
                    
                    lcdClear(lcd); //Limpa o lcd
                    printaLCDInt("Resposta_node:", valorAnalog, lcd);
                    printf("Resposta_node: %d\n", valorAnalog);
                    delay(2000);
				}
				else if(digitalRead(next) == LOW){ 
					op++;
                    lcdClear(lcd); //Limpa o lcd
				}
                delay(500);
			}
			//==========================================================
			//Terceira opcao = Valor Digital
			else if(op == 2){
				atualizaLCD(opcao2, opcao3, lcd);
				if(digitalRead(previous) == LOW){
					op--;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){
                    printaLCD(opSel, opcao2, lcd);
                    delay(1000);
					opcaoSelecionada = 1;
                    codigo = entrada_digital_0;
                    /* Manda código*/
                    writeUart(uart0_filestream,codigo);
                    delay(4); // Tempo minimo para recepcao
                    /*Recebe codigo*/
                    readUart(uart0_filestream,resposta);
                    respostaC = resposta[0] + '0';
                    lcdClear(lcd); //Limpa o lcd
                    printaLCDInt("Resposta_node:", respostaC, lcd);
                    delay(2000);
				}
				else if(digitalRead(next) == LOW){ 
					op++;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
			}
			//==========================================================
			//Quarta opcao = AcendimentoLed
			else if(op == 3){
				atualizaLCD(opcao3, opcao0, lcd);	
				if(digitalRead(previous) == LOW){
					op--;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
				else if(digitalRead(enter) == LOW){
                    printaLCD(opSel, opcao3, lcd);
                    delay(500);
					opcaoSelecionada = 1;
                    codigo = acende_led;
                    /* Manda código*/
                    writeUart(uart0_filestream,codigo);
                    delay(4); // Tempo minimo para recepcao
                    /*Recebe codigo*/
                    readUart(uart0_filestream, resposta);
                    respostaC = resposta[0] + '0';
                    lcdClear(lcd); //Limpa o lcd
                    printaLCDHexa("Resposta_node:", resposta[0], lcd);
                    printf("0x%x\n",resposta);
                    delay(2000);
				}
				else if(digitalRead(next) == LOW){ 
					op = 0;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
				}
			}
			//==========================================================
		}
        //=============== Em caso de Já ter mandado uma requisição a uma unidade ============================
        else if(unidadeSelecionada == 1 && opcaoSelecionada == 1){
            if(op2 == 0){ 
                printaLCD("->Outra Unid.", "Nova Requisicao", lcd);            
                if(digitalRead(enter) == LOW){
                    opcaoSelecionada = 0;
                    unidadeSelecionada =0;
                    delay(300); //tempo para tirar o dedo do botão
                }
                else if(digitalRead(previous) == LOW){
                    op2 ++;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
                else if(digitalRead(next) == LOW){
                    op2 ++;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
            }
            else if(op2 == 1){
                printaLCD("->Requisicao", "Outra Unid.", lcd);
                if(digitalRead(enter) == LOW){
                    opcaoSelecionada = 0;
                    delay(300);
                }
                else if(digitalRead(previous) == LOW){
                    op2 --;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
                else if(digitalRead(next) == LOW){
                    op2 --;
                    lcdClear(lcd); //Limpa o lcd
                    delay(300); //tempo para tirar o dedo do botão
                }
            }
        }
    }
    close(uart0_filestream);
    return 0;
}
//===================================================================================================
/*======================================== Funcoes ================================================*/
/* Função para mostrar String na LCD */
void printaLCD(char dadoSup[],char dadoInf[], int lcd){ //ImpressÃ£o no lcd
    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
    lcdPrintf(lcd, "%s", dadoSup);

    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
    lcdPrintf(lcd, "%s", dadoInf);
}
/* Função para mostrar String na LCD */
void printaLCDHexa(char dadoSup[],char dadoInf, int lcd){ //ImpressÃ£o no lcd
    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
    lcdPrintf(lcd, "%s", dadoSup);

    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
    lcdPrintf(lcd, "0x%x", dadoInf);
}
//==========================================================
//==========================================================
/* Função para mostrar na int LCD -> Funciona como um printf */
void printaLCDInt(char dadoSup[],int valorAnalog, int lcd){ //ImpressÃ£o no lcd
    lcdPosition(lcd, 0, 0); //Seleciona a linha superior;
    lcdPrintf(lcd, "%s", dadoSup);

    lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
    lcdPrintf(lcd, "%d", valorAnalog);
}
//==========================================================
/* Função pra Escrever na UART */
void writeUart (int uart0_filestream, unsigned char dado){

    unsigned char tx_buffer[10];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];

    *p_tx_buffer++ = dado ;
    
    // Envio do TX - Uart
    int count = write (uart0_filestream, &tx_buffer[0], (p_tx_buffer-&tx_buffer[0]));
    printf("Escrevendo na UART ...\n");
    if(count<0){
        printf("Erro no envio de dados - TX\n"); 
    }
}
//==========================================================
/* Função pra Ler a UART */
void readUart(int uart0_filestream, unsigned char rx_buffer[]){

    // Recebimento do RX - Uart
    int rx_length =0 ;

    rx_length = read (uart0_filestream, (void*) rx_buffer, 1);
    if(rx_length <0){
        delay(4);
        rx_length = read (uart0_filestream, (void*) rx_buffer, 1);
    }
    else if (rx_length == 0){
        printf("Nenhum dado disponível\n");
    }
    else{
        //rx_buffer[rx_length] = '\0';
    }
    printf("tamanho:%d 0x%x\n",rx_length, rx_buffer[0]);
}
//==========================================================
/*Função para colocar a chave de seleção*/
void atualizaLCD (char fraseSup[], char fraseInf[], int lcd){
	// Variaveis auxiliares
	char selecao[16] = "->", prox[16];
	
	//Adiciona a seta na parte de selecao
	strcat(selecao,fraseSup);
	strcpy(prox, fraseInf);

    printaLCD(selecao, prox, lcd);
}
//==========================================================
/*Função para colocar a chave de seleção antes da unidade e acrescentar o valor da unidade no fim da linha*/
void atualizaLCDVetor (char fraseSup[], char fraseInf[], int valor[], int lcd){
	// Variaveis auxiliares
	int posSup = 0, posInf = 0;
	char k, l, m, n;
	char selecao[16] = "->", prox[16];
	
	//Adiciona a seta na parte de selecao
	strcat(selecao,fraseSup);
	strcpy(prox, fraseInf);
	
		
	// Percorrendo a string
	posSup = strlen(selecao);
	posInf = strlen(prox);
	
    //Frase superior
    k = valor[0] +'0'; // dezena em char
    l = valor[1] +'0'; // unidade em char
    selecao [posSup] = k;
    selecao [posSup + 1] = l;
    selecao [posSup + 2] = '\0';

    // Frase inferior
    m = valor[2] +'0'; //Unidade Futura
    n = valor[3] +'0'; //Dezena Futura
    prox [posInf] = m;
    prox [posInf + 1] = n;
    prox [posInf + 2] = '\0';
    
    if(valor[0] == 0 && valor[1] == 0){
    	strcpy(selecao, "->TodasUnidades");
	}
	else if(valor[2] == 0 && valor[3] == 0){
    	strcpy(prox, "TodasUnidades");
	}
    printaLCD(selecao, prox, lcd);
}
//==========================================================
/*Função para passar o valor do vetor*/
void nextValor(int v[]){
	//Dezena, Unidade, Dezena Futura, Unidade Futura; 
	v[1] = v[1] + 1;        
	v[3] = v[3] + 1;    
	if (v[1] >= 10){ // Se unidade chegar a 10
	    v[0]= v[0]+1; // Adicione 1 em dezena
	    v[1]= 0; // resete a unidade
	}
	else if(v[0]==3 && v[1]==3){ // Se Dezena for igual 3 e unidade for igual a 3
	    v[0] = 0; // resete dezena
	    v[1] = 0; // resete unidade
	}	
	if(v[3] >= 10){
	    v[2] = v[2]+1;
	    v[3] = 0;
	}	
	else if(v[2]==3 && v[3]==3){ // Se Dezena for igual 3 e unidade for igual a 3
	    v[2] = 0;
	    v[3] = 0;
	}
}
//========================================================================================
/*Função para voltar o valor do vetor*/
void previousValor(int v[]){
	int negativo = -1, zero =0;
	//Valor Atual
	if((v[0]) == zero && (v[1] - 1) == negativo){
	}
	else if ( (v[0]) >= zero && (v[1] - 1) >=  zero){
		v[1] = v[1] - 1;  
	}
	else if ( (v[0]) > zero && (v[1] - 1) == negativo){
		v[1] = 9;
		v[0] = v[0] - 1;
	}
	
	// Valor Futuro
	if((v[2]) == zero && (v[3] - 1) == negativo){
	}	
	else if ( (v[2]) == zero && (v[3] - 1) ==  zero){
	}
	else if ( (v[2]) >= zero && (v[3] - 1) >=  zero){
		v[3] = v[3] - 1;  
	}
	else if ( (v[2]) > zero && (v[3] - 1) == negativo){
		v[3] = 9;
		v[2] = v[2] - 1;
	}
}
/*==========================================================================================*/