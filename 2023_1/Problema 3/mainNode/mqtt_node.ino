#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


// MQTT Broker
const char *mqtt_broker = "10.0.0.1";
const char *topic = "esp8266/test";
const char *mqtt_username = "aluno";
const char *mqtt_password = "@luno*123";
const int mqtt_port = 1883;


//Defina o agente MQTT, escreva a função de retorno de chamada e
// imprima as informações de conexão no monitor serial ao mesmo tempo.

client.setServer(mqtt_broker, mqtt_port);
client.setCallback(callback);
while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("O cliente %s se conecta ao intermediário mqtt público\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
    } else {
        Serial.print("falha com o estado ");
        Serial.print(client.state());
        delay(2000);
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Mensagem chegou no tópico:  ");
    Serial.println(topic);
    Serial.print("Mensagem:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}


//Depois de se conectar com sucesso ao broker MQTT, o ESP8266 publicará mensagens e assinará o broker MQTT.


// publish and subscribe
client.publish(topic, "hello emqx");
client.subscribe(topic);


//Imprima o nome do tópico na porta serial e, em seguida, imprima cada byte de mensagens recebidas.
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Mensagem chegou no tópico: ");
    Serial.println(topic);
    Serial.print("Mensagem:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}
