<div id="inicio">
    <h1 id="titulo" align="center"> Problema 1 da disciplina MI - Sistemas Digitais.</h1>
	<h2 id="titulo" align="center"> Linguagem Assembly</h1>
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
        <li><a href="https://github.com/thiagocj">Thiago Cerqueira de Jesus</a></li>
    </ul>
</div>

<div id="recursos-utilizados">
	<h1> Recursos Utilizados </h1>
	<ul>
        <li><a href="">Orange Pi PC PLUS</a></li>
	</ul>	


<div id="requisitos">
    <h1>Requisitos Atendidos</h1>
	<ul>
		<li>Sequência de instruções em Assembly :heavy_check_mark:</li>
		<li>Códigos comentados:heavy_check_mark:</li>
		<li>Script de compilação tipo Makefile para geração do código executável :heavy_check_mark:</li>
		<li>Documentação no READ.ME do projeto no GitHub:heavy_check_mark:</li>
		<li>O sistema deve permitir configurar o tempo de contagem:heavy_multiplication_x:</li>
		<li>O sistema deve permitir configurar o tempo de contagem para o temporizador:heavy_multiplication_x:</li>
		<li>Apresentação no timer na LCD :heavy_multiplication_x:</li>
		<li>Criação da biblioteca(.o) com funções para manipulação do LCD :heavy_multiplication_x:</li>
	</ul>
</div>

<div id="implementacao">
	<h1>Implementação</h1>
	<ul><p align="justify"> 
    	<h4> Para funcionamento, é necessária a implementação de alguns arquivos usados em conjunto.</h4>
		<li>Sys_calls</li>
		<h4> Dentro desse arquivo existem comandos para a invocação do sistema operacional para fazer tal ação. As System Calls,como são chamadas, são divididas em quatro grupos: chamadas de sistema para gerenciamento de processos, para gerenciamento de diretórios, para gerenciamento de arquivos e restantes. </h4>
		<li>Gpio_macros</li>
		<h4> Dentro desse arquivo existem comandos para a configuração dos pinos da GPIO.Sendo eles:  </h4>
		<li>Arquivo_io</li>
		<li>Principal (main)</li>
    <p> 
	<h3>
</div>

<div id="metodologia">
	<h1>Metodologia</h1>

</div>
		
<h2>Pré-requisitos</h2>
<h3>Antes de começar, você vai precisar ter:</h3>
<li>Orange Pi PC PLUS</li>
<li>Um LCD 16x2.</li>
<li>Uma protoboard.</li>
<li>Um jumper para ligar a GPIO na protoboard.</li>
<li>Um potênciometro para regular o LCD.</li>

### 🎲 Rodando o código:

```bash
# Clone este repositório
$ git clone <>

# Acesse a pasta do projeto no terminal/cmd
$ cd timer

#Dê o comando para compilar:
$ make

		
#Dê o comando para Rodar:
$ sudo ./problema3