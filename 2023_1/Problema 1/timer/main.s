@ Universidade Estadual De Feira De Santana
@ Problema 1 - Tec499- Mi Sistemas Digitais
@ Código que mapeia a gpio da orange pi pc plus e habilita para enviar dados para a tela Lcd 16x2
@ Semestre 2023.1
@ Autores - Danrlei Araujo e Evelyn Suzarte

@ Compilação :
@ as -o main.o main.s
@ ld -o main main.o
@ sudo ./main


@ Tabela de Bits - Asci
@ 0011 0000 - 0
@ 0011 0001 - 1
@ 0011 0010 - 2
@ 0011 0011 - 3
@ 0011 0100 - 4
@ 0011 0101 - 5
@ 0011 0110 - 6
@ 0011 0111 - 7
@ 0011 1000 - 8
@ 0011 1001 - 9

@Clear display
@ RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
@  0  0   0   0   0   0   0   0   0   1



@ watch GPIO readall -> Para monitorar o GPIO

@ Endereço da Gpio 0x01C20800_

@ Constantes
.equ PROT_READ, 1
.equ PROT_WRITE, 2
.equ MAP_SHARED, 1


.include "gpio_macros.s"

.global _start

_start:
    openDevmem       @Abre o Devmen para abertura de arquivos
    mapMem           @ Mapeia a memória para acesso dos registradores 
    movs r8, r0      @ Joga o mapeamento em r8
    GPIODirectionOut @ Direciona o pino para Saída



_end: 
    mov r0, #0   @ Usa 0 como retorno do codigo
    mov r7, #1   @ Move 1 em r7 para sinalizar para o S.O. fechar o programa
    svc 0        @ Comando do linux para terminar
    

@Variáveis    
.data
    @ Tempo em Segundo
    timespec: .word 1

    @ Tempo em Nanosegundo
    timespecnano: .word 1000000000 @ = 1s

    @ Caminho do Devmen
    devmem: .asciz "/dev/mem"
    
    @ Endereço de memória dos registradores da GPIO 
    @  0x01C20800 (Endereço)  / 4096 (pela quantidade de páginas) = 0x01C20
    gpioaddr: .word 0x01C20

    @ Segundo o dataSheet o pino por padrão vem 0x77777777
    padraoPin: .word 0x77777777

    @ Padrão do pino para saida:
    @ padraoSaidaPin: .word 0x00000000

    @ Tamanho da Página do mapeamento = 4096
    pagelen: .word 0x1000


    @.align 4 @ Tamanho em bytes de dados


     OffSet: 
        .word 4
        .word 0 @ offset to select register
        .word 8 @ bit offset in select register
        .word 0x10 @ bit offset in set & clr register