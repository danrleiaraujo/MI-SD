@ Chama a biblioteca de entrada e saída de arquivos
.include "file_io.s"

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

@Macro para colocar o pino PA9 como Saída:
.macro GPIODirectionOut
   @ PA Configure Register 1 (Default Value: 0x77777777)
   @ Pn_CFG1: 
   @          Port n Configure Register 1 (n from 0 to 6)
   @               n 0x77777777 = 0111 0111 0111 0111 0111 0111 0111 0111
   @          * 0x24
   @          + 0x04 (OffSet do registrador 1) 
   @          = 0x804
   @          bit = 6:4  /  Default hexa = 0x7

   ldr r6, [r8, #0x804]    @ Carrega no r6 o endereço o offset do gpio e do registrador 1
   mov r1, #7              @ em binário 111
   bic r6, r6, r1          @ Dá um clear no r6 com os bits do r1
   mov r1, #1              @ Atualiza o valor de r1 para 001
   orr r6, r6, r1          @ Dá um orr no bit do r6 em relação a r1 , deixando o ultimo bit 1
   str r6, [r8, #0x804]    @ Carrega a configuração
.endm

@Macro para colocar o valor do pino PA8 como 1
.macro GPIOTurnOn
   ldr r6, [r8, #0x810]       @Acessar pinos com deslocamento 0x10 do endereço base -> PA_DAT
   mov r1, #1
   lsl r1, r1, #8 
   bic r6, r6, r1
   orr r6, r6, r1             @
   str r6, [r8, #0x810]       @Carrega a configuração
.endm


@Macro para colocar o valor do pino PA8 como 0
.macro GPIOTurnOff 
   ldr r6, [r8, #0x810]       @Acessar pinos com deslocamento 0x10 do endereço base -> PA_DAT
   mov r1, #1
   lsl r1, r1, #8
   bic r6, r6, r1
   orr r6, r6, r1             @
   str r6, [r8, #0x810]       @Carrega a configuração
.endm
