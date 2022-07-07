<div id="inicio">
    <h1 id="titulo" align="center"> Problema 3 da disciplina MI - Sistemas Digitais.</h1>
	<p id="descricao" align="justify">Foi requisitado um protótipo de um sistema para monitoramento ambiental. O protótipo deve incluir todo o tratamento e controle de sensores analógicos e digitais, bem como uma IHM (interface Homem-Máquina) para apresentação das informações,  incluindo históricos dos dados. O protótipo deve ser desenvolvido num SBC (Raspberry Pi Zero) que medirá temperatura, umidade, pressão atmosférica e luminosidade. A IHM deve apresentar, em tempo real, as leituras atuais. Ela também deve permitir a visualização do histórico com as 10 últimas medições de cada sensor. O sistema deve permitir o ajuste local e remoto do intervalo de tempo que serão realizadas as medições.o</p>
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
        <li><a href="https://github.com/thiagocj">Thiago Cerqueira de Jesus</a></li>
    </ul>
</div>

<div id="recursos-utilizados">
	<h1> Recursos Utilizados </h1>
	<ul>
        <li><a href="https://www.embarcados.com.br/wp-content/uploads/2015/11/RpiZero-Adafruit-Frente.jpg.webp">Raspberry Pi Zero</a></li>
		<li><a href="https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf">DHT11</a></li>
	</ul>	


<div id="requisitos">
    <h1>Requisitos Atendidos</h1>
	<ul>
		<li>Código escrito em linguagem C :heavy_check_mark:</li>
		<li>Uso do protocolo MQTT:heavy_multiplication_x:</li>
		<li>Interface baseada em display LCD, botões e chaves:heavy_check_mark:</li>
		<li>Interface em forma de aplicativo para Desktop ou Smartphone.:heavy_multiplication_x:</li>
		<li>Apresentar informações sobre a conexão com a estação de medição na IHM local:heavy_check_mark:</li>
		<li>apresentar informações sobre a conexão com a estação de medição na IHM remotamente :heavy_multiplication_x:</li>
	</ul>
</div>

<div id="implementacao">
	<h1>Implementação</h1>
	<ul><p align="justify"> 
    	<li> Para implementação do código foi utilizada a linguagem C.
		<li>Para a interface WEB do usuário foi utilizada o código em linguagem HTML.
    <p> 
	<h3>
</div>

<div id="metodologia">
	<h1>Metodologia</h1>
	<h3><p><b>Interação com usuário:</b></p></h3>
	<p align="justify"> 
        O usuário se comunica com o sistema através dos botões conectados na placa Raspberry Pi Zero, onde qualquer escolha feita é selecionada através de um Dip Switch e exibida na tela LCD conectada na placa.
    <p> 
	<h3><p><b>Funcionamento das medições:</b></p></h3>
	<p align="justify"> 
        Para fazer as medições foi utilizado um sensor DHT11, que faz as medições de temperatura e umidade do ar, e para as medições de luminosidade e pressão atmosférica foi usado o potenciômetro como um simulador dos sensores, mas para utilizar ele foi necessário utilizar a biblioteca "ads1115_rpi.h" e o arquivo "ads1115_rpi.c" para fazer a conversão dos dados analógicos para digitais.
    <p>
	<h3><p><b>Uso das chaves:</b></p></h3>
	<p align="justify"> 
        O Dip Switch utilizado é numerado de 1 a 4. Cada um dos números leva a uma opção para ser selecionada.
		<li>Ao selecionar 1: exibe medição da umidade e temperatura com o uso do DHT11
		<li>Ao selecionar 2: exibe medição da luminosidade e pressão atmosférica
		<li>Ao selecionar 3: exibe histórico com as últimas 10 medições da luminosidade e pressão atmosférica 
		<li>Ao selecionar 4: exibe histórico com as últimas 10 medições de umidade e temperatura do DHT11
    <p>  
	<h3><p><b>Processos feitos na área principal do código:</b></p></h3>
	<p align="justify"> 
        <li>No ínicio do código são inicializadas algumas várias necessárias para o funcionamento, inclusive a alocação de memória para o salvamento dos arquivos do histórico.
		<li>Usando a função "pinMode()" é feita o inicialização dos pinos do Dip Switch como entrada de dados
		<li>Um loop com "while" é feito enquanto for verdadeiro, ou seja, de forma que seja infinito. Dentro dele possui seleções das opções dos menus separados em "if", que dependendo da seleção do pino do Dip Switch ele acessa uma opção.
		<li>Em cada opção é feita a chamada das funções respectivas para fazer as medições e para atualizar o histórico de tal medida. 
    <p> 
	<h3><p><b>Salvamento do histórico:</b></p></h3>
	<p align="justify"> 
       Para visualização dos dados em um histórico, foi gerado um arquivo .txt onde a cada medição é feito uma sobrescrita nos dados em formato de fila (FIFO), mantendo salvo as 10 últimas medições. Foi usado um ponteiro de struct para salvar os dados, onde "linha" é salvo as linhas que são lidas e o "nLinhas" que quando carrega o documento, conta quantas linhas tem. Existem dois históricos, o do DHT11 e dos Sensores de Luminosidade e Pressão. Para a atualização do histórcico os dados do DHT11, como são em formato "int", é feito uma conversão de dados para String.
    <p> 
</div>
		
<h2>Pré-requisitos</h2>
<h3>Antes de começar, você vai precisar ter:</h3>
<li>Um Raspberry Pi Zero</li>
<li>Um DHT11.</li>
<li>Um conversor ads1115 analógico para digital.</li>
<li>Dois potênciometros (Para a simulação de sensores).</li>

### 🎲 Rodando o código:

```bash
# Clone este repositório
$ git clone <https://github.com/danrleiaraujo/MI-SD/tree/main/Problema%203>

# Acesse a pasta do projeto no terminal/cmd
$ cd problema3

#Dê o comando para compilar:
$ gcc -o problema3 problema3.c ads1115_rpi.c -lwiringPI -lwiringPiDev

		
#Dê o comando para Rodar:
$ sudo ./problema3
Obs. Verifique as pinagens dos sensores antes de rodar.
		
