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
const char* host = "ESP-10.0.0.108";

int led_pin = LED_BUILTIN;
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};

/*Unidade 0*/
#define unidade_1 0b11000001
#define todas_unidades 0b11111110

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
#define entrada_digital_9  0b00011011
#define entrada_digital_10  0b00011100
#define entrada_digital_11  0b00011101
#define entrada_digital_12  0b00011110
#define entrada_digital_13  0b00011111
#define acende_led  0b00100001

void setup() {
  
  /* switch on led */
  pinMode(led_pin, OUTPUT);
  pinMode(D0, INPUT);
  
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


void loop() {
  ArduinoOTA.handle();
  opcao = false;
  /* ====================== Verifica se tem algo sendo recebido =========================*/
  if(Serial.available() > 0) {
    char c = Serial.read(); //Le o pino RX
    /*======== Situacao 1 -> Ativa apenas a unidade ==================*/
    if (c == unidadeAtual  && unidade == false){
      Serial.write(unidadeAtual);
      unidade = true;
      delay(500); 
    }    
    /*======== Situacao 2 -> Ativa todas as unidades ==================*/
    else if (c == todas_unidades  && unidade == false){
      Serial.write(unidadeAtual);
      unidade = true;
      delay(500); 
    }   
    /*======================== Situação 3 -> Unidade já está ativa: =============================*/
    else if (unidade == true){
      /*======================== Acende o LED =============================*/
       if(c == acende_led && digitalRead(led_pin) == HIGH){
        Serial.write(1);
        digitalWrite(led_pin, LOW);
        delay(500); 
      }        
      /*======================== Desliga o LED =============================*/
      else if(c == acende_led && digitalRead(led_pin) == LOW){
        Serial.write(0);
        digitalWrite(led_pin, HIGH);
        delay(500);
      } 
      /*======================== Le o potenciometro =============================*/          
      else if(c == entrada_analogica){
        valor = analogRead(A0);
        dest[0] = valor         & 0xff;
        dest[1] = (valor >> 8)  & 0xff; 
        dest[2] = (valor >> 16)  & 0xff; 
        
        Serial.write(dest[0]);
        delay(2);
        Serial.write(dest[1]);
        delay(2);
        Serial.write(dest[2]);
        delay(2);
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
      else if(c == entrada_digital_9){
        dado_digital = digitalRead(D9);             
        Serial.write(dado_digital);          
      }
      else if(c == entrada_digital_10){
        dado_digital = digitalRead(D10);             
        Serial.write(dado_digital);          
      }
      /*======================== Fim do bloco de sensores digitais =============================*/
    } /*======================== Fim da condicional da unidade ativa =============================*/
  } // Fim da verificacao de buffer
} // Fim do While True