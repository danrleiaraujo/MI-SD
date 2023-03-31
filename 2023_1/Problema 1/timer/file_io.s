@ Macros para manipular arquivos
@ O parâmetro fd precisa ser um registrador.
@ Usar R0, R1, R7.
@ O código de retorno está R0.

@ Biblioteca com constantes para chamadas do sistema
.include "sys_calls.s"

@ Constantes
.equ O_RDONLY, 0
.equ O_WRONLY, 1
.equ O_CREAT, 0100
.equ S_RDWR, 0666       @ o valor 0666 significa que o arquivo pode ser lido e escrito por qualquer 
                        @ usuário no sistema, mas não pode ser executado como um programa;

@ Macro para abrir um arquivo, recebe o nome do arquivo e flags para saber como vai ser aberto o arquivo
.macro openFile fileName, flags
	ldr r0, =\fileName					  @ Nome do arquivo
	mov r1, #\flags                       @ Leitura ou escrita, 0 ou 1 respectivamente
	mov r2, #S_RDWR 					  @ Direitos de acesso a Leitura e Escrita
	mov r7, #sys_open 				 	  @ Chama a sysCall para abrir o arquivo
	svc 0								  @ Informa ao linux que acabou os comandos
.endm

@ Macro para leitura do arquivo, recebe o valor do arquivo e o tamanho dele:
.macro readFile fd, buffer, length
	mov r0, \fd      				    @ Posição de memoria do arquivo  "descritor de arquivo"
	ldr r1, =\buffer                    @ Valor lido do arquivo (Posição de memoria do valor lido)
	mov r2, #\length                    @ Tamanho dos dados a serem lidos
	mov r7, #sys_read                   @ Chamada de sistema para ler o arquivo
	svc 0 						  		@ Informa ao linux que acabou os comandos
.endm

@ Macro para escrever o arquivo, recebe a descrição do arquivo, o valor dele e o tamanho
.macro writeFile fd, buffer, length
	mov r0, \fd                 	@ Recebe em R0 a descrição do arquivo
	ldr r1, =\buffer				@ Recebe em R1 o valor a ser escrito no arquivo (posição de memoria do valor a ser colocado)
	mov r2, \length                 @ Recebe em R1 a quantidade de bytes a ser escrito no arquivo
 	mov r7, #sys_write              @ Chamada de sistema para escrever o arquivo
	svc 0 						    @ Informa ao linux que acabou os comandos
.endm

@ Macro para salvar as as modificações e fechar o programa, recebe a descrição do arquivo
.macro flushClose fd      @Salvar as modificações no disco rigido e não só na memoria temporaria
	@fsync syscall
	mov r0, \fd
	mov r7, #sys_fsync     @Chamada de sistema para sincronizar os dados com o arquivo
	svc 0

	@close syscall        
	mov r0, \fd
	mov r7, #sys_close     @Chamada de sistema para fechar o arquivo
	svc 0
.endm