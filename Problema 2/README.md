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
        Para interagir com os usuários, no arquivo "/SBC/telaUser.c" foi criada uma tela interface em linguagem C que interage com a Raspberry,  onde o usuário tem 3 opções de seleção em relação ao sensor DHT11, onde a resposta deve ser dada pela escolha de um número de 1 a 3:
    <p>   
        <p>1. Situação atual do sensor</p>
        <p>2. Temperatura</p>
        <p>3. Umidade</p>
    <p>A partir da seleção do usuário, é enviado um código para a função "uartRasp(código)", essa função é encontrada na biblioteca importada "codigoUartRasp.h". Nessa biblioteca que criamos (também em C), é processada a UART da Raspberry, que é configurada a partir desta biblioteca.
    </p>
    <p><b>Biblioteca da UART da Raspberry:</b></p>
    <p>
        No arquivo "SBC/codigoUartRasp.h", utilizamos as bibliotecas "fcntl.h" e "termios.h" para manipulação da UART.
    <p>   
    <p>
        Começamos tentando o acesso através da variável "uart0_filestream" utilizando a função "open()". da verificando se deu erro na abertura da UART, caso não ocorra erro, começamos a manipulação da UART. Utilizamos as flags para configuração do BaudRate, paridade e tamanho da mensagem.
    </p>
    <p>
        Em seguida, verificamos o envio da mensagem na UART e o recebimento, caso dê algum erro recebemos uma mensagem sinalizando, para confirmar que foi enviado e recebido corretamente nós recebemos uma mensagem e também o comprimento da mensagem.
    </p>
    <p>
        Para fazer o teste de entrada e saída de dados é necessário colocar a UART em loopback.
    </p>
    <p><b>UART da FPGA:</b></p>
    <p>
        Para manipulação da UART foi utilizada a linguagem de programação Verilog. Nos arquivos "FPGA/uart_fpga_transmissor.v" e "FPGA/uart_fpga_receptor.v", temos as variáveis para inicialização da UART, como o clock, start e os dados. 
    <p>   
    <p>
        Logo abaixo é iniciado o processo de envio ou recebimento de dados, determinando a frequência de clock e o BaudRate da UART. E em seguida é feita o envio ou recebimento dos dados, que são 10 bits.
    </p>
    <p><b>DHT11 na FPGA:</b></p>
    <p>
	O sensor DHT11 possui 4 pinos:
    </p>   
    <p>
	<ul>
		<li><b>VCC</b></li>
		<li><b>DATA</b></li>
		<li><b>NULL</b></li>>
		<li><b>GND</b></li>
	</ul>	
    </p>
    <p>
	O DATA, é o pino de dados caracteriza-se como entrada e saída, ou seja um TRISTATE, este recebe as requisições e realiza o envio dos dados ao MCU (Micro-computer Unite). Ele recebe sinais de humidade e temperatura de tipo int e float. Para a configuração do mesmo, foi necessário a criação de uma máquina de estados, onde seria responsável pela leitura dos 40 bits, guardadas em registradores e tratamento de possíveis erros, como o travamento do sensor.
    </p>
	
</div>
    
</div>
