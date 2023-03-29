@ Macros para acessar os pinos do GPIO


.include "file_io.s"

@ Macro nanoSleep para esperar 0.1 segundo
@ Chama o serviço nanosleep do Linux, que é funcão 162.
@ Passe uma referência para uma especificação de tempo em r0 e r1
@ Primeiro é o tempo de entrada para esperar em segundos e nanossegundos.
@ Segundo é o tempo restante para esperar se for interrompido

.macro nanoSleep
   ldr r0, =timespecsec
   ldr r1, =timespecsec
   mov r7, #sys_nanosleep
   svc 0
.endm

.macro GPIOExport pin
   openFile gpioexp, O_WRONLY          @O_WRONLY está em fileio.s, é a a flag para escrever em arquivo
   mov r8, r0                          @ Salva o nome do arquivo
   writeFile r8, \pin, #2              @Escreve o valor 2, no pino passado por "parametro" e passa o nome do arquivo a ser modificado (r8)
   flushClose r8                       @Salva as alterações no disco rigido
.endm

.macro GPIODirectionOut pin       @Definir input ou output
   ldr r1, =\pin            @copiar pin no padrão de nome de arquivo
   ldr r2, =gpiopinfile     @Caminho para setar output ou input no pino
   add r2, #20              @Adiona 20 ao endereço de r2 para poder acessar um offset especifico
   ldrb r3, [r1], #1        @ Carrega o valor do pino no R3 e soma 1
   strb r3, [r2], #1        @ Carrega o valor de r3 em r2 e adiciona 1
   ldrb r3, [r1]             
   strb r3, [r2]            
   
   openFile gpiopinfile, O_WRONLY         @Abrir arquivo com tipo escrita
   mov r8, r0                             @ Salvar o nome do arquivo em r8
   writeFile r8, outstr, #3               @Escrever o valor 'out' no arquivo de pino 
   flushClose r8                          @Sincronizar alterações
.endm

.macro GPIOWrite pin, value   @Escrever valor no pino
   ldr r1, =\pin              @copiar pin no padrão de nome de arquivo
   ldr r2, =gpiovaluefile     @Caminho do arquivo para setar o valor
   add r2, #20
   ldrb r3, [r1], #1          @ Carrega o valor do pino no R3 e soma 1
   strb r3, [r2], #1          @ Carrega o valor de r3 em r2 e adiciona 1
   ldrb r3, [r1]
   strb r3, [r2]
   
   openFile gpiovaluefile, O_WRONLY          @Abrir arquivo com tipo escrita
   mov r8, r0                                @ Salvar o nome do arquivo em r8
   writeFile r8, \value, #1                  @Escrever o valor 1 ou 0 no arquivo de saida
   flushClose r8                             @Sincronizar alterações
.endm

.data
  timespecsec: .word 0
  timespecnano: .word 100000000
  gpioexp: .asciz "/sys/class/gpio/export"
  gpiopinfile: .asciz "/sys/class/gpio/gpioxx/direction"
  gpiovaluefile: .asciz "/sys/class/gpio/gpioxx/value" 
  outstr: .asciz "out"
          .align 2      @ save users of this file having to do this.