<div id="inicio">
    <h1 id="titulo" align="center"> Problema 2 da disciplina MI - Sistemas Digitais.</h1>
	<h2 id="titulo" align="center"> Interfaces de E/S</h1>
	<p id="descricao" align="justify"></p>
</div>

<div id="sumario">
    <h1>Sumário</h1>
	<ul>
		<li><a href="#inicio"> <b>Início</b></li>
        <li><a href="#equipe"> <b>Equipe de Desenvolvimento</b></li>
		<li><a href="#recursos-utilizados"> <b>Recursos Utilizados</b> </a></li>
        <li><a href="#requisitos"> <b>Requisitos Atendidos</b> </a> </li>
		<li><a href="#implementacao"> <b>Implementação</b> </a> </li>
        <li><a href="#metodologia"> <b>Metodologia</b> </a> </li>
	</ul>	
</div>

<div id="equipe">
    <h1>Equipe de Desenvolvimento</h1>
    <ul>
		<li><a href="https://github.com/danrleiaraujo"> Danrlei Almeida Araujo</li>
		<li><a href="https://github.com/evelynsuzarte"> Evelyn Suzarte Fernandes</a></li>
	</ul>
    <h1>Tutor</h1>
    <ul>
        <li><a href="https://github.com/">Anfranserai Morais Dias</a></li>
    </ul>
</div>

<div id="recursos-utilizados">
	<h1> Recursos Utilizados </h1>
	<ul>
        <li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%201/Referencias/Raspberry_Pi_Assembly_Language_Programming_(Stephen_Smith)_(z-lib.org)_(2).pdf">Raspberry Pi Assembly Language Programming</a></li>
		<li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%201/Referencias/DataSheet%20do%20ORANGE%20PI.pdf">DataSheet do processador</a></li>
		<li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%201/Referencias/DataSheet%20do%20LCD.pdf">DataSheet do LCD</a></li>
	</ul>	


<div id="requisitos">
	<h1>Pré-requisitos</h1>
	<h3>Antes de começar, você vai precisar ter:</h3>
	<ul>        
		<li><a href="https://www.diktio-kapa.dos.gr/OrangePi_PcPlus_Android_4.4/OrangePi%20PC%20Plus_H3%20User%20Manual_v0.9.4.pdf">Orange Pi PC PLUS</a></li>
        <li><a>Tela LCD 16x2</a></li>
        <li><a>NodeMCU</a></li>
        <li><a href="https://components101.com/sites/default/files/component_datasheet/Push-Button.pdf">Button</a></li>
        <li><a href="https://www.farnell.com/datasheets/1498852.pdf">Led</a></li>
		<li>Uma protoboard.</li>
		<li>Um jumper para ligar a GPIO na protoboard.</li>
		<li>Um potênciometro para regular o LCD.</li>
	</ul>
</div>

<div id="requisitos">
    <h1>Requisitos Atendidos</h1>
	<ul>
		<li>Sequência de instruções em linguagem C :heavy_check_mark:</li>
		<li>Códigos comentados:heavy_check_mark:</li>
		<li>Script de compilação tipo Makefile para geração do código executável :heavy_check_mark:</li>
		<li>Documentação no READ.ME do projeto no GitHub:heavy_check_mark:</li>
		<li>Capacidade de interligação com até 32 unidades de sensoriamento:heavy_multiplication_x:</li>
		<li>Mecanismo de controle de status de funcionamento das unidades:heavy_multiplication_x:</li>
		<li>Apresentação na LCD :heavy_multiplication_x:</li>
	</ul>
</div>

<div id="implementacao">
	<h1>Implementação</h1>
	<p>
		<li>
			Para implementação do código foi utilizado a linguagem assembly.
		</li>
	</p>
	<h2>Para o funcionamento da led</h2>
	<p align="justify"> 
		Foram criados alguns arquivos com macros necessários para o acesso ao pino e a manipulação do mesmo, são eles:
		<ul>
			<li>sys_calls.s</li>
			<h4 align="justify"> O Arquivo possuia criação de constantes com os valores respectivos das SysCalls. As System Calls, como são chamadas, são divididas em quatro grupos: chamadas de sistema para gerenciamento de processos, para gerenciamento de diretórios, para gerenciamento de arquivos e restantes. Essas funções servem para ser o sistema ter permissões de acesso a recursos que o usuário comum não tem acesso. </h4>
			<li>file_io.s</li>
			<h4 align="justify"> Nesse arquivo existem instruções para a manipulação de arquivos, macros como leitura, escrita, abertura e fechamento de arquivo. Nele é chamada as constantes criadas do arquivo "sys_calls" para acesso a manipulação de arquivos. </h4>
			<li>gpio_macros.s:</li>
			<h4> Dentro desse arquivo existem macros para a configuração dos pinos da GPIO, sendo eles: 
			<ul>
				<li>openDevmem</li>
					<ul><li>Serve para abrir o devmen e nos dar os direitos de leitura e escrita de arquivos.</ul></li>
				<li>nanoSleep</li>
					<ul><li>Um macro para fazer o sistema entrar em um sleep porém com seu tempo em nanosegundo, para deixar a medição de tempo mais precisa.</ul></li>
				<li>mapMem</li>
					<ul><li>Macro para mapear o endereço virtual dos pinos.</ul></li>	
				<li>directionOut</li>
					<ul><li>Macro para direcionar o pino para saída.</ul></li>
				<li>turnOn</li>					
					<ul><li>Macro para ligar o pino.</ul></li>
				<li>turnOff</li>
					<ul><li>Macro para desligar o pino.</ul></li>
				<li>main</li>
				<h4>É o arquivo principal onde fazemos as chamadas dos macros.</h4>
			</ul>
		</ul>
    </p>
	<h2>Para o funcionamento do LCD:</h2>
	<p align="justify"> 
		Foi ultilizado o mesmo raciocínio que para acender a led, sendo assim, apenas dois arquivos são diferentes:
		<ul>
			<li>macros.s:</li>
				<h4> Possui todos os macros do gpio_macros.s com exceção de "directionOut", "turnOn" e "turnOff" que foram substituídos por:
				<ul>
					<li>d4Out</li>
					<li>d5Out</li>
					<li>d6Out</li>
					<li>d7Out</li>
					<li>ersOut</li>
						<ul><li>Macro para direcionar os pinos da LCD para saída.</ul></li>
				</ul>
			<li>main_timer.s</li>
			<h4>É o arquivo principal onde fazemos as chamadas dos macros.</h4>
		</ul>
	</p>
</div>

<div id="metodologia">
	<h1>Metodologia</h1>
	<p align="justify">
		Para a realização do problema, nossa turma teve que se basear no livro "Raspberry Pi Assembly Language Programming" do autor Stephen Smith, já que não tinha uma referência para a própria Orange PI PC Plus. Sendo assim, usamos exemplos e referências do livro com algumas modificações de endereços e offSet.
		Para a noção de comportamento da GPIO, tivemos que ler e entender o DataSheet do processador. Para entendermos como funcionava o LCD, também tivemos que ler e entender o datasheet do mesmo.
		<h2>Ligando a led</h2>
		<p align="justify">
			Sabíamos que precisavamos antes de mais nada entender o comportamento dos pinos da GPIO e como configura-los da melhor forma, sendo assim, entramos em consenso em sessão que precisariamos realizar ligação de uma led que estava conectada em um dos pinos (Podendo ser tanto PA8 ou PA9 que possui lógica invertida).
			Para isso tivemos que seguir alguns passos:
			<ul align="justify">
				<li>
					Primeiro temos que conseguir o direito de leitura e escrita de arquivo, pois em linux, tudo é arquivo. Sendo assim, quando nós acessamos o pino, na verdade estamos acessando um arquivo que o linux gera para a representação do mesmo.
				</li>
				<li>
					Em seguida, precisamos mapear os pinos da GPIO, que é basicamente guardar em um registrador os bits de um determinado local da memória, através do endereço dividido por 4096, que é o tamanho da páginação do mapeamento.
				</li> 
				<li>
					Posteriormente temos que transformar os pinos em saída, para isso temos que acessar o endereço do GPIO com o offSet do GPIO (0x800) somado ao offSet do registador que se encontra o pino (0x04), como queremos o PA8, temos que mudar 3 bits da sequência que recebemos, que são de 2-0, mudando-os e transformando em 001.
				</li> 
				<li>
					O ultimo passo é ligar e desligar o pino, para isso existe um registrador chamado PA DATA que tem como offSet 0x10, o comportamento dele é um pouco diferente, agora na sequência de bits, cada bit representa um PA, por exemplo, queremos ligar o PA8, sendo assim, temos que mudar o oitavo bit do menos significativo pro mais significativo para 1 quando queremos ligar ou 0 quando queremos desligar esse pino.
				</li>
			</ul>
		</p> 
		<h2>LCD</h2>
		<p align="justify"> 
			Após nosso aprendizado sobre os comportamentos da GPIO, partimos então para o que realmente interessava, que era inicializar e mandar dados para o LCD. 
			Como o LCD também estava ligado no GPIO então agora ficaria fácil, chegamos a criar macros funcionais onde muda o comportamento dos pinos conectados, deixando-os como saída, mas na parte de mandar bit deixamos a desejar.
		</p> 
	</p>
</div>

<div id="conclusoes">
	<h1>Conclusões</h1>
	<p align="justify"> 
	Entendemos como é o funcionamento de uma SBC em baixo nível e como devemos manipular seus registradores para ter acesso a uma determinada parte através da linguagem Assembly para a arquitetura ARMV7. Apesar do problema ser extendido e o professor ter nos dado mais prazo do que foi definido inicialmente, não conseguimos concluir nosso problema. Acreditamos que isso deu-se por conta das baixas quantidades referências, do tempo que nossa turma tinha acesso a placa para a realização de testes e por dificuldade em entendimento do DataSheet do processador principalmente na parte do OffSet. 
	</p>
</div>

### 🎲 Rodando o código:

```bash
# Clone este repositório
$ git clone <https://github.com/danrleiaraujo/MI-SD/tree/main/2023_1/Problema%201>

#Para o uso do led:
# Acesse a pasta do projeto no terminal/cmd
$ cd acende_led

#Dê o comando para compilar:
$ make
		
#Dê o comando para Rodar:
$ sudo ./main

#Para o uso do LCD:
# Acesse a pasta do projeto no terminal/cmd
$ cd timer

#Dê o comando para compilar:
$ make
		
#Dê o comando para Rodar:
$ sudo ./main_timer


