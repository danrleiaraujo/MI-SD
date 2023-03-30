@Compilação :
@ as -o main.o main.s
@ ld -o main main.o
@ sudo ./main
@ Gpio 0x01C20800
@ Macros para acessar os pinos do GPIO
@ Constantes
@.equ pagelen, 4096
.equ setregoffset, 28
.equ clrregoffset, 40
.equ PROT_READ, 1
.equ PROT_WRITE, 2
.equ MAP_SHARED, 1


.include "gpio_macros.s"

.global _start

_start:
    openDevmem
    mapMem
    movs r8, r0     @Jogar o mapeamento em r8
    GPIODirectionOut @pinSaida

loop: 
    GPIOTurnOn @pin
    nanoSleep
    GPIOTurnOff @pin
    Sleep
    b loop

_end: 
    mov r0, #0   @ Usar 0 como retorno do codigo
    mov r7, #1   @ Colocar 1 em r7 para sinalizar para o SO fechar o programa
    svc 0        @ Chamada de sistema 
    
.data
timespec: .word 1
timespecnano: .word 100000000
devmem: .asciz "/dev/mem"
@ mem address of gpio register 0x01C20800  / 4096 = 0x01C20
gpioaddr: .word 0x01C20
padraoPin: .word 0x77777777
padraoSaidaPin: .word 0x00000000
pagelen: .word 0x1000
pinSaida: .word 0x804
pin: .word 0x810
.align 4 @ realign after strings
