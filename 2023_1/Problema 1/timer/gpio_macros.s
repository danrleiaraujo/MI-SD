@ Chama a biblioteca de entrada e saída de arquivos
.include "file_io.s"

@ Configuração de pinos do DISPLAY:
@  Pinos Enable e Seleção de comandos:
@     RS   -> PA2 : Pino -> 10:08 : Bits de configuração -> 0x00 : Offset
@     E    -> PA18 : Pino -> 10:08 : Bits de configuração -> 0x08 : Offset

@  OffSet para ligar ou desligar o pino:
@     PA_DATA -> 0x10 -> Bits para mandar sinal alto ou baixo pro pino

@  Pinos digitais:
@     D5   -> PG9 : Pino -> 06:04 : Bits de configuração -> 0xDC : Offset
@     D4   -> PG8 : Pino -> 02:00 : Bits de configuração -> 0xDC : Offset
@     D6   -> PG6 : Pino -> 26:24 : Bits de configuração -> 0xD8 : Offset
@     D7   -> PG7 : Pino -> 30:28 : Bits de configuração -> 0xD8 : Offset
@     PG_DATA -> 0xE8 -> Bits para mandar sinal alto ou baixo pro pino

@  OffSet para ligar ou desligar o pino:
@     PG_DATA -> 0xE8 : Offset -> Bits para mandar sinal alto ou baixo pro pino

@ Macro para abrir o Devmen
.macro openDevmem
   ldr r0, =devmem       @ Caminho do devmem, /dev/mem
	mov r1, #2            @
	mov r2, #S_RDWR       @ Direitos de acesso a Leitura e Escrita
	mov r7, #5            @ sys_open
	svc 0                 @ Indica ao linux que acabou os comandos
.endm

@ Macro para esperar em nanosegundos:
.macro nanoSleep
    ldr r0, =timespecnano        @ Passa a referencia de espera para r0 e r1 
    ldr r1, =timespecnano        @
    mov r7, #162                 @ Chama a função de nanosleep do sistema -> sys_nanosleep
    svc 0                        @ Indica ao linux que acabou os comandos
.endm


@ Macro para mapear a memória dos registradores da GPIO
.macro mapMem
   mov r4, r0            @ Move descritor do arquivo de r0 para r4
   mov r0, #0            @ Deixar o SO escolher o mapeamento
   ldr r1, =pagelen      @ Tamanho da memoria de paginação
   ldr r1, [r1]          @ Carrega o valor no r1
   mov r2, #(PROT_READ + PROT_WRITE)      @Proteção de leitra e escrita, valor 3 
   mov r3, #MAP_SHARED   @ Valor de MAP_SHARED é 1
   ldr r5, =gpioaddr     @ Valor base GPIO dividido por 0x1000
   ldr r5, [r5]          @ Carregar valor em r5
   mov r7, #192          @ 192 é a syscall para sys_mmap2
   svc 0
.endm

.macro pins_display_saida
   @ Setar pins do d7 e d6 como saída
   @ O OffSet do GPIO é 0x800 
   @ O OffSet dos pinos D6 e D7 0xD8

   @ Então acrescento no final ficando 0x8D8
   ldr r6, [r8, #0x8D8] @Carrego no Registrador RD o mapeamento com o offset 0x8D8
   mov r7, #0xFF        @ Mando o valor ff para o registrador R7 ficando "000000ff"
   lsl r7, r7, #24      @ Movo o valor para esquerda em 24bits que são os bits do D6 "ff000000"
   bic r6, r6, r7       @ Dou um AND de bit a bit com o R6 e o R7 dando um clear nos bits deixando "00aaaaaa" (a é qualquer valor)
   mov r7, #0x11        @ Movo o valor 11 para r7 deixando-o em 00000011
   lsl r7, r7, #24      @ movo 24bits para esquerda deixando-o 11000000
   orr r6, r6, r7       @ Dou um ORR em R6 e R7 transformando R6 em 11aaaaaa
   str r6, [r8, #0x8D8] @ Salvo na memória os valores dos bits modificados no Endereço da gpio com o OffSet dos pinos


   @Setar d5 e d4 como saida
   @ O OffSet dos pinos D8 e D9 0xDC
   ldr r6, [r8, #0x8DC]
   mov r7, #0xFF
   bic r6, r6, r7
   mov r7, #0x11
   orr r6, r6, r7
   str r6, [r8, #0x8DC]


   @Setar E como saida
   @ O OffSet dos pinos E 0x08 e RS 0x00
   ldr r6, [r8, #0x808] @E
   ldr r5, [r8, #0x800] @RS
   
   mov r7, #0xF00
   bic r6, r6, r7
   bic r5, r5, r7
   
   mov r7, #0x100
   orr r6, r6, r7
   orr r5, r5, r7

   str r6, [r8, #0x808] @E
   str r6, [r8, #0x800] @RS
.endm