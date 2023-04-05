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
   @
   @          bit = 6:4  /  Default hexa = 0x7

   ldr r6, [r8, #0x804]        @Acessar pinos com deslocamento 0x4 do endereço base
                               @Valor padrão do 0x804 é 0x77777777 = 0111 0111 0111 0111 0111 0111 0111 0111
                               @Para setar como saida o PA8 setar os bits 2:0 como 001 -> 0x77777771  
                               @000 input, 001 output
   lsl r6, r6, #4              @0x77777770 = 0111 0111 0111 0111 0111 0111 0111 0000
   add r6, #1                  @0x77777771 = 0111 0111 0111 0111 0111 0111 0111 0001
   str r6, [r8, #0x804]        @Carrega a configuração
.endm

@Macro para colocar o valor do pino PA9 como 1
.macro GPIOTurnOn
   @Configurar PA_DAT -> Valor padrão = 0x00000000
   ldr r6, [r8, #0x810]       @Acessar pinos com deslocamento 0x10 do endereço base -> PA_DAT
                              @Valor padrão do 0x810 é 0x00000000
                              @Para setar o PA8 com saida logica alta, mudar o bit 8 para 1
   add r6, #1                 @0x00000001 = 0000 0000 0000 0000 0000 0000 0000 0001
   lsl r6, r6, #8             @0000 0000 0000 0000 0000 0000 1000 0000
   str r6, [r8, #0x810]       @Carrega a configuração
.endm


@Macro para colocar o valor do pino PA9 como 0
.macro GPIOTurnOff 
   @Configurar PA_DAT -> Valor atual = 0000 0000 0000 0000 0000 0000 1000 0000
   ldr r6, [r8, #0x810]        @Acessar pinos com deslocamento 0x10 do endereço base -> PA_DAT                      
   lsr r6, r6, #9              @0000 0000 0000 0000 0000 0000 0000 0000
   str r6, [r8, #0x810]        @Carrega a configuração
.endm
