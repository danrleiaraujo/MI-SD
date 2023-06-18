#include<stdio.h>
#include<string.h>
#include<time.h>
#include <stdlib.h>


#define typeof(var) _Generic( (var),\
char: "Char",\
int: "Integer",\
float: "Float",\
char *: "String",\
void *: "Pointer",\
default: "Undefined")

int main(){
    
    char msg_front[100];
    char datahora[100] = "666696";
    char unidadeescolhida[] = "1";
    char valoranalog[5] = "teste";
    //msg_front = datahora + "," + unidadeescolhida + "," + "A0," + valoranalog;
    //printf("%s",msg_front);
    
    char aux[100];
    strcat(datahora, ",");
    strcat(msg_front, datahora);
    strcat(msg_front, unidadeescolhida);
    strcat(msg_front, ",A0,");
    strcat(msg_front, valoranalog);
    
     printf("\n %s",msg_front);
}
