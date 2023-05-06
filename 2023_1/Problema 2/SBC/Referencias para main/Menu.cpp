#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void atualizaLCD (char fraseSup[], char fraseInf[]);
void atualizaLCD (char fraseSup[], char fraseInf[], int valor[]);
void nextValor(int v[]);
void previousValor(int v[]);

int main(int argc, char *argv[]) {
	int valor[5] = {0,0,0,1}; //Dezena, Unidade, Dezena Futura, Unidade Futura; 
	int op = 0, t = 0; //Opcao e entradada de teclado
	char uniSel[16] = "Uni_Selecionada", opSel[16] = "Op_Selecionada";
	char unidade[16]= "Unidade = ";
    char opcao0[16] = "Situacao atual", opcao1[16]="Valor Analogic"; 
    char opcao2[16] = "Valor Digital", opcao3[16] ="AcendimentoLed";
    char digital[16] = "Digital = ";
    bool unidadeSelecionada = false, opcaoSelecionada = false; 
	while(1){
		// Se nao foi selecionada uma unidade: ==============================================================
		if(unidadeSelecionada == false){
			atualizaLCD(unidade, unidade, valor); //Inicia o LCD

			/* Vai ser substituido pela entrada de botão*/
			scanf("%d",&t);
			fflush(stdin);
			/*==========================================*/
			// Se o botão Previous for apertado:
			if(t == 1){
				previousValor(valor);
			}
			//==========================================
			// Se o botão Enter for apertado:
			else if(t == 2){
				printf("%s\n", uniSel);
				printf("%d%d\n", valor[0], valor[1]);
				unidadeSelecionada = true;
			}
			//==========================================
			// Se o botão Next for apertado:
			else if(t == 3){
				nextValor(valor);
			}
			//==========================================
		}
		// Caso uma unidade tenha sido selecionada: ============================================================
		else if(unidadeSelecionada == true){
			//Primeira opcao = Situacao atual
			if(op == 0 && opcaoSelecionada == false){
				atualizaLCD(opcao0, opcao1);
				/* Vai ser substituido pela entrada de botão*/
				scanf("%d",&t);
				fflush(stdin);		
				/*==========================================*/
				// Se o botão Previous for apertado:	
				if(t == 1){ 
					op = 3;
				}
				/*==========================================*/
				// Se o botão Enter for apertado:
				else if(t == 2){
					printf("%s\n", opSel);
					printf("%s\n", opcao0);
					opcaoSelecionada = true;
				}
				/*==========================================*/
				// Se o botão Next for apertado:
				else if(t == 3){ 
					op++;
				}
				/*==========================================*/
			}
			//==========================================================
			//Segunda opcao = Valor Analogic
			else if(op == 1 && opcaoSelecionada == false){
				atualizaLCD(opcao1, opcao2);
				scanf("%d",&t);
				fflush(stdin);
				if(t == 1){
					op--;
				}
				else if(t == 2){
					printf("%s\n", opSel);
					printf("%s\n", opcao1);
					opcaoSelecionada = true;
				}
				else if(t == 3){
					op++;
				}
			}
			//==========================================================
			//Terceira opcao = Valor Digital
			else if(op == 2 && opcaoSelecionada == false){
				atualizaLCD(opcao2, opcao3);
				scanf("%d",&t);
				fflush(stdin);				
				if(t == 1){
					op--;
				}
				else if(t == 2){
					printf("%s\n", opSel);
					printf("%s\n", opcao2);
					opcaoSelecionada = true;
				}
				else if(t == 3){
					op++;
				}
			}
			//==========================================================
			//Quarta opcao = AcendimentoLed
			else if(op == 3 && opcaoSelecionada == false){
				atualizaLCD(opcao3, opcao0);
				scanf("%d",&t);
				fflush(stdin);				
				if(t == 1){
					op--;
				}
				else if(t == 2){
					printf("%s\n", opSel);
					printf("%s\n", opcao3);
					opcaoSelecionada = true;
				}
				else if(t == 3){
					op = 0;
				}
			}
			//==========================================================
		}

		
	}
    return 0;
}
void atualizaLCD (char fraseSup[], char fraseInf[]){
	// Variaveis auxiliares
	int posSup = 0, posInf = 0;
	char k, l, m, n;
	char selecao[16] = "->", prox[16];
	int tamanho = 0;
	
	//Adiciona a seta na parte de selecao
	strcat(selecao,fraseSup);
	strcpy(prox, fraseInf);

	printf("%s\n",selecao);
	printf("%s\n",prox);
}

void atualizaLCD (char fraseSup[], char fraseInf[], int valor[]){
	// Variaveis auxiliares
	int posSup = 0, posInf = 0;
	char k, l, m, n;
	char selecao[16] = "->", prox[16];
	int tamanho = 0;
	
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
    
	printf("%s\n",selecao);
	printf("%s\n",prox);
}

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
