/*  Desenvolvido por:
    Danrlei Almeida Araujo
    Evelyn Suzarte
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "codigoUartRasp.h"


void main() {

    int resposta=0;
    unsigned char teste;
    setlocale(LC_ALL,"Portuguese");
    
    
    while(resposta == 0){
        printf("Bem vindo ao nosso PBL MI de SD:\n");
        printf("Digite o que deseja:\n");
        printf("1 - Situação atual do sensor\n");
        printf("2 - Temperatura:\n");
        printf("3 - Umidade:\n");
        scanf("%d",&resposta);
        if(resposta == 1){
            teste = '3';
            printf("Você selecionou Situação Atual do Sensor! = 0x0%c", teste);
        }
        else if(resposta == 2){
            teste = '4';
            printf("Você selecionou Temperatura! = 0x0%c", teste);
        }
        else if(resposta == 3){
            teste = '5';
            printf("Você selecionou Umidade! = 0x0%c", teste);
        }
        else{
            printf("Erro - Digite um comando válido!\n\n");
            getchar ();
        }
    }
    uartRasp(teste);

}