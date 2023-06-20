#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <string.h> 


#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

//ESP na rede
const char* host = "ESP-10.0.0.109";
IPAddress local_IP(10, 0, 0, 109);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// Definições do servidor MQTT
const char* BROKER_MQTT = "10.0.0.101";        // broker MQTT 
int BROKER_PORT = 1883;
              
// Definições do ID
#define ID_MQTT   "ESP-109"  // ID desta nodeMCU (ID Client)
#define USER      "aluno"
#define PASSWORD  "@luno*123"
#define QOS       1
WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);   // Instancia o Cliente MQTT passando o objeto espClient

// Topicos a serem subescritos
#define TOPICSUB    "requisicoes"

// Topicos a serem publicados
#define TOPICPUB   "respostas"

/*Unidades*/
#define todas_unidades "11111110"
#define unidade_1  "11000001"
#define unidade_2  "11000010"
#define unidade_3  "11000011"
#define unidade_4  "11000100"
#define unidade_5  "11000101"
#define unidade_6  "11000110"
#define unidade_7  "11000111"
#define unidade_8  "11001000"
#define unidade_9  "11001001"
#define unidade_10 "11001010"
#define unidade_11 "11001011"
#define unidade_12 "11001100"
#define unidade_13 "11001101"
#define unidade_14 "11001110"
#define unidade_15 "11001111"
#define unidade_16 "11010000"
#define unidade_17 "11010001"
#define unidade_18 "11010010"
#define unidade_19 "11010011"
#define unidade_20 "11010100"
#define unidade_21 "11010101"
#define unidade_22 "11010110"
#define unidade_23 "11010111"
#define unidade_24 "11011000"
#define unidade_25 "11011001"
#define unidade_26 "11011010"
#define unidade_27 "11011011"
#define unidade_28 "11011100"
#define unidade_29 "11011101"
#define unidade_30 "11011110"
#define unidade_31 "11011111"
#define unidade_32 "11100000"

/*Valores da led*/
#define ledOff  "0x00"
#define ledOn  "0x01"

/* Tabela de REQUISICAO*/
#define situacao_sensor  "0x02"
#define entrada_analogica  "0x11"
#define entrada_digital_0  "0x12"
#define entrada_digital_1  "0x13"
#define entrada_digital_2  "0x14"
#define entrada_digital_3  "0x15"
#define entrada_digital_4  "0x16"
#define entrada_digital_5  "0x17"
#define entrada_digital_6  "0x18"
#define entrada_digital_7  "0x19"
#define entrada_digital_8  "0x1A"
#define acende_led  "0x21"

/*Script do problema - Exemplos respostas*/
#define problema  "0x01"
#define funcionando  "0x02"
#define resposta_Digital  "0x11"
#define resposta_Analogica  "0x12"

/*Variáveis*/
int led_pin = LED_BUILTIN;
bool unidade = false, opcao = false, sensorProblema = false;
int valor = 0, testeSensor[9];
unsigned char dado_digital; 
const char unidadeAtual[] = unidade_2;
byte dest[4];

/**
 * Reconecta-se ao broker Tenta se conectar ao broker constantemente
 */
void reconnectMQTT() {
  while (!MQTT.connected()) {
     if (!MQTT.connect(ID_MQTT, USER, PASSWORD)){ 
      delay(2000);
    }
  }
  MQTT.subscribe(TOPICSUB, 1); 
}

/**
 * Caso a NodeMCU não esteja conectado ao WiFi, a conexão é restabelecida.
*/
void reconnectWiFi() {
  //se já está conectado a rede WI-FI, nada é feito. 
  //Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED)
    return;
        
  WiFi.begin(ssid, password); // Conecta na rede WI-FI
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}

/**
 * Verifica se o cliente está conectado ao broker MQTT e ao WiFi.
 * Em caso de desconexão, a conexão é restabelecida.
*/
void checkMQTTConnection(void) {
  reconnectWiFi();
  if (!MQTT.connected()){
    reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
  } 
}

/**
 * Configura a comunicacao com o nodemcu via WIFI
*/
void config_connect(){
  Serial.begin(9600);
  // Configuração do IP fixo no roteador, se não conectado, imprime mensagem de falha
  if (!WiFi.config(local_IP, gateway, subnet)) {
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(host);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) {
    } else if (error == OTA_BEGIN_ERROR) {
    } else if (error == OTA_CONNECT_ERROR) {
    } else if (error == OTA_RECEIVE_ERROR) {
    } else if (error == OTA_END_ERROR) {
    }
  });
  ArduinoOTA.begin();
  Serial.println(WiFi.localIP());
}

/**
 * Recebe as mensagens via mqtt 
 * @param topic - Topico que enviou a mensagem
 * @param payload - Mensagem recebida
 * @param length - Tamanho da mensagem
*/
void on_message(char* topic, byte* payload, unsigned int length){
  String recvd; 
  char value[2]; //Recebido

  if(length){ // Caso a mensagem nao seja nula:

    for(int i = 0; i < length; i++) {
      char c = (char) payload[i];
      recvd += c;
    }

    /* ====================== Situação 1 -> Caso tenha e a unidade esteja desativada: =========================*/
    if (unidade == false){
    /* ====================== Se o que foi lido for igual ao código da unidade: =========================*/
      if (recvd == unidadeAtual){
        unidade = true;
        MQTT.publish(TOPICPUB, unidadeAtual);
      }    
    /* ====================== Se o que foi lido for igual ao código de todas unidades: =========================*/
      else if (recvd == todas_unidades){
        unidade = true;
        MQTT.publish(TOPICPUB, unidadeAtual);
      }    
    }
    else if (unidade == true){
      /*======================== Acende ou apaga a LED =============================*/
      if(recvd == acende_led){ 
        if(digitalRead(led_pin) == HIGH){
          digitalWrite(led_pin, LOW);
          MQTT.publish(TOPICPUB, ledOn);
        }
        else if(digitalRead(led_pin) == LOW){
          digitalWrite(led_pin, HIGH);
          MQTT.publish(TOPICPUB, ledOff);
        }
      }
      /*======================== Situação Atual =============================*/ 
      else if(recvd == situacao_sensor){
        /*Leitura dos sensores*/
        valor = analogRead(A0);
        testeSensor[0] = digitalRead(D0);
        testeSensor[1] = digitalRead(D1);

        /*===================== VERIFICACAO ===================*/
        /*Portas Analogica com Potenciometro*/
        if(valor < 0 || valor > 1024){
          sensorProblema = true;
        }
        /*Portas digitais com pushButton*/
        else if(testeSensor[0] != HIGH){  // D0
          sensorProblema = true;
        }
        else if(testeSensor[1] != HIGH){ // D1
          sensorProblema = true;
        }
        else{
          sensorProblema = false;
        }
        /*=========== Envio da verificação ================*/
        if (sensorProblema == false){  // Caso não tenha problema
          MQTT.publish(TOPICPUB, funcionando);
        }
        else{
          MQTT.publish(TOPICPUB, problema);
        }
      } 
      /*======================== Le o potenciometro =============================*/          
      else if(recvd == entrada_analogica){
        sprintf(value, "%d", analogRead(A0));
        MQTT.publish(TOPICPUB, value);
      }
      /*================== Le as Entradas Digitais e envia os valores das portas ======================*/ 
      else if (recvd == entrada_digital_0){
        sprintf(value, "%d", digitalRead(D0));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_1){
        sprintf(value, "%d", digitalRead(D1));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_2){
        sprintf(value, "%d", digitalRead(D2));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_3){
        sprintf(value, "%d", digitalRead(D3));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_4){
        sprintf(value, "%d", digitalRead(D4));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_5){
        sprintf(value, "%d", digitalRead(D5));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_6){
        sprintf(value, "%d", digitalRead(D6));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_7){
        sprintf(value, "%d", digitalRead(D7));
        MQTT.publish(TOPICPUB, value);
      }
      else if (recvd == entrada_digital_8){
        sprintf(value, "%d", digitalRead(D8));
        MQTT.publish(TOPICPUB, value);
      }
      /*======== Situacao 3 -> desativa apenas a unidade ==================*/
      if (recvd == unidade_2){
        unidade = false;
      }    
      /*======== Situacao 4 -> desativa todas as unidades ==================*/
      else if (recvd == todas_unidades){
        unidade = false;
      }  
    } 
  }// if se mensagem não for nula
}// end on_message

/**
 * Inicia as configuracoes no momento do upload 
 */
void setup() {
  // realiza a configuracao inicial para conexao via wifi com nodemcu
  config_connect();
  //Serial.begin(9600);

  // definicao dos pinos
  pinMode(led_pin, OUTPUT);  

  // inicia a comunicacao mqtt
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); 
  MQTT.setCallback(on_message);

  // pisca o led do nodemcu no momento da execucao
  for(int i=0; i<10; i++){
    digitalWrite(LED_BUILTIN,LOW);
    delay(50);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(50);
  }
}

void loop() {
  ArduinoOTA.handle();
  checkMQTTConnection();
  MQTT.loop();
}