#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

void uartRasp (unsigned char teste){

    int uart0_filestream = -1; //Retorno de erro da função Open - 

    //Para abrir o open usaremos a uart 0:
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);  
    /*
    O_RDWR -> Lê e escreve
    O_NOCTTY -> Identifica o dispositivo como dispositivo de terminal
    O_NDELAY -> Sem delay, para acesso imediato
    */

    if (uart0_filestream == -1){ //Verifica se deu erro na abertura da UART
        printf("Erro na abertura da UART\n");
    }

    /* Configuração da uart*/
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD; //BaudRate, tamanho da palavra
    options.c_iflag = IGNPAR; //ignora a paridade
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[10];
    unsigned char *p_tx_buffer;

    p_tx_buffer = &tx_buffer[0];

    *p_tx_buffer++ = teste ;
    
    /*
    *p_tx_buffer++ ='O';
    *p_tx_buffer++ ='l';
    *p_tx_buffer++ ='a';
    *p_tx_buffer++ =' ';
    *p_tx_buffer++ ='M';
    *p_tx_buffer++ ='u';
    *p_tx_buffer++ ='n';
    *p_tx_buffer++ ='d';
    *p_tx_buffer++ ='o';
    */

    // Envio do TX - Uart
    int count = write (uart0_filestream, &tx_buffer[0], (p_tx_buffer-&tx_buffer[0]));
    printf("Escrevendo na UART ...\n");
    if(count<0){
        printf("Erro no envio de dados - TX\n"); 
    }

    // Recebimento do RX - Uart
    unsigned rx_buffer [100];
    int rx_length = read (uart0_filestream, (void*) rx_buffer, 100);
    if(rx_length <0){
        printf("Erro na leitura da UART - RX\n");
    }
    else if (rx_length == 0){
        printf("Nenhum dado disponível\n");
    }
    else{
        rx_buffer[rx_length] = '\0';
        printf("Mensagem de comprimento %d: %s\n", rx_length, rx_buffer);
    }

    close(uart0_filestream);
}