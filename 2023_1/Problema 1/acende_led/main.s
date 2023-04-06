@ Universidade Estadual De Feira De Santana
@ Problema 1 - Tec499- Mi Sistemas Digitais
@ Código que mapeia a gpio da orange pi pc plus e habilita para enviar dados para a tela Lcd 16x2
@ Semestre 2023.1
@ Autores - Danrlei Araujo e Evelyn Suzarte

@ Compilação :
@ as -o main.o main.s
@ ld -o main main.o
@ sudo ./main

@ watch GPIO readall -> Para monitorar o GPIO

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

loop: 
    GPIOTurnOn       @ Coloca o pino em valor 1 ou seja, em sinal alto
    nanoSleep        @ Função para esperar
    GPIOTurnOff      @ Coloca o pino em valor 0 ou seja, em sinal baixo
    nanoSleep        @ Função para esperar
    b loop           @ Volta para função Loop

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
    @  0x01C20800_ (Endereço)  / 4096 (pela quantidade de páginas) = 0x01C20
    gpioaddr: .word 0x01C20

    @ Segundo o dataSheet o pino por padrão vem 0x77777777
    padraoPin: .word 0x77777777

    @ Tamanho da Página do mapeamento = 4096
    pagelen: .word 0x1000

    @ OffSet do registrador que está o PA8
    @ pinSaida: .word 0x804

    @ Offset para PA_DATA - > Registrador para ligar o pino
    @ pin: .word 0x810

    @ OffSet em vetor - > Pode ser dessa forma mas acaba utilizando memória e é uma espécie de armengue.
    @ PA8: 
        @.word 4
        @.word 0
        @.word 8
        @.word 0x10