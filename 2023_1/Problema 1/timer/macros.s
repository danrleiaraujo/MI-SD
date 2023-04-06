@ Chama a biblioteca de entrada e saída de arquivos
.include "file_io.s"

@ O OffSet do GPIO é 0x800 

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

.macro d6Out
   @ Setar pins do d6 como saída
   @ O OffSet dos pinos D6 e D7 0xD8
   @ Então acrescento no final ficando 0x8D8
   ldr r6, [r8, #0x8D8] @Carrego no Registrador RD o mapeamento com o offset 0x8D8
   mov r1, #7              @ em binário 001
   lsl r1, r1, #24
   bic r6, r6, r1          @ Dá um clear no r6 com os bits do r1
   mov r1, #1              @ Atualiza o valor de r1 para 001
   lsl r1, r1, #24
   orr r6, r6, r1          @ Dá um orr no bit do r6 em relação a r1 , deixando o ultimo bit 1   
   str r6, [r8, #0x8D8] @ Salvo na memória os valores dos bits modificados no Endereço da gpio com o OffSet dos pinos
.endm

.macro d7Out
   @ Setar pins do d7 como saída
   @ O OffSet dos pinos D6 e D7 0xD8
   @ Então acrescento no final ficando 0x8D8
   ldr r6, [r8, #0x8D8] @Carrego no Registrador RD o mapeamento com o offset 0x8D8
   mov r1, #7              @ em binário 001
   lsl r1, r1, #28
   bic r6, r6, r1          @ Dá um clear no r6 com os bits do r1
   mov r1, #1              @ Atualiza o valor de r1 para 001
   lsl r1, r1, #28
   orr r6, r6, r1          @ Dá um orr no bit do r6 em relação a r1 , deixando o ultimo bit 1   
   str r6, [r8, #0x8D8] @ Salvo na memória os valores dos bits modificados no Endereço da gpio com o OffSet dos pinos
.endm

.macro d4Out @PG8
   @Setando D4 como saída
   @ O OffSet dos pinos PG8 e PG9 0xDC
   ldr r6, [r8, #0x8DC]
   mov r1, #7              @ em binário 111
   bic r6, r6, r1          @ Dá um clear no r6 com os bits do r1
   mov r1, #1              @ Atualiza o valor de r1 para 001
   orr r6, r6, r1          @ Dá um orr no bit do r6 em relação a r1 , deixando o ultimo bit 1    
   str r6, [r8, #0x8DC]
.endm


.macro d5Out @PG9
   @Setar D5 como saída 
   @ O OffSet dos pinos PG8 e PG9 0xDC
   ldr r6, [r8, #0x8DC]
   mov r1, #7              @ em binário 111
   lsl r1, r1, #4 
   bic r6, r6, r1          @ Dá um clear no r6 com os bits do r1
   mov r1, #1              @ Atualiza o valor de r1 para 001
   lsl r1, r1, #4
   orr r6, r6, r1          @ Dá um orr no bit do r6 em relação a r1 , deixando o ultimo bit 1    
   str r6, [r8, #0x8DC]
.endm


.macro ersOut
   @Setar E e RS como saída
   @ São o mesmo bit para a modificação - > 000>0< 0000 -> 8
   @ O OffSet dos pinos E 0x08 e RS 0x00
   ldr r6, [r8, #0x808] @E
   ldr r5, [r8, #0x800] @RS
   
   mov r7, #7
   lsl r7, r7, #8
   bic r6, r6, r7
   bic r5, r5, r7
   
   mov r7, #7
   lsl r7, r7, #8
   orr r6, r6, r7
   orr r5, r5, r7

   str r6, [r8, #0x808] @E
   str r6, [r8, #0x800] @RS
.endm

.macro display_saida
   d4Out
   d5Out
   d6Out
   d7Out
   ersOut
.endm