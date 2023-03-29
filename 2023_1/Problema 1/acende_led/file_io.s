@ Macros para controlar arquivos
@ O parâmetro fd precisa ser um registrador.
@ Usar R0, R1, R7.
@ Os dados de retorno para o SO está em R0.

.include "sys_calls.s"

.equ O_RDONLY, 0
.equ O_WRONLY, 1
.equ O_CREAT, 0100
.equ S_RDWR, 0666       @ o valor 0666 significa que o arquivo pode ser lido e escrito por qualquer 
                        @ usuário no sistema, mas não pode ser executado como um programa;

.macro openFile fileName, flags
	ldr r0, =\fileName					  @Nome do arquivo
	mov r1, #\flags                       @Flag é como quero abrir o arquivo, leitura ou escrita, 0 ou 1 respectivamente
	mov r2, #S_RDWR 					  @ Direitos de acesso a Leitura e Escrita
	mov r7, #sys_open
	svc 0
.endm

.macro readFile fd, buffer, length
	mov r0, \fd      				    @Posição de memoria do arquivo  "descritor de arquivo"
	ldr r1, =\buffer                    @Valor lido do arquivo (Posição de memoria do valor lido)
	mov r2, #\length                    @Tamanho dos dados a serem lidos
	mov r7, #sys_read                   @Chamada de sistema para ler o arquivo
	svc 0
.endm

.macro writeFile fd, buffer, length
	mov r0, \fd                 	@Posição de memoria do arquivo  "descritor de arquivo"
	ldr r1, =\buffer				@Valor a ser escrito no arquivo (posição de memoria do valor a ser colocado)
	mov r2, \length                 @Quantidade de bytes a ser escrito no arquivo
 	mov r7, #sys_write
	svc 0
.endm

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