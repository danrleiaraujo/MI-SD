#include<stdio.h>
#include<stdlib.h>

void printaLCD(char dadoSup[],char dadoinf[]){ //Impressão no lcd
        int lcd;

        //lcdPosition(lcd, 0, 0); //Seleciona a linha superior; lcd
		printf( "%s", dadoSup);
		printf( "%s", dadoinf);
/*
        lcdPosition(lcd, 0, 1);//Seleciona a linha inferior;
        lcdPrintf(lcd, "%s", dadoInf);*/
}

main(void){
	printaLCD("Linda","BB");
}
