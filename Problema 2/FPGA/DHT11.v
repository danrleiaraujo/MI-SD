`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Matéria: MI- SD
// Integrantes: Danrlei Almeida Araujo;
// 				Evelyn Suzarte Fernandes.
// Create Date:    23/05/2022;
// Sensor :    DHT11;
// Descrição: Configuração do DHT11 em verilog //
//////////////////////////////////////////////////////////////////////////////////

module DHT11(
	//pino de entrada
	input CLK,  //100 MHz
	input EN, 
	input RST,	 
	
	//pino tristate
	inout DHT_DATA,

	//pinos de saída com 8 bits
	output [7:0] HUM_INT,
	output [7:0] HUM_FLOAT,
	output [7:0] TEMP_INT,
	output [7:0] TEMP_FLOAT,
	output [7:0] CRC,

	output WAIT, //pino pra informar se a estrutura está aguardando
	output DEBUG, //pino para debug para testes
	output error // pino para debug para erros
	);
	
	reg DHT_OUT, DIR, WAIT_REG, DEBUG_REG;  //Registrador de saída	
	reg [25:0] COUNTER; //Contador de ciclos para gerar delays
	reg [5:0] index;
	reg [39:0] INTDATA; //registrador de dados interno
	reg errorReg;
	wire [39:0] DHT_IN;
	
	//wire DHT_IN;
	
	assign WAIT = WAIT_REG;
	assign DEBUG = DEBUG_REG;
	
	//assign DHT_DATA = DIR ? DHT_OUT : 1'bZ; // Se DIR 1 -- copia DHT_OUT para saida, caso nao, deixa o pino indefinido para atuar como entrada
	//assign DHT_IN = DHT_DATA;

TRIS TRIS_DATA(
    .PORT(DHT_DATA),
    .DIR(DIR),
    .SEND(DHT_OUT),
    .READ(DHT_IN)
    );	
	
	assign HUM_INT[7] = INTDATA[0];
	assign HUM_INT[6] = INTDATA[1];
	assign HUM_INT[5] = INTDATA[2];
	assign HUM_INT[4] = INTDATA[3];
	assign HUM_INT[3] = INTDATA[4];
	assign HUM_INT[2] = INTDATA[5];
	assign HUM_INT[1] = INTDATA[6];
	assign HUM_INT[0] = INTDATA[7];
	
	assign HUM_FLOAT[7] = INTDATA[8];
	assign HUM_FLOAT[6] = INTDATA[9];
	assign HUM_FLOAT[5] = INTDATA[10];
	assign HUM_FLOAT[4] = INTDATA[11];
	assign HUM_FLOAT[3] = INTDATA[12];
	assign HUM_FLOAT[2] = INTDATA[13];
	assign HUM_FLOAT[1] = INTDATA[14];
	assign HUM_FLOAT[0] = INTDATA[15];
	
	assign TEMP_INT[7] = INTDATA[16];  
	assign TEMP_INT[6] = INTDATA[17];
	assign TEMP_INT[5] = INTDATA[18];
	assign TEMP_INT[4] = INTDATA[19];
	assign TEMP_INT[3] = INTDATA[20];
	assign TEMP_INT[2] = INTDATA[21];
	assign TEMP_INT[1] = INTDATA[22];
	assign TEMP_INT[0] = INTDATA[23]; 
	
	assign TEMP_FLOAT[7] = INTDATA[24];
	assign TEMP_FLOAT[6] = INTDATA[25];
	assign TEMP_FLOAT[5] = INTDATA[26];
	assign TEMP_FLOAT[4] = INTDATA[27];
	assign TEMP_FLOAT[3] = INTDATA[28];
	assign TEMP_FLOAT[2] = INTDATA[29];
	assign TEMP_FLOAT[1] = INTDATA[30];
	assign TEMP_FLOAT[0] = INTDATA[31];	
	
	assign CRC[7] =  INTDATA[32];
	assign CRC[6] =  INTDATA[33];
	assign CRC[5] =  INTDATA[34];
	assign CRC[4] =  INTDATA[35];
	assign CRC[3] =  INTDATA[36];
	assign CRC[2] =  INTDATA[37];
	assign CRC[1] =  INTDATA[38];
	assign CRC[0] =  INTDATA[39];

	 reg [3:0] STATE; //maquina de estados
	 
	 //Definição de estados
	 parameter S0=1, S1=2, S2=3, S3=4, S4=5, S5=6, S6=7, S7=8, S8=9, S9=10, STOP=0, START=11;

 
	//Processo de FSM 
	always @(posedge CLK)
	begin: FSM
	  if (EN == 1'b1)
	  begin
		 if ( RST == 1'b1)
		 begin			  
			  DHT_OUT <= 1'b1;			  
			  WAIT_REG <= 1'b0;
			  COUNTER <= 26'b00000000000000000000000000;	
			  INTDATA <= 40'b0000000000000000000000000000000000000000;
			  DIR <= 1'b1;			   //Configura pino saida
			  errorReg <= 1'b0;
			  STATE <= START;
		 end else begin
		 
			 case (STATE)
				 START:
				     begin			
						   WAIT_REG <= 1'b1;
							DIR <= 1'b1;
							DHT_OUT <= 1'b1;
					/*	    if (COUNTER < 67108860 )
							 begin
							    COUNTER <= COUNTER +1'b1;
							 end else begin
							      COUNTER <= 26'b00000000000000000000000000;
									STATE <= S0;
							 end		*/		
							STATE <= S0;							 
					  end
			   
				 S0:
					 begin
					   DIR <= 1'b1;	
					   DHT_OUT <= 1'b1;
						WAIT_REG <= 1'b1;
						errorReg <= 1'b0;
						if (COUNTER < 1800000)							// -- b111001111110111100000 --100.000.000/2 = 50.000.000 -> 1/50.000.000 = 0,00002ms --> 18ms/0,00002 = 900000 ciclos)
						begin
							COUNTER <= COUNTER +1'b1;
						end else begin
							COUNTER <= 26'b00000000000000000000000000;
							STATE <= S1;
						end
					 end
				 
				 S1:
					 begin
					   DHT_OUT <= 1'b0;	
						WAIT_REG <= 1'b1;
						if (COUNTER < 1800000)							// --b111001111110111100000 -- 100.000.000/2 = 50.000.000 -> 1/50.000.000 = 0,00000002s --> 18ms/0,00002 = 900000 ciclos)
						begin
							COUNTER <= COUNTER +1'b1;
						end else begin
							COUNTER <= 26'b00000000000000000000000000;
							STATE <= S2;
						end
					 end
				S2:
					begin					   
						DHT_OUT <= 1'b1;			//Leva para 1 aguarda 20 uS (resposta do DHT ocorre entre 20 e 40 uS)
						//DIR <= 1'b0;	
						if (COUNTER < 2000)
						begin
						   COUNTER <= COUNTER +1'b1;
						end else begin						
							DIR <= 1'b0;							
							STATE <= S3;									
						end						
					end
					
				S3:
					begin	 
               //   DIR <= 1'b0;					
						if (COUNTER < 6000 && DHT_IN == 1'b1 )							// 60 (88) uS / 0,02uS = 2000 CICLOS DE 50MHZ
						begin		                     						
						 	COUNTER <= COUNTER +1'b1;
							STATE <= S3;
						end else begin
						  if ( DHT_IN == 1'b1 )  											//Se ultrapassa o limite de 40uS -- erro de inicializacao do DHT11
						  begin						  																	
							   errorReg <= 1'b1;
								COUNTER <= 26'b00000000000000000000000000;
							   STATE <=STOP;
						  end else begin
						      COUNTER <= 26'b00000000000000000000000000;
						      STATE <=S4;														//Nao passou 40uS, entao DHT foi a 0, logo - OK
						  end
						end
					end
					
				S4:
					begin
					  //DETECTA PULSO DE SINCRONISMO - P1
						if ( DHT_IN == 1'b0  && COUNTER < 8800)												// 0,00002 = 0,02 uS -> 80uS/0,02uS = 4000 ciclos de 50MHz -> 25'b0000000000000111110100000
						begin
						   COUNTER <= COUNTER +1'b1;
							STATE <= S4;
						end else begin
					      if ( DHT_IN == 1'b0)
							begin
							   errorReg <= 1'b1;
								COUNTER <= 26'b00000000000000000000000000;	
							   STATE <=STOP;
							end else begin 
						   	STATE <= S5;
								COUNTER <= 26'b00000000000000000000000000;															
							end
						end
					end
			
				S5:
					begin
					  //DETECTA PULSO DE SINCRONISMO - P2
						if ( DHT_IN == 1'b1 && COUNTER < 8800)												// 0,00002 = 0,02 uS -> 80uS/0,02uS = 4000 ciclos de 50MHz -> 25'b0000000000000111110100000
						begin
						   COUNTER <= COUNTER +1'b1;
							STATE <= S5;
						end else begin
						   if ( DHT_IN == 1'b1)
							begin
							   errorReg <= 1'b1;
								COUNTER <= 26'b00000000000000000000000000;	
							   STATE <=STOP;
							end else begin 
								STATE <= S6;  
								errorReg <= 1'b0;
								index <= 6'b000000; //reseta indexador
								COUNTER <= 26'b00000000000000000000000000;															
							end
						end
					end
				//Inicio da analise de dados	
				S6:						//Realiza Resets
					begin				
							 if ( DHT_IN == 1'b0 )
							 begin
							//COUNTER <= 26'b00000000000000000000000000;
							//error <= 1'b0;
							//DIR <= 1'b0;
								STATE <= S7;
							end else begin
							   errorReg <= 1'b1;
								COUNTER <= 26'b00000000000000000000000000;	
							   STATE <=STOP;							
							end
					end					
				
				S7:      //Chegou neste estado, o nivel logico j� � zero, deve aguardar comuta��o para 1 de modo a avaliar tempos
					begin
					  if ( DHT_IN == 1'b1 )
					  begin
								COUNTER <= 26'b00000000000000000000000001;
								STATE <= S8;								
						end else begin
						   if ( COUNTER < 3200000)  // -- 60uS - 3000 ciclos - VERIFICA SE ESTOUROU TEMPO PARA IR A ZERO E INICIAR TRANSMISSAO DE DADOS
							begin								
								COUNTER <= COUNTER +1'b1;
								STATE <= S7;	
							end else begin
								COUNTER <=  26'b00000000000000000000000000;
								errorReg <= 1'b1;
								STATE <= STOP;
							end
						end
					end
					
				S8:   //Aguarda comuta��o para 0
				   begin
						if (  DHT_IN == 1'b0 ) /// 50MHz = 0,02 uS -> 60uS = 2500 ciclos
						begin
									
									if ( COUNTER > 5000) 
									begin
									   INTDATA[index] <= 1'b1;
										DEBUG_REG <= 1'b1;
										
									end else begin
		
										INTDATA[index] <= 1'b0;
										DEBUG_REG <= 1'b0;									   
									end																	

									if (index < 39 )
									begin										
										COUNTER <= 26'b00000000000000000000000000;
										STATE <= S9;
									end else begin		
										errorReg <= 1'b0;									
										STATE <= STOP;
									end										
									
									
								//	STATE <= S9;
									
						end else begin
									COUNTER <= COUNTER + 1'b1;
									
									if (COUNTER > 3200000) //Caso mais de 80uS de espera, aborta
									begin
									   errorReg <= 1'b1;
										STATE <= STOP;
									end
						end
					 end
					 
				 S9:
					begin
					    index <= index+1'b1;
						 STATE <= S6;
					end	
				 
				 STOP:
					begin
					   STATE <= STOP;
					   if ( error == 1'b0 ) 
						begin						
							DHT_OUT <= 1'b1;								
							WAIT_REG <= 1'b0;
							COUNTER <= 26'b00000000000000000000000000;	
							DIR <= 1'b1;			   //Configura pino saida	
							errorReg <= 1'b0;							
							index <= 6'b000000;	
						end else begin
						  if ( COUNTER < 3200000 )   //Se error, mantem estrutura bloqueada por 3,2 ms at� DHT finalizar e sinaliza erro
						  begin
						      INTDATA <= 40'b0000000000000000000000000000000000000000;
								COUNTER <= COUNTER + 1'b1;
								errorReg <= 1'b1;
								WAIT_REG <= 1'b1;
								DIR <= 1'b0;			   //Configura pino said
						  end else begin
								errorReg <= 1'b0;				//volta error a 0 para resetar tudo
						  end
						end
					   	
					end
			 endcase
		end
	end
  end


endmodule