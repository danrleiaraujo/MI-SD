<div id="inicio">
    <h1>Comunicação serial com o Raspberry PI Zero, FPGA Cyclone IV e Sensor DHT11</h1>
	<p align="justify"> 
		Este projeto foi desenvolvido para a avaliação do Problema 2 da disciplina MI - Sistemas Digitais da Universidade Estadual de Feira de Santana, consiste na implementação de um protótipo de sistema para medição de temperatura e umidade atravéz do sensor <a href="https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf">DHT11</a>. O protótipo realiza a leitura dos dados enviados pelo sensor por meio da <a href="https://www.macnicadhw.com.br/sites/default/files/documents/downloads/manual_mercurioiv_v2.pdf">FPGA Cyclone 4</a> e, estabelece uma comunicação serial RS-232 com o SBC Raspberry Pi Zero, o qual faz requisições e recebe os dados como resposta. 	p
	</p><br>
    <p>
        O sistema foi dividido em duas partes: 
    </p>
    <ul>
		<li><p>Raspberry Pi Zero: Implementação da requisição de dados em conjunto com a configuração da UART ambos desenvolvidos em Linguagem C;</p></li>
		<li><p>FPGA: Implementação em Verilog da UART de comunicação.</p></li>
	</ul>	
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
		<li><a href="https://wiki.sj.ifsc.edu.br/images/b/b0/KitMERCURIO.png">FPGA Cyclone IV (EP4CE30F23)</a></li>
	</ul>	
</div>

<div id="requisitos">
    <h1>Requisitos Atendidos</h1>
    <p><b>Raspberry Pi Zero:</b></p>
	<ul>
		<li>O código deverá ser escrito em linguagem C :heavy_check_mark:</li>
		<li>Capacidade de interligação com até 32 sensores :heavy_multiplication_x:</li>
		<li>Mecanismo de controle de status de funcionamento dos sensores :heavy_multiplication_x:</li>
        <li>Apenas o SBC será capaz de iniciar uma comunicação :heavy_check_mark:</li>
	</ul>
    <p><b>FPGA:</b></p>
		<li>O código deverá ser escrito em Verilog :heavy_check_mark:</li>
		<li>Deverá ser capaz de ler e interpretar comandos oriundos do SBC :heavy_multiplication_x:</li>
    <ul>
	</ul>
</div>

<div id="implementacao">
    <h1>Implementação</h1>
    <p>
        Para realizar a implementação deste projeto siga as etapas a seguir:
    </p>
    <h3>Realize o download do projeto</h3>
    <p><code>$ git clone https://github.com/danrleiaraujo/MI-SD.git</code></p>
    <h3>Compilando o projeto na Raspberry Pi Zero</h3>
    <p><code>$ cd /SBC</code></p>
    <p>
        Tranfira os seguintes arquivos para o Raspberry P zero: 
    </p>
    <ul>
		<li><a href="">Makefile</a></li>
		<li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/SBC/telaUser.c">telaUser.c</a></li>
        <li><a href="https://github.com/danrleiaraujo/MI-SD/blob/main/SBC/codigoUartRasp.h">codigoUartRasp.h</a></li>
	</ul><br>
    <p>Execute os seguintes comandos:</p>
    <p><code>$ make</code></p>
    <p><code>$ sudo ./telaUser.c</code></p>
    <h3>Compilando o projeto na FPGA:</h3>
    <p><code>$ cd /FPGA</code></p>
    <p>
        Crie um projeto no <it>Altera Quartus Prime Lite Edition 21</it>, especificamente para a família EP4CE30F23 e importe os seguintes arquivos: 
    </p>

<div id="metodologia">
    <h1>Metodologia</h1>
    <p><b>Interação com usuário:</b></p>
    <p>
        Para interagir com os usuários, foi criada uma tela interface em linguagem C, onde o usuário tem 3 opções de seleção, onde a resposta é dada de 1 a 3:
    <p>   
        <p>1. Situação atual do sensor</p>
        <p>2. Temperatura</p>
        <p>3. Umidade</p>
    <p>A partir da seleção do usuário
    </p>
    </p>
</div>
    
</div>
