/*

Tabela 1 – Exemplos de requisição.
Código Descrição do comando
0x01 Solicita a situação atual da unidade de sensoriamento
0x11 Solicita o valor da entrada analógica
0x12 Solicita o valor de uma das entradas digitais
0x21 Acendimento do led da NodeMCU

--------------------------NodeMCU--------------------------------
#include <bibliotecas>
#define pinos

int main(){
    if (requisicao = 0x01){
        return situacao_Sensor
    }
    else if(requisicao = 0x11){
        return entrada_Analogica

    }
    else if(requisicao = 0x12){
        return entrada_digital
        
    }
    else if(requisicao = 0x21){
        AcendeLed();
    }

}
--------------------------------------------------------------------

Tabela 2 – Exemplos de resposta.
Código Descrição
0x01 NodeMCU com problema
0x02 NodeMCU funcionando normalmente
0x11 Medida da entrada analógica
0x12 Estado da entrada digital

--------------------------------SBC----------------------------------

#include <bibliotecas>
#define pinos

int main(){
    if (resposta = 0x01){
        print("NodeMCU com problema");
    }
    else if(resposta = 0x11){
        print("NodeMCU funcionando normalmente");

    }
    else if(resposta = 0x12){
        print("Entrada Analogica:", entrada);
        
    }
    else if(resposta = 0x21){
        print("Entrada Digital:", entrada);
    }

}

*/