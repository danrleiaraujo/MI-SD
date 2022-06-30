/*
    Código para conversão A/D:
        Lendo sinais analógicos vindo de um potenciometro usando um ADS1115 e printando no terminal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "ads1115_rpi.h"

void main(void) {

	if(openI2CBus("/dev/i2c-1") == -1){
		return EXIT_FAILURE;
	}

	setI2CSlave(0x48);

	while(1){
		printf("CH_0 = %.2f V | ", readVoltage(0));
		printf("CH_1 = %.2f V | ", readVoltage(1));
	}

	//return EXIT_SUCCESS;
}