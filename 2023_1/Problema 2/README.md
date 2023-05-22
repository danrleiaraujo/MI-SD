<div id="inicio">
    <h1 id="titulo" align="center"> Problema 2 da disciplina MI - Sistemas Digitais.</h1>
	<h2 id="titulo" align="center"> Interfaces de E/S</h1>
	<p id="descricao" align="justify"></p>
	<p align ="center"><img src="http://img.shields.io/static/v1?label=STATUS&message=Concluido&color=GREEN&style=for-the-badge"/>
</div>

<div id="sumario">
    <h1>Sumário</h1>
	<ul>
		<li><a href="#inicio"> <b>Início</b></li>
        <li><a href="#equipe"> <b>Equipe de Desenvolvimento</b></li>
		<li><a href="#recursos-utilizados"> <b>Recursos Utilizados</b> </a></li>
		<li><a href="#prerequisitos"> <b>Pré-requisitos</b> </a> </li>
        <li><a href="#requisitos"> <b>Requisitos Atendidos</b> </a> </li>
		<li><a href="#fundamentacao"> <b>Fundamentação teórica</b> </a> </li>
		<li><a href="#implementacao"> <b>Implementação</b> </a> </li>
        <li><a href="#metodologia"> <b>Metodologia</b> </a> </li>
		<li><a href="#conclusao"> <b>Resultados e Conclusões</b> </a> 
		<li><a href="#referencias"> <b>Referências</b> </a> </li>
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


<div id="prerequisitos">
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

<div id="fundamentacao">
	<h1>Fundamentação teórica</h1>
	<p align="justify">Para construção do sistema, tivemos que estudar algunms conceitos importantes para o entendimento do funcionamento.</p>
	<p align="justify">Precisamos utilizar um protocolo de programação para fazer a conexão entre a SBC e a NodeMCU. Segundo Celso Kitamura (2022), "o protocolo de comunicação é uma convenção que controla e possibilita uma conexão, comunicação ou transferência de dados entre dois sistemas computacionais", neste caso, utlizamos o protocolo Universal Asynchronous Receiver/Transmitter (UART), que utilizamos um fio para conectar o transmissor e o receptor para enviar os dados (ROHDE, 2023). Ele utiliza do envio de dados de forma serial com um conjunto de bits referentes ao "start bit", para sinalizar que o envio de dados começou, bits de dados e o "stop bit" , para indicar que terminou o envio de dados, além de também pode conter um bit opcional de paridade para detecção de erros que por padrão é nível lógico baixo (0). </p> 
	<p align ="center"><img src="https://cdn.rohde-schwarz.com/pws/solution/research___education_1/educational_resources_/oscilloscope_and_probe_fundamentals/05_Understanding-UART_04_w1280_hX.png" width="600"/></p>
	<h6 align="center">Figura 1 - Estrutura de envio de dados via UART</h6>
	<p align="justify">Utilizamos uma placa chamada NodeMCU(ESP8266) para fazer a conexão com a SBC. A NodeMCU é um "um SoC (System-on-a-Chip ou Sistema-em-um-Chip) com a pilha do protocolo TCP/IP integrada, que permite que você possa implementar o acesso a rede WiFi com qualquer microcontrolador [...] onde a programação pode ser feita usando LUA ou a própria IDE do Arduino"(HU,2019). Utilizamos o mapa de pinagem, como da Figura 2, para controlar a placa.</p>
	<p align ="center"><img src="https://www.make-it.ca/wp-content/uploads/2021/09/nodemcu-pinout-functions.jpg" width="600"/></p>
	<h6 align="center">Figura 2 - Pinagem da NodeMCU(ESP8266)</h6>
</div>

<div id="implementacao">
	<h1>Implementação</h1>
	<p>Para implementação do código foi utilizado a linguagem C.</p>
	<h2>Para a programação da SBC</h2>
	<p > 
		No arquivo "main.c" na pasta principal, é o arquivo que programa a SBC. Lá fizemos a importação de diversas bibliotecas necessárias, inicialização de variáveis e funções para conexão com a NodeMCU.
		<ul>
			<li>Funções</li>
			<h align="justify"> As funções para funcionamento da SBC são:</h>
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
			<h7> Dentro desse arquivo existem macros para a configuração dos pinos da GPIO, sendo eles:</h7>
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
		<h7 align="justify"> O fluxo do nosso sistema ficou da seguinte forma: temos 32 unidades de sensoriamento (referente a 32 NodeMCUs) e cada uma unidade com a possibilidade de interação com todas as suas entradas digitais, analógica e a LED.</p>
		<p align="justify">De forma que a SBC interaja com a NodeMCU, são enviadas requisições com códigos de 1 byte (8 bits), assim como também é recebido a mesma quantidade de bits como resposta.</p>
		<p align="justify">Para declaração dos bits para indicar as unidades e as entradas, foram declaradas variáveis para as unidades referenciando as unidades de 1 a 32, além da opção de escolha de todas as unidades, com bits declarados também para cada uma das entradas. </p>
		<p align ="center"><img src="Referencias\diagrama1.jpg"/></p>
		<h6 align="center">Figura 3 - Fluxograma do menu principal</h6>
		<p align ="center"><img src="Referencias\diagrama2.jpg"/></p>
		<h6 align="center">Figura 4 - Fluxograma do sub-menu</h6>
	<h2>Na NodeMCU</h2>
	<p align="justify">
		É feita a leitura de dados por meio da conexão serial via UART, onde a comunicação é feita por até 8 bits de cada vez. O byte recebido é interpretado através de um protocolo pré-estasbelecido e acontece uma ação a partir da requisição recebida, fazendo uma comparação com suas constantes já definidas. </p> 
	<p align="justify">
		As requisições recebidas são:
		<li>Acender led: Se a unidade selecionada já estiver ativa ele acende ou apaga a led.</li>
		<li>Situação atual da unidade de sensoriamento: Retorna se a unidade está ativa ou não, se está funcionando normalmente. </li>
		<li>Valor de entrada analógico: Informa o dado capturado da entrada analógica com o potenciômetro. </li>
		<li>Valor de entrada digital: Informa o dado capturado da entrada digital.</li>
		<p align="justify">
		A NodeMCU retorna para a SBC com um código referente a solicitação do usuário, como na Tabela 1. </p> 
		<p align ="center"><img src="Referencias\respostanode.jpg"/></p>
		<h6 align="center">Tabela 1 - Códigos de resposta da NodeMCU</h6>
	</p>
	<h2>Na SBC</h2>
		<h7 align="justify"> A SBC é quem controla a NodeMCU fazendo as requisições, sendo assim, é feito o envio de dados por meio da conexão serial via UART, assim como também processa as respostas recebidas também em 1 byte. </h7>
		<p align="justify">A SBC envia as solicitações por meio dos 8 bits de protocolo pré-estabelecido no arquivo de programação da placa. Ao receber a resposta da NodeMCU, a SBC processa para que seja exibido na LCD, inclusive, a entrada do usuário é feita através dos botões conectados na placa, que funcionam respectivamente como: anterior, enter e próximo, visto que é exibido na LCD um menu para escolha das opções.</p>
		<p align ="center"><img src="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%202/Referencias/unidades.PNG?raw=true"/></p>
		<h6 align="center">Figura 5 - Conjunto de bits para a unidade</h6>
		<p align ="center"><img src="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%202/Referencias/entradas.PNG?raw=true"/></p>
		<h6 align="center">Figura 6 - Conjunto de bits para as entradas</h6>
</div>

<div id="conclusoes">
	<h1>Resultados e Conclusões</h1>
	<p align="justify">
	A seguir serão apresentados alguns dos principais resultados que obtivemos na criação do sistema:</p>
	<p align ="center"><img src="Referencias\menugif2.gif"></p>
	<h6 align="center">Figura 7 - Menu de seleção de unidades</h6>
	<p align="justify">
	É necessário fazer a escolha de qual unidade da NodeMCU o usuário deseja acessar, então o menu mostra as opções de 1 a 32, incluindo o acesso a todas as unidades, como mostrado na Figura 7, sendo que aquelas que não estão conectadas ao SBC, é mostrado um código de erro.</p>
	<p align ="center"><img src="Referencias\collage.png"  width="500"></p>
	<h6 align="center">Figura 8 - Requisição de acender/apagar LED</h6>
	<p align="justify">
	É mostrado na Figura 8 a solicitação de acender ou apagar a LED na NodeMCU.</p>
	<p align ="center"><img src="Referencias\sensordigital.gif"></p>
	<h6 align="center">Figura 9 - Requisição de entrada digital</h6>
	<p align="justify">
	O usuário escolhe qual entrada digital deseja ver os dados, então é retornado "1" no exemplo da Figura 9 com a porta D0, porque o botão indica "high" ou "1" no momento em que ele não está pressionado, caso contrário, ele indica "low" ou "0".</p>
	<p align ="center"><img src="Referencias\valoranalogico.gif"></p>
	<h6 align="center">Figura 10 - Requisição de entrada analógica</h6>
	<p align="justify">
	Assim como na entrada digital, o usuário pode escolher qual entrada analógica acessar, a que temos ativa fica conectada ao potenciômetro para simular um sensor, no momento que solicitamos os dados, é mostrado na tela o valor como na Figura 10.</p>
	<p align="justify">
	Na opção de solicitação de situação atual da unidade, a NodeMCU retorna se está funcionando ou não, no exemplo da Figura 11 é mostrado que a NodeMCU se encontra funcionando.</p>
	<p align ="center"><img src="Referencias\status.gif"></p>
	<h6 align="center">Figura 11 - Requisição de status de funcionamento da NodeMCU</h6>
	<p align="justify"> 
	Apesar da dificuldade de acesso ao laboatório conseguimos implementar um sistema funcional cumprindo quase todas as requisições impostas pelo problema. </p>
	<p align="justify"> O nosso sistema funciona quase por completo, exceto pela interação com as 32 unidades de uma vez, onde existe a opção para selecionar todas, porém, o tratamento da resposta não foi feito. </p>
	<p align="justify"> Acreditamos que tivessemos mais tempo teríamos concluido com excelência o problema imposto.</p>
	</p>
</div>

<div id="referencias"> 
	<h1>Referências</h1>
	<li align="justify">O Que É Protocolo De Comunicação? - Celso Kitamura. Celso Kitamura. Disponível em: https://celsokitamura.com.br/o-que-e-protocolo-de-comunicacao/. Acesso em: 21 maio 2023</li>
	<li align="justify">ROHDE. Compreender UART. Rohde-schwarz.com. Disponível em: https://www.rohde-schwarz.com/br/produtos/teste-e-medicao/essentials-test-equipment/digital-oscilloscopes/compreender-uart_254524.html#:~:text=UART%20significa%20%22universal%20asynchronous%20receiver,dados%20seriais%20entre%20dois%20dispositivos. Acesso em: 21 maio 2023.</li>
	<li align="justify">HU. O que é NodeMCU? -. Hu Infinito Componentes Eletrônicos. Disponível em: https://www.huinfinito.com.br/blog/artigos/o-que-e-nodemcu. Acesso em: 21 maio 2023.</li>
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


