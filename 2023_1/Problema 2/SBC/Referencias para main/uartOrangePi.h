#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // A biblioteca deve definir as seguintes solicitações e argumentos para uso pelas funções fcntl () e open () .
#include <termios.h> // A biblioteca contém as definições usadas pelas interfaces de E/S do terminal 
#include <time.h>

/* --- BAUDRATE E TEMPO PARA TRANSMISSÃO DE DADOS ---
    BaudRate, tamanho da palavra (CSIZE) -> 9600 8-N-1
    Tempo Por Bit passado = 1/bps = 1/9600 = 0.00010416666s = ~0,105 ms
    Numeros de bits = 1 bitstart + 8 datas bit + 1 stopbit = 10bits
    Tempo para passar os 8bits = tempoPorBit * numeroDeBits = 0,10 * 0,105 = 1,05 ms = 0.00105s

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


/*  Funcao para mandar dados pelo RX    */
void writeUart (unsigned char dado){

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
    else{
        printf("Enviado");
    }
    close(uart0_filestream);
}

/*  Funcao para receber dados pela RX    */
unsigned readUart (int uart0_filestream){

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

    /* ----- Recebimento do RX - Uart ---- */
    unsigned rx_buffer [100]; //Buffer para recebimento
    int rx_length =0 ; //Tamanho do buffer em int
    // while (rx_length <1){
    rx_length = read (uart0_filestream, (void*) rx_buffer, 100); // Lê o buffer da uart com ponteiro apontando pra nulo,
                                                                // podendo receber até 100 caracteres e retorna o tamanho recebido;
    if(rx_length <0){ // Caso dê erro
        delay(0.00105); // Espera 1,05 ms que é o tempo que precisa para chegar os dados
        rx_length = read (uart0_filestream, (void*) rx_buffer, 100); //Tenta ler novamente
    }
    else if (rx_length == 0){ // Caso não tenha nada para ser lido
        printf("Nenhum dado disponível\n");
    }
    else{ // Caso não tenha nenhum problema:
        rx_buffer[rx_length] = '\0';
        printf("Mensagem de comprimento %d: %s\n", rx_length, rx_buffer);
    }
    // }
    return rx_buffer; // Retorna o buffer recebido
    
    close(uart0_filestream);
}