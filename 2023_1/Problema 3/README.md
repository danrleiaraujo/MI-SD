<div id="inicio">
    <h1 id="titulo" align="center"> Problema 3 da disciplina MI - Sistemas Digitais.</h1>
	<h2 id="titulo" align="center"> IoT: A Internet das Coisas</h1>
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
		<li>Biblioteca Paho-mqtt instalada em sua máquina</li>
		<li>Uma protoboard</li>
		<li>Jumpers</li>
		<li>Dois potênciometros</li>
		<li>Sensores</li>
	</ul>
</div>

<div id="requisitos">
    <h1>Requisitos Atendidos</h1>
	<p align="justify">Requisitos correspondentes ao problema 2 necessários no problema 3:</p>
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
	<p align="justify">Requisitos correspondentes ao problema 3:</p>
	<ul>
		<li>Código deverá ser escrito em linguagem C :heavy_check_mark:</li>
		<li>Usar protocolo MQTT.:heavy_check_mark:</li>
		<li>O sistema mantém a IHM local com interface baseada em display LCD, botões e chaves. Mas alterações podem ser realizadas para adaptação das novas funcionalidades :heavy_check_mark:</li>
		<li>O sistema deverá implementar uma IHM em forma de aplicativo para Desktop ou Smartphone. Esta interface deve ser capaz de apresentar as medições coletadas :heavy_multiplication_x:</li>
		<li>O sistema deve implementar uma unidade de sensoriamento sem fio utilizando uma segunda NodeMCU.:heavy_check_mark:</li>
		</ul>
	</ul>
</div>

<div id="fundamentacao">
	<h1>Fundamentação teórica</h1>
	<p align="justify">No problema anterior foi necessário o estudo sobre como funciona a NodeMCU e o envio de dados via UART. Neste problema, tivemos que estudar como funciona o envio de informações através do protocolo de comunicação Message Queuing Telemetry Transport (MQTT), já que é a funcionalidade implementada de forma adicional em relação ao problema 2.</p>
	<p align="justify"> O MQTT é um protocolo de comunicação, onde é possível transmitir e receber dados de máquina para máquina através do protocolo TCP/IP. Ele funciona por meio do princípio de Publish-Subscribe , que se parecem muito com o padrão de projeto Observer, onde existem observadores (observer) e o sujeito(subject),  onde "os observadores irão realizar uma requisição para se inscrever no subject e dessa forma ser notificado quando houver alguma mudança de estado, o sujeito irá possuir uma lista dos seus observadores para que ele saiba para quem enviar as notificações quando houver a mudança de estado" (UFRJ,2019).</p> 
	<p align="justify"> No Publish-Subscribe, além das características principais do padrão Obserser, adicionamos o Broker, que é um servidor responsável por encaminhar as mensagens para quem deve recebê-las, sendo assim, o Publisher não precisa saber quem é o Subscriber e vice-versa, ele deve apenas conhecer o Broker, sendo feita a conexão entre eles. </p> 
	<p align="justify"> Para o Broker identificar para onde se deve enviar cada mensagem, existe uma palavra chave chamada Tópico, que se refere ao grupo de mensagens que aquele Cliente está inscrito para receber. O Cliente pode atuar como aquele que publica ou aquele que recebe uma informação, de qualquer forma, terá que haver a conexão com o Broker para que isso aconteça.</p>
	<p align="justify"> A Figura 1 mostra um exemplo de como funciona o Tópico. Após o Cliente se inscrever no Tópico "temperatura" ele vai receber mensagens sobre o tema, como o dado de um sensor de temperatura, e caso ele deseje não receber mais essas mensagens, a inscrição no tópico pode ser desfeita.</p> 
	<p align ="center"><img src="https://curtocircuito.com.br/pub/media/wysiwyg/blog/MQTT_Parte4/MQTT_01-720x425.png" width="600"/></p>
	<h6 align="center">Figura 1 - Exemplo de envio de mensagens via MQTT</h6>
	<p align="justify">De forma resumida, podemos dizer que o MQTT funciona da seguinte forma: </p>
	<ol>
		<li>Um cliente MQTT estabelece uma conexão com o Broker.</li>
		<li>Depois de conectado, o cliente pode publicar mensagens, assinar mensagens específicas ou fazer as duas coisas.</li>
		<li>Ao receber uma mensagem, o Broker a encaminha aos assinantes interessados. (AWS,2023) </li>
	</ol>
</div>

<div id="implementacao">
	<h1>Implementação</h1>
	<p>Para implementação do código foi utilizado a linguagem C.</p>
	<h2>Para a programação da SBC</h2>
	<p > 
		No arquivo "main.c" na pasta principal, fizemos a importação de diversas bibliotecas necessárias, inicialização de variáveis e funções para conexão com a NodeMCU, via UART e via MQTT.
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
				<li>Criação da conexão MQTT</li>
					<ul><li align="justify">Foram criadas constantes e feita a chamada de funções correspondentes a criação da conexão do MQTT e envio e recebimento de mensagens através da biblioteca PAHO-mqtt. Desta forma, existe um tópico para recebimento de mensagens (subscriber), chamado "respostas", e um tópico de envio (publisher) chamado "requisicao" </ul></li>	
				<li>Outras</li>
					<ul><li align="justify">É feita a declaração de constantes com a pinagem da LCD, botões, dipswitch e variáveis auxiliares.</ul></li>	
			</ul>
		</ul>
    </p>
	<h2>Para a programação da NodeMCU</h2>
	<p align="justify"> 
		O arquivo "main.ino" na pasta "mainUart" e os arquivos da pasta "mainMqtt", são os arquivo que programam a NodeMCU, inicializando cada código de acordo com o protocolo de envio, se caso ela for conectada via UART ou MQTT. Nesses arquivos fizemos a importação de diversas bibliotecas necessárias, inicialização de variáveis e funções para conexão com a SBC.
		<ul>
			<li>Defines de seleção de unidade</li>
				<ul><li align="justify">Constantes definidas assim como na SBC.</ul></li>
			<li>Defines de requisição</li>
				<ul><li align="justify">Constantes definidas assim como na SBC.</ul></li>
			<li>Outras</li>
				<ul><li align="justify">É feita a declaração de constantes com respostas para as solicitações para status das entradas, são eles: proble,a funcionando, resposta_digital, resposta_analogica.</ul></li>	
			<li>Função void setup()</li>
				<ul><li align="justify">Nessa função é feita a inicialização dos botões e outras constantes para o processamento do sistema.</ul></li>	
			<li>Função void loop()</li>
				<ul><li align="justify">Nessa função é feita o tratamento das requisições e respostas recebidas e enviadas.</ul></li>
			<li>Conexão com MQTT</li>
				<ul><li align="justify">Foram criadas constantes e feita a chamada de funções correspondentes a criação da conexão do MQTT, envio e recebimento de mensagens através da biblioteca PubSubClient.h. Existe um tópico para recebimento de mensagens (subscriber), chamado "respostas", e um tópico de envio (publisher) chamado "requisicao, já que a NodeMCU recebe e envia informações.</ul></li>
		</ul>
	</p>
	<h2>Para a programação do FrontEnd</h2>
	<p align="justify"> O arquivo "index.html" na pasta "tela", é o arquivo que programa o FrontEnd. Foi criado uma tela para exibição dos dados utilizando Charts.js, uma ferramenta em Java Script para criação de gráficos. Também foi utilizado CSS para estilização da tela.</p>
	<p align ="center"><img src="Referencias\tela sem dados.JPG"/></p>
	<h6 align="center">Figura 2 - Tela sem dados </h6>
	<p align="justify"> A SBC faz a conexão com o front através do MQTT, ela prepara o envio de uma mensagem para a tela no formato "data/hora, node, pino,valor do sensor" e publica no tópico "front", como na Figura 3, assim, no arquivo principal do front, é feito o processamento da mensagem.</p>
	<p align ="center"><img src="Referencias\exemplo saida front.JPG"/></p>
	<h6 align="center">Figura 3 - Exemplo de formato de envio de mensagens</h6>
	
	
</div>

<div id="metodologia">
	<h1>Metodologia</h1>
		<h7 align="justify"> O fluxo do nosso sistema ficou da seguinte forma: temos 32 unidades de sensoriamento (referente a 32 NodeMCUs) e cada uma unidade com a possibilidade de interação com todas as suas entradas digitais, analógica e a LED.</p>
		<p align="justify">De forma que a SBC interaja com a NodeMCU, são enviadas requisições com códigos de 1 byte (8 bits), assim como também é recebido a mesma quantidade de bits como resposta.</p>
		<p align="justify">Independente da origem dos dados recebidos, via MQTT ou via UART, a SBC faz a verificação do remetente das mensagens para encaminhar o que fazer em seguida.</p>
		<p align="justify">Para declaração dos bits para indicar as unidades e as entradas, foram declaradas constantes para as unidades referenciando as unidades de 1 a 32, além da opção de escolha de todas as unidades, também existem bits declarados para cada uma das entradas digitais, analógica e da LED. </p>
		<p align ="center"><img src="Referencias\diagrama1.jpg"/></p>
		<h6 align="center">Figura 6 - Fluxograma do menu principal</h6>
		<p align ="center"><img src="Referencias\diagrama2.jpg"/></p>
		<h6 align="center">Figura 7 - Fluxograma do sub-menu</h6>
	<h2>Na NodeMCU</h2>
	<p align="justify">
		É feita a leitura de dados por meio da conexão via UART ou via MQTT (de acordo com a configuração da NodeMCU), onde a comunicação é feita por até 8 bits de cada vez. O byte recebido é interpretado através de um protocolo pré-estabelecido e acontece uma ação a partir da requisição recebida, fazendo uma comparação com suas constantes já definidas. </p> 
	<p align="justify">
		As requisições recebidas são:
		<li>Acender led: Se a unidade selecionada já estiver ativa ele acende ou apaga a led.</li>
		<li>Situação atual da unidade de sensoriamento: Retorna se a unidade está ativa ou não, se está funcionando normalmente. </li>
		<li>Valor de entrada analógico: Informa o dado capturado da entrada analógica com o potenciômetro. </li>
		<li>Valor de entrada digital: Informa o dado capturado da entrada digital.</li>
		<p align ="center"><img src="Referencias\mqtt2.jpg"/></p>
		<h6 align="center">Figura 8 - Conjunto de bits para envio de respostas</h6>
		<p align ="center"><img src="Referencias\mqtt.jpg"/></p>
		<h6 align="center">Figura 9 - Bits em string definidos para identificação</h6>
		<p align="justify">Quando uma mensagem é enviada da NodeMCU com comunicação via MQTT, os dados são enviados como na Figura 9, eles são enviados em formato de string e as requisições são processadas com um código pré-definido, já na UART, é enviado e recebido diretamente o byte da informação, como na figura 8.</p> 
	</p>
	<h2>Na SBC</h2>
		<h7 align="justify"> A SBC é quem controla a NodeMCU enviando requisições e recebendo respostas, sendo assim, é feito o envio de dados por meio da conexão serial via UART e também MQTT, assim como processa as respostas recebidas em 1 byte de dados (via UART) e uma string de bits (via MQTT). </h7>
		<p align="justify">A SBC envia as solicitações por meio dos 8 bits de protocolo pré-estabelecido no arquivo de programação da placa. Ao receber a resposta da NodeMCU, a SBC processa para que seja exibido na LCD, inclusive, a entrada do usuário é feita através dos botões conectados na placa, que funcionam respectivamente como: anterior, enter e próximo, visto que é exibido na LCD um menu para escolha das opções.</p>
		<p align ="center"><img src="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%202/Referencias/unidades.PNG?raw=true"/></p>
		<h6 align="center">Figura 10 - Conjunto de bits para a unidade</h6>
		<p align ="center"><img src="https://github.com/danrleiaraujo/MI-SD/blob/main/2023_1/Problema%202/Referencias/entradas.PNG?raw=true"/></p>
		<h6 align="center">Figura 11 - Conjunto de bits para as entradas</h6>
</div>

<div id="conclusoes">
	<h1>Resultados e Conclusões</h1>
	<p align="justify">
	A seguir serão apresentados alguns dos principais resultados que obtivemos na criação do sistema:</p>
	<p align ="center"><img src="Referencias\menugif2.gif"></p>
	<h6 align="center">Figura 12 - Menu de seleção de unidades</h6>
	<p align="justify">
	É necessário fazer a escolha de qual unidade da NodeMCU o usuário deseja acessar, então o menu mostra as opções de 1 a 32, incluindo o acesso a todas as unidades, como mostrado na Figura 12, sendo que aquelas que não estão conectadas ao SBC, é mostrado um código de erro. Nós configuramos a unidade 1 para a NodeMCU com conexão via UART e a unidade 2 e 3 com conexão MQTT.</p>
	<p align="justify"> Ao escolher uma unidade, ela retorna uma mensagem para o  usuário sinalizando o status de conexão, como visto da Figura 13, 14 e 15.</p>
	<p align ="center"><img src="Referencias\uart sem conexão.jpg"  width="500"></p>
	<h6 align="center">Figura 13 - Erro ao solicitar NodeMCU 1 via UART </h6>
	<p align ="center"><img src="Referencias\mqttunid2.jpg"  width="500"></p>
	<h6 align="center">Figura 14 - NodeMCU 2 conectada ao MQTT</h6>
	<p align ="center"><img src="Referencias\todas.jpg"  width="500"></p>
	<h6 align="center">Figura 15 - Conexão a todas NodeMCU</h6>
	<p align="justify">
	É mostrado na Figura 16 a solicitação de acender ou apagar a LED na NodeMCU.</p>
	<p align ="center"><img src="Referencias\collage.png"></p>
	<h6 align="center">Figura 16 - Requisição de acender LED</h6>
	<p align ="center"><img src="Referencias\sensordigital.gif"></p>
	<h6 align="center">Figura 17 - Requisição de entrada digital</h6>
	<p align="justify">
	O usuário escolhe qual entrada digital deseja ver os dados, então é retornado "1" ,no exemplo da Figura 17 com a porta D0, porque o botão indica "high" ou "1" no momento em que ele não está pressionado, caso contrário, ele indica "low" ou "0".</p>
	<p align ="center"><img src="Referencias\valoranalogico.gif"></p>
	<h6 align="center">Figura 18 - Requisição de entrada analógica</h6>
	<p align="justify">
	Assim como na entrada digital, o usuário pode escolher qual entrada analógica acessar, a que temos ativa fica conectada ao potenciômetro para simular um sensor, no momento que solicitamos os dados, é mostrado na tela o valor como na Figura 18.</p>
	<p align="justify">
	Na opção de solicitação de situação atual da unidade, a NodeMCU retorna se está funcionando ou não, no exemplo da Figura 19 é mostrado que a NodeMCU se encontra funcionando.</p>
	<p align ="center"><img src="Referencias\status.gif"></p>
	<h6 align="center">Figura 19 - Requisição de status de funcionamento da NodeMCU</h6>
	<p align="justify"> No menu de monitoramento de dados de um pino da NodeMCU, é mostrado de forma sequencial ao usuário todos os valores capturados pelo sensor de cada placa.</p>
	<p align="justify"> Para melhor visualização dos testes, utilizamos a ferramenta da biblioteca Mosquitto para observar o que está sendo enviado e recebido nos tópicos de requisições, respostas e front, como na Figura 20.</p>
	<p align ="center"><img src="Referencias\mosquitto2.jpg"></p>
	<h6 align="center">Figura 20 - Requisição de status de funcionamento da NodeMCU</h6>
	<p align="justify"> 
	Apesar da dificuldade de acesso ao laboratório conseguimos implementar um sistema funcional cumprindo quase todas as requisições impostas pelo problema, exceto pela visualização dos dados pelo FrontEnd. </p>
	<p align="justify"> O nosso sistema funciona por completo, com interação com as 32 unidades uma de cada vez ou todas de uma vez, ocorrendo o recebimento e envio de informações através do protocolo UART e MQTT de acordo com a configuração da NodeMCU. </p>
	<p align="justify"> Apesar de ter sido concluído, futuramente ele pode ser melhorado, pensando em um código projetado de forma mais simples e otimizada.</p>
	</p>
</div>

<div id="referencias"> 
	<h1>Referências</h1>
	<li align="justify">UFRJ. Protocolo MQTT - Redes 1. Ufrj.br. Disponível em: https://www.gta.ufrj.br/ensino/eel878/redes1-2019-1/vf/mqtt/#:~:text=MQTT (Message%20Queuing%20Telemetry%20Transport,cima%20do%20protocolo%20TCP%2FIP. Acesso em: 18 jun. 2023.‌</li>
	<li align="justify"> AWS - Amazon Web Services. O que é MQTT? – Explicação sobre o protocolo MQTT. Disponível em: https://aws.amazon.com/pt/what-is/mqtt/. Acesso em: 18 jun. 2023</li>
</div>

### 🎲 Rodando o código:

```bash
# Clone este repositório
$ git clone <https://github.com/danrleiaraujo/MI-SD/tree/main/2023_1/Problema%203>

#Para inicializar o SBC:
# Acesse a pasta do projeto no terminal/cmd
$ cd Problema 3

#Dê o comando para compilar:
$ make
		
#Dê o comando para Rodar:
$ sudo ./main

#Para inicializar o NodeMCU:
# Acesse o arquivo na IDE do Arduíno com as configurações de acesso a NodeMCU
Acess a pasta: Problema 3/main

e abra o arquivo: main.ino

#Dê o comando para compilar:
apertando no ícone de (V) - o primeiro botão na barra de ferramentas
		
#Dê o comando para Rodar:
apertando no ícone de (->) - o segundo botão na barra de ferramentas para jogar o código na placa NodeMCU


