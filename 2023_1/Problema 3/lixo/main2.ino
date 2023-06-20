#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <Timer.h>
#include <WiFiUdp.h>
#include <string.h> 


#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

//ESP na rede
const char* host = "ESP-10.0.0.111";
IPAddress local_IP(10, 0, 0, 111);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// Definições do servidor MQTT
const char* BROKER_MQTT = "10.0.0.101";        // broker MQTT 
int BROKER_PORT = 1883;
              
// Definições do ID
#define ID_MQTT   "ESP-111"  // ID desta nodeMCU (ID Client)
#define USER      "aluno"
#define PASSWORD  "@luno*123"
#define QOS       1
WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);   // Instancia o Cliente MQTT passando o objeto espClient

// Topicos a serem subescritos
#define SBC_ESP    "requisicoes"

// Topicos a serem publicados
#define SENSORES   "respostas"

/*Unidade 0*/
#define todas_unidades "11111110"
#define unidade_1 "11000001"
#define unidade_2 "11000010"
#define unidade_3 "11000011"
#define unidade_4 "11000100"
#define unidade_5 "11000101"
#define unidade_6 "11000110"
#define unidade_7 "11000111"
#define unidade_8 "11001000"
#define unidade_9 "11001001"
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

#define espera 1 //tem que ser referente ao valor da unidade

/*Valores da led*/
#define ledOn  "0x01"
#define ledOff  "0x00"

/* Tabela de REQUISICAO*/
#define situacao_sensor  "00000001"
#define entrada_analogica  "00010001"
#define entrada_digital_0  "00010010"
#define entrada_digital_1  "00010011"
#define entrada_digital_2  "00010100"
#define entrada_digital_3  "00010101"
#define entrada_digital_4  "00010110"
#define entrada_digital_5  "00010111"
#define entrada_digital_6  "00011000"
#define entrada_digital_7  "00011001"
#define entrada_digital_8  "00011010"
#define acende_led  "00100001"

/*Script do problema - Exemplos respostas*/
#define problema  "0x01"
#define funcionando  "0x02"
#define resposta_Digital  "0x11"
#define resposta_Analogica  "0x12"

/*Variáveis*/
bool unidade = false, opcao = false, sensorProblema = false;
int valor = 0, testeSensor[9];
unsigned char dado_digital;
char unidadeAtual[] = unidade_1; 
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
  MQTT.subscribe(SBC_ESP, 1); 
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
  Serial.println("Booting");

  // Configuração do IP fixo no roteador, se não conectado, imprime mensagem de falha
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
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
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


/**
 * Recebe as mensagens via mqtt 
 * @param topic - Topico que enviou a mensagem
 * @param payload - Mensagem recebida
 * @param length - Tamanho da mensagem
*/
void on_message(char* topic, byte* payload, unsigned int length){
  String recvd; //Recebido
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
        value = unidadeAtual;
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }    
    /* ====================== Se o que foi lido for igual ao código de todas unidades: =========================*/
      else if (recvd == todas_unidades){
        unidade = true;
        delay(espera);
        value = unidadeAtual;
        
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }    
    }
    else if (unidade == true){
      /*======================== Acende ou apaga a LED =============================*/
      if(recvd == "00100001"){ 
        if(digitalRead(led_pin) == HIGH){
          digitalWrite(led_pin, LOW);
          value = "00000000"; 
        }
        else if(digitalRead(led_pin) == LOW){
          digitalWrite(led_pin, HIGH);
          value = "00000001";
        }
        client.publish("respostas", String(value).c_str());
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
          value = funcionando;
          delay(1500);
          client.publish("respostas", String(value).c_str());
        }
        else{
          value = problema;
          delay(1500);
          client.publish("respostas", String(value).c_str());
        }
      } 
      /*======================== Le o potenciometro =============================*/          
      else if(recvd == entrada_analogica){
        value = analogRead(A0);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      /*================== Le as Entradas Digitais e envia os valores das portas ======================*/ 
      else if (recvd == entrada_digital_0){
        value = digitalRead(D0);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_1){
        value = digitalRead(D1);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_2){
        value = digitalRead(D2);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_3){
        value = digitalRead(D3);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_4){
        value = digitalRead(D4);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_5){
        value = digitalRead(D5);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_6){
        value = digitalRead(D6);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_7){
        value = digitalRead(D7);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (recvd == entrada_digital_8){
        value = digitalRead(D8);
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }

      /*======== Situacao 3 -> desativa apenas a unidade ==================*/
      if (recvd == unidadeAtual){
        unidade = false;
      }    
      /*======== Situacao 4 -> desativa todas as unidades ==================*/
      else if (recvd == todas_unidades){
        unidade = false;
      }  
  } // if se mensagem não for nula
}// end on_message


/**
 * Inicia as configuracoes no momento do upload 
 */
void setup() {
  // realiza a configuracao inicial para conexao via wifi com nodemcu
  config_connect();
  //Serial.begin(9600);

  // definicao dos pinos
  pinMode(LED_BUILTIN, OUTPUT);  

  // inicia a comunicacao mqtt
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); 
  MQTT.setCallback(on_message);

  //timer.every(tempo_medicoes * 1000, medicoes); 

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