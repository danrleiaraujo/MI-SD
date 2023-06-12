
# include <ESP8266WiFi.h>
# include <ESP8266mDNS.h>
# include <WiFiUdp.h>
# include <ArduinoOTA.h>
# include <PubSubClient.h>

# ifndef STASSID
# define STASSID "INTELBRAS"
# define STAPSK  "Pbl-Sistemas-Digitais"
# endif

const char* ssid = STASSID; 
const char* password = STAPSK;
const char* host = "ESP-10.0.0.107";

int buttonD0 = D0; // push button is connected
int buttonD1 = D1; // push button is connected
int temp0 = 0;   // temporary variable for reading the button pin status
int temp1 = 0;   // temporary variable for reading the button pin status

// MQTT broker credentials
const char* MQTT_username = "aluno"; 
const char* MQTT_password = "@luno*123"; 

// MQTT broker
const char* mqtt_server = "10.0.0.101";

// Client MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// # define N_DIMMERS 3
// int dimmer_pin[] = {14, 5, 15};

// /* POTENCIOMETRO */
// int analogInPin = A0;
// int sensorValue = 0;  // value read from the pot
// /* POTENCIOMETRO */
// char arrb[]={0b00000000, 0b00000001, 0b00000010, 0b00000011, 0b00000100};      

// char protocolCodes[] = {
// 0b00000000, 0b00000001, 0b00000010, 0b00000011, 0b00000100, 0b00000101, 0b00000110, 0b00000111, 0b00001000, 0b00001001,
// 0b00001010, 0b00001011, 0b00001100, 0b00001101, 0b00001110, 0b00001111, 0b00010000, 0b00010001, 0b00010010, 0b00010011,
// 0b00010100, 0b00010101, 0b00010110, 0b00010111, 0b00011000, 0b00011001, 0b00011010, 0b00011011, 0b00011100, 0b00011101,
// 0b00011110, 0b00011111, 0b00100000, 0b00100001, 0b00100010, 0b00100011, 0b00100100, 0b00100101, 0b00100110, 0b00100111,
// 0b00101000, 0b00101001, 0b00101010, 0b00101011, 0b00101100, 0b00101101, 0b00101110, 0b00101111, 0b00110000, 0b00110001,
// 0b00110010, 0b00110011, 0b00110100, 0b00110101, 0b00110110, 0b00110111, 0b00111000, 0b00111001, 0b00111010, 0b00111011,
// 0b00111100, 0b00111101, 0b00111110, 0b00111111
// };

// char toRequest[] = { 0b00100010, 0b00100011, 0b00100100, 0b00100101, 0b00100110, 0b00100111, 0b00101000, 0b00101001, 0b00101010 };


int led_pin = LED_BUILTIN;
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};

/*Unidade 0*/
#define unidade_1 0b11000001
#define unidade_2 0b11000010
#define unidade_3 0b11000011
#define todas_unidades 0b11111110

#define espera 1 //tem que ser referente ao valor da unidade

/*Valores da led*/
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

/*Script do problema - Exemplos respostas*/
#define problema  0x01
#define funcionando  0x02
#define resposta_Digital  0x11
#define resposta_Analogica  0x12


/*Variáveis*/
bool unidade = false, opcao = false, sensorProblema = false;
int valor = 0, testeSensor[9];
unsigned char dado_digital, unidadeAtual = unidade_1;
byte dest[4];

void setup() {

  // pinMode(D0, INPUT);
  /* BaudRate */
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  /* switch off led */
  digitalWrite(led_pin, LOW);

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

  pinMode(led_pin, OUTPUT); /* configura LED embutido como saída */  
  pinMode(buttonD0, INPUT); /* configura D0 embutido como entrada (botão) */    
  pinMode(buttonD1, INPUT); /* configura D1 embutido como entrada (botão) */    
  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);
}

// This function is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// When a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  if(topic == "comandos"){
    String messageTemp, value;
  
    for (int i = 0; i < length; i++) {
      messageTemp += (char)message[i];
    }

    opcao = false;
    /* ====================== Situação 1 -> Caso tenha e a unidade esteja desativada: =========================*/
    if (unidade == false){
    /* ====================== Se o que foi lido for igual ao código da unidade: =========================*/
      if (messageTemp == unidadeAtual){
        unidade = true;
        value = unidadeAtual;
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }    
    /* ====================== Se o que foi lido for igual ao código de todas unidades: =========================*/
      else if (messageTemp == todas_unidades){
        unidade = true;
        delay(espera);
        value = unidadeAtual;
        
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }    
    }
    else if (unidade == true){
      /*======================== Acende ou apaga a LED =============================*/
      if(messageTemp == "0b00100001"){ 
        if(digitalRead(led_pin) == HIGH){
          digitalWrite(led_pin, LOW);
          value = "0b00000000"; 
        }
        else if(digitalRead(led_pin) == LOW){
          digitalWrite(led_pin, HIGH);
          value = "0b00000001";
        }
      // value = "0b00000001"; //ok    

        delay(1500);
        client.publish("respostas", String(value).c_str());
      } 
      else if (messageTemp == entrada_analogica){
        value = analogRead(D0);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }  


      /*======================== Situação Atual =============================*/ 
      else if(messageTemp == situacao_sensor){
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
          //value
          delay(1500);
          client.publish("respostas", String(value).c_str());
        }
        else{
          value = problema;
          //value
          delay(1500);
          client.publish("respostas", String(value).c_str());
        }
      } 
      /*======================== Le o potenciometro =============================*/          
      else if(c == entrada_analogica){
        valor = analogRead(A0); // Le a porta analogica e salva o valor na variavel
        // // Particiona o int (12 bits) em três palavras de 8 bits
        // dest[0] = valor         & 0xff;
        // dest[1] = (valor >> 8)  & 0xff; 
        // dest[2] = (valor >> 16)  & 0xff; 
        
        // Serial.write(dest[0]);
        // Serial.write(dest[1]);
        // Serial.write(dest[2]);

        value = analogRead(A0);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());

      /*================== Le as Entradas Digitais e envia os valores das portas ======================*/ 
      else if (messageTemp == entrada_digital_0){
        value = digitalRead(D0);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_1){
        value = digitalRead(D1);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_2){
        value = digitalRead(D2);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_3){
        value = digitalRead(D3);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_4){
        value = digitalRead(D4);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_5){
        value = digitalRead(D5);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_6){
        value = digitalRead(D6);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_7){
        value = digitalRead(D7);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }
      else if (messageTemp == entrada_digital_8){
        value = digitalRead(D8);
        //value
        delay(1500);
        client.publish("respostas", String(value).c_str());
      }

      /*======== Situacao 3 -> desativa apenas a unidade ==================*/
      if (messageTemp == unidadeAtual){
        unidade = false;
      }    
      /*======== Situacao 4 -> desativa todas as unidades ==================*/
      else if (messageTemp == todas_unidades){
        unidade = false;
      }  
      /*
      Testar 
      readSensors(messageTemp);
      */
    }
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect("ESP8266Client", MQTT_username, MQTT_password)) {
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("comandos");
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
/*
void readSensors(){             
  if(pc == toRequest[3] ){ 
    digitalWrite(led_pin, LOW);
  }
  if(pc == toRequest[4] ){  
    digitalWrite(led_pin, HIGH);
  }      
}
*/
void loop() {
  ArduinoOTA.handle();

  if (!client.connected()) {
  reconnect();
  }

  if(!client.loop())
    client.connect("ESP8266Client", MQTT_username, MQTT_password);  
}