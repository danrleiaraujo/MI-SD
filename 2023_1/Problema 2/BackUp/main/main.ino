#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "ESP-10.0.0.107";

int led_pin = LED_BUILTIN;
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};

/*Unidade 0*/
#define unidade_1 0b11000001
#define todas_unidades 0b11111110

#define ledOn  0b00000001
#define ledOff  0b00000000

/* Tabela de REQUISICAO*/
#define situacao_sensor  0b00000001
#define entrada_analogica  0b00010001
#define entrada_digital_0  0b00010010
#define entrada_digital_1  0b00010011
#define entrada_digital_2  0b00010100
#define entrada_digital_3  0b00010101
#define entrada_digital_4  0b00010110
#define entrada_digital_5  0b00010111
#define entrada_digital_6  0b00011000
#define entrada_digital_7  0b00011001
#define entrada_digital_8  0b00011010
#define acende_led  0b00100001


#define problema  0x01
#define funcionando  0x02
#define resposta_Digital  0x11
#define resposta_Analogica  0x12

void setup() {
  
  /* definicoes de pinos*/
  //Led:
  pinMode(led_pin, OUTPUT);

  //Possiveis Botoes:
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  
  Serial.begin(9600); // BaudRate
  
  //Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    //Serial.println("Tentando nova conexão...");
  }
  /* switch off led */
  digitalWrite(led_pin, HIGH);

  /* configure dimmers, and OTA server events */
  analogWriteRange(1000);
  analogWrite(led_pin, 990);

  for (int i = 0; i < N_DIMMERS; i++) {
    pinMode(dimmer_pin[i], OUTPUT);
    analogWrite(dimmer_pin[i], 50);
  }

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    for (int i = 0; i < N_DIMMERS; i++) {
      analogWrite(dimmer_pin[i], 0);
    }
    analogWrite(led_pin, 0);
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
    for (int i = 0; i < 30; i++) {
      analogWrite(led_pin, (i * 100) % 1001);
      delay(50);
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  //Serial.println("Tudo pronto.");

  digitalWrite(led_pin, HIGH);


}

/*Variáveis*/
bool unidade = false, opcao = false;
int valor = 0;
byte dest[4];
unsigned char dado_digital, unidadeAtual = unidade_1;
int testeSensor = 0;


void loop() {
  ArduinoOTA.handle();
  opcao = false;
  /* ====================== Verifica se tem algo sendo recebido =========================*/
  if(Serial.available() > 0) {
    char c = Serial.read(); //Le o pino RX
    if (unidade == false){
      if (c == unidadeAtual){
        Serial.write(unidadeAtual);
        unidade = true;
      }    
      /*======== Situacao 2 -> Ativa todas as unidades ==================*/
      else if (c == todas_unidades){
        Serial.write(unidadeAtual);
        unidade = true;
      }    
    }
    /*======================== Situação 5 -> Unidade já está ativa: =============================*/
    else if (unidade == true){
      /*======================== Acende o LED =============================*/
      if(c == acende_led && digitalRead(led_pin) == HIGH){
        Serial.write(ledOn);
        digitalWrite(led_pin, LOW);
      }        
      /*======================== Desliga o LED =============================*/
      else if(c == acende_led && digitalRead(led_pin) == LOW){
        Serial.write(ledOff);
        digitalWrite(led_pin, HIGH);
      } 
      /*======================== Situação Atual =============================*/ 
      /*  
      else if(c == situacao_sensor){
        testeSensor = digitalRead(D0);
        if(testeSensor HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D1);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D2);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D3);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D4);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D5);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D6);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D7);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D8);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D9);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        testeSensor = digitalRead(D10);
        if(testeSensor!= HIGH || testeSensor!= LOW ){
          Serial.write(problema);
        }
        Serial.write(funcionando);
        digitalWrite(led_pin, HIGH);
        delay(500);
      } */
      /*======================== Le o potenciometro =============================*/          
      else if(c == entrada_analogica){
        valor = analogRead(A0);
        dest[0] = valor         & 0xff;
        dest[1] = (valor >> 8)  & 0xff; 
        dest[2] = (valor >> 16)  & 0xff; 
        
        Serial.write(dest[0]);
        Serial.write(dest[1]);
        Serial.write(dest[2]);
      }
      /*======================== Le as Entradas Digitais =============================*/    
      else if(c == entrada_digital_0){
        dado_digital = digitalRead(D0);             // lendo da porta digital
        Serial.write(dado_digital);                 // enviando o valor pela serial
      }
      else if(c == entrada_digital_1){
        dado_digital = digitalRead(D1);             
        Serial.write(dado_digital);               
      }
      else if(c == entrada_digital_2){
        dado_digital = digitalRead(D2);             
        Serial.write(dado_digital);                
      }
      else if(c == entrada_digital_3){
        dado_digital = digitalRead(D3);             
        Serial.write(dado_digital);                
      }
      else if(c == entrada_digital_4){
        dado_digital = digitalRead(D4);             
        Serial.write(dado_digital);            
      }
      else if(c == entrada_digital_5){
        dado_digital = digitalRead(D5);             
        Serial.write(dado_digital);             
      }
      else if(c == entrada_digital_6){
        dado_digital = digitalRead(D6);             
        Serial.write(dado_digital);         
      }
      else if(c == entrada_digital_7){
        dado_digital = digitalRead(D7);             
        Serial.write(dado_digital);       
      }
      else if(c == entrada_digital_8){
        dado_digital = digitalRead(D8);             
        Serial.write(dado_digital);        
      }
      /*======== Situacao 3 -> desativa apenas a unidade ==================*/
      if (c == unidadeAtual){
        Serial.write(unidadeAtual);
        unidade = false;
      }    
      /*======== Situacao 4 -> desativa todas as unidades ==================*/
      else if (c == todas_unidades){
        Serial.write(unidadeAtual);
        unidade = false;
      }     
      /*======================== Fim do bloco de sensores digitais =============================*/
    } /*======================== Fim da condicional da unidade ativa =============================*/
  } // Fim da verificacao de buffer
} // Fim do While True