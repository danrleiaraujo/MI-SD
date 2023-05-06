#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // A biblioteca deve definir as seguintes solicitações e argumentos para uso pelas funções fcntl () e open () .
#include <termios.h> // A biblioteca contém as definições usadas pelas interfaces de E/S do terminal 
#include <time.h>

/*
    Arquivo para testar em looping back a UART da SBC
*/

void delay(float number_of_seconds){
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void uartRasp (unsigned char dado){

    int uart0_filestream = -1; //Retorno de erro da função Open - 

    //Para abrir o open usaremos a uart 3: 0x01C28 C00
    uart0_filestream = open("/dev/ttyS3", O_RDWR | O_NOCTTY | O_NDELAY);  
    /*
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
    printf("%d",count);

    delay(1);

    // Recebimento do RX - Uart
    unsigned rx_buffer [100];
    int rx_length =0 ;
     while (rx_length <1){
        rx_length = read (uart0_filestream, (void*) rx_buffer, 100);
        if(rx_length <0){
            //printf("Erro na leitura da UART - RX\n");
            //printf ("%d",rx_length);
        }
        else if (rx_length == 0){
            printf("Nenhum dado disponível\n");
        }
        else{
            rx_buffer[rx_length] = '\0';
            printf("Mensagem de comprimento %d: %s\n", rx_length, rx_buffer);
        }
    }
    close(uart0_filestream);
}

int main() {

    unsigned char b;
    b = 0b00100001;
    //while (1){
        uartRasp(b);
    //}
    return (0);
}