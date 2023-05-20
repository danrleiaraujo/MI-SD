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
		<li><a href="#conclusao"> <b>Resultados e Conclusões</b> </a> </li>
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
		<li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%201/Referencias/DataSheet%20do%20ORANGE%20PI.pdf">DataSheet do processador</a></li>
		<li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%201/Referencias/DataSheet%20do%20LCD.pdf">DataSheet do LCD</a></li>
		<li><a href="https://github.com/danrleiaraujo/MI-SD/blob/110265040c0de46d0c557e626c53a32831574807/2023_1/Problema%202/Referencias/DataSheet_Esp.pdf">DataSheet do NodeMCU</a></li>
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
		<li>Uma protoboard</li>
		<li>Jumpers</li>
		<li>Dois potênciometros</li>
		<li>Sensores</li>
	</ul>
</div>

<div id="requisitos">
    <h1>Requisitos Atendidos</h1>
	<ul>
		<li>Sequência de instruções em linguagem C :heavy_check_mark:</li>
		<li>Códigos comentados:heavy_check_mark:</li>
		<li>Script de compilação tipo Makefile para geração do código executável :heavy_check_mark:</li>
		<li>Documentação no READ.ME do projeto no GitHub:heavy_check_mark:</li>
		<li>O protótipo a ser implementado na SBC deverá atender às seguintes requisições:</li>
			<ul>
				<li>Capacidade de interligação com até 32 unidades de sensoriamento:heavy_check_mark:</li>
				<li>Apenas o SBC será capaz de iniciar uma comunicação:heavy_check_mark:</li>
				<li>Mecanismo de controle de status de funcionamento das unidades:heavy_check_mark:</li>
			</ul>	
		<li>O protótipo a ser implementado na NodeMCU deverá atender às seguintes restrições:</li>
			<ul>
				<li>O código deverá ser escrito em linguagem C:heavy_check_mark:</li>
				<li>Deverá ser capaz de ler e interpretar comandos oriundos do SBC:heavy_check_mark:</li>
				<li>Os comandos serão compostos por palavras de 8 bits:heavy_check_mark:</li>
				<li>A informação medida deve ter a maior precisão possível:heavy_check_mark:</li>
				<li>As requisições do SBC podem ser direcionadas para uma unidade específica ou a todas.:heavy_check_mark:</li>
				<li>Apresentação na LCD :heavy_check_mark:</li>
			</ul>
		</ul>
	</ul>
</div>

<div id="implementacao">
	<h1>Implementação</h1>
	<p>
		<li>
			Para implementação do código foi utilizado a linguagem C.
		</li>
	</p>
	<h2>Para a programação da SBC</h2>
	<p > 
		No arquivo "main.c" na pasta principal, é o arquivo que programa a SBC. Lá fizemos a importação de diversas bibliotecas necessárias, inicialização de variáveis e funções para conexão com a NodeMCU.
		<ul>
			<li>Funções</li>
			<h4 align="justify"> As funções para funcionamento da SBC são: 
			<ul>
				<li>printaLCD</li>
					<ul><li>Serve para mostrar na LCD as dados em string.</ul></li>
				<li>printaLCDInt</li>
					<ul><li>Funciona para mostrar um int na LCD, usados para valores analógicos.</ul></li>
				<li>printaLCDHexa</li>
					<ul><li>Serve para mostrar na LCD as dados em hexadecimal.</ul></li>	
				<li>writeUart</li>
					<ul><li>Função para envio de dados na conexão via UART.</ul></li>
				<li>readUart</li>					
					<ul><li>Função para recebimento de dados na conexão via UART.</ul></li>
				<li>atualizaLCD</li>
					<ul><li>Serve para colocar uma seta (-->) para ajudar na seleção da opção no menu da LCD.</ul></li>
				<li>atualizaLCDVetor</li>
					<ul><li> Serve para colocar uma seta (-->)  antes da unidade e acrescentar o valor da unidade no fim da linha.</ul></li>
				<li>nextValor</li>					
					<ul><li>Função para passar o valor do vetor do menu rotativo.</ul></li>
				<li>previousValor</li>
					<ul><li>Função para voltar o valor do vetor do menu rotativo.</ul></li>
				<li>limpaVetor</li>
					<ul><li>Função para limpar o vetor do buffer de recebimento de dados.</ul></li>
			</ul>
			<li>Constantes:</li>
			<h4> Dentro desse arquivo existem macros para a configuração dos pinos da GPIO, sendo eles: 
			<ul>
				<li>Defines de seleção de unidade</li>
					<ul><li align="justify">Constantes definidas com o nome "unidade_x" e "todas_unidades" são referentes as unidades de sensoriamento.</ul></li>
				<li>Defines de requisição</li>
					<ul><li align="justify">Constantes definidas com o nome "entrada_digital_x", "entrada_analogica", "situação_atual "e "acende_led" são referentes as entradas da NodeMCU.</ul></li>
				<li>Outras</li>
					<ul><li align="justify">É feita a declaração de constantes com a pinagem da LCD, botões e dipswitch.</ul></li>	
			</ul>
		</ul>
    </p>
	<h2>Para a programação da NodeMCU</h2>
	<p align="justify"> 
		No arquivo "main.ino" na pasta "main", é o arquivo que programa a NodeMCU. Lá fizemos a importação de diversas bibliotecas necessárias, inicialização de variáveis e funções para conexão com a SBC.
		<ul>
			<li>Defines de seleção de unidade</li>
				<ul><li align="justify">Constantes definidas assim como na SBC.</ul></li>
			<li>Defines de requisição</li>
				<ul><li align="justify">Constantes definidas assim como na SBC.</ul></li>
			<li>Outras</li>
				<ul><li align="justify">É feita a declaração de constantes com respostas para as solicitações para status das entradas, são eles: proble,a funcionando, resposta_digital, resposta_analogica.</ul></li>	
			<li>Função void setup()</li>
				<ul><li align="justify">Lá é feita a inicialização dos botões e outras constantes para o processamento do sistema.</ul></li>	
			<li>Função void loop()</li>
				<ul><li align="justify">Lá é feito o tratamento das requisições e respostas recebidas e enviadas.</ul></li>
		</ul>
	</p>
</div>

<div id="metodologia">
	<h1>Metodologia</h1>
		<h4 align="justify"> O fluxo do nosso sistema ficou da seguinte forma: temos 32 unidades de sensoriamento (referente a 32 NodeMCUs) e cada uma unidade com a possibilidade de interação com todas as suas entradas digitais, analógica e a LED.</h4>
		<h4 align="justify">De forma que a SBC interaja com a NodeMCU, são enviadas requisições com códigos de 1 byte (8 bits), assim como também é recebido a mesma quantidade de bits como resposta.</h4>
		<h4 align="justify">Para declaração dos bits para indicar as unidades e as entradas, foram declaradas variáveis para as unidades referenciando as unidades de 1 a 32, além da opção de escolha de todas as unidades, com bits declarados também para cada uma das entradas. </h4>
		<p align ="center"><img src="http://img.shields.io/static/v1?label=STATUS&message=Concluido&color=GREEN&style=for-the-badge"/></p>
	<h2>Na NodeMCU</h2>
	<p align="justify">
		É feita a leitura de dados por meio da conexão serial via UART, onde a comunicação é feita por até 8 bits de cada vez. O byte recebido é interpretado através de um protocolo pré-estabelecido e acontece uma ação a partir da requisição recebida, fazendo uma comparação com suas constantes já definidas. </p> 
	<p align="justify">
		As requisições recebidas são:
		<li>Acender led: Se a unidade selecionada já estiver ativa ele acende ou apaga a led.  </li>
		<li>Situação atual da unidade de sensoriamento: Retorna se a unidade está ativa ou não. </li>
		<li>Valor de entrada analógico: Informa o dado capturado da entrada analógica com o potenciômetro. </li>
		<li>Valor de entrada digital: Informa o dado capturado da entrada digital.</li>
	</p>
	<h2>Na SBC</h2>
		<h4 align="justify"> É feita a leitura de dados por meio da conexão serial via UART, onde a comunicação é feita por até 8 bits de cada vez. O byte recebido é interpretado através de um protocolo pré-estabelecido e acontece uma ação a partir da requisição recebida, fazendo uma comparação com suas constantes já definidas. </h4>
		<p align ="center"><img src="http://img.shields.io/static/v1?label=STATUS&message=Concluido&color=GREEN&style=for-the-badge"/>
		</p>
</div>

<div id="conclusoes">
	<h1>Resultados e Conclusões</h1>
	<p align="justify"> 
	Apesar da dificuldade de acesso ao laboatório conseguimos implementar um sistema funcional cumprindo quase todas as requisições impostas pelo problema. </p>
	<p align="justify"> O nosso sistema funciona quase por comṕleto, exceto pela interação com as 32 unidades de uma vez, onde existe a opção para selecionar todas, porém, o tratamento da mesma não foi feito. </p>
	<p align="justify"> Acreditamos que tivessemos mais tempo teríamos concluido com excelência o problema imposto.</p>
	</p>
</div>

### 🎲 Rodando o código:

```bash
# Clone este repositório
$ git clone <https://github.com/danrleiaraujo/MI-SD/tree/973d18d14ae7fc1250ef5ff3b43c846092dd19c7/2023_1/Problema%202>

#Para inicializar o SBC:
# Acesse a pasta do projeto no terminal/cmd
$ cd Problema 2

#Dê o comando para compilar:
$ make
		
#Dê o comando para Rodar:
$ sudo ./main

#Para inicializar o NodeMCU:
# Acesse o arquivo na IDE do Arduíno com as configurações de acesso a NodeMCU
Acess a pasta: Problema 2/main

e abra o arquivo: main.ino

#Dê o comando para compilar:
apertando no ícone de (V) - o primeiro botão na barra de ferramentas
		
#Dê o comando para Rodar:
apertando no ícone de (->) - o segundo botão na barra de ferramentas para jogar o código na placa NodeMCU


