@Compilação :
@ as -o main.o main.s
@ ld main main.o
@ ./sudo main


.include "gpio_macros.s"

.global _start

_start:
    GPIOExport pin8
    GPIODirectionOut pin8

loop: 
    GPIOWrite pin8, high
    b loop

_end: 
    mov R0, #0   @ Usar 0 como retorno do codigo
    mov R7, #1   @ Colocar 1 em r7 para sinalizar para o SO fechar o programa
    svc 0        @ Chamada de sistema 

pin17: .asciz "8"
low: .asciz "0"
high: .asciz "1"