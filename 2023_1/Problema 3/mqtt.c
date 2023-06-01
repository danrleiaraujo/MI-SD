* Includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>
 
/*
* Defines
*/
#define MQTT_ADDRESS   "tcp://10.0.0.101:1883@@luno*123"
#define CLIENTID       "aluno"  
#define USERNAME	"aluno"
#define PASSWORD	"@luno*123"
#define MQTT_PUBLISH_TOPIC     "MQTTCClientPubTopic"
#define MQTT_SUBSCRIBE_TOPIC   "MQTTCClientSubTopic"
 
/*
*  Variaveis globais
*/
MQTTClient client;
 
/*
* Prototipos de funcao
*/
void publish(MQTTClient client, char* topic, char* payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);
 
/*
* Implementacoes
*/
 
/* Funcao: publicacao de mensagens MQTT
*/
void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
 
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}
 
/* Funcao: callback de mensagens MQTT recebidas e echo para o broker
 * Parametros: contexto, ponteiro para nome do topico da mensagem recebida, 
 * tamanho do nome do topico e mensagem recebida
 * Retorno : 1: sucesso (fixo / nao ha checagem de erro neste exemplo)
*/
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;
 
    /* Mostra a mensagem recebida */
    printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName, payload);
 
    /* Faz echo da mensagem recebida */
    publish(client, MQTT_PUBLISH_TOPIC, payload);
 
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
 
 
int main(int argc, char *argv[])
{
   int rc;
   MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
 
   /* Inicializacao do MQTT (conexao & subscribe) */
   MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
   MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
 
   rc = MQTTClient_connect(client, &conn_opts);
 
   if (rc != MQTTCLIENT_SUCCESS)
   {
       printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
       exit(-1);
   }
 
   MQTTClient_subscribe(client, MQTT_SUBSCRIBE_TOPIC, 0);
 
   while(1)
   {
       /*
        * o exemplo opera por "interrupcao" no callback de recepcao de 
        * mensagens MQTT. Portanto, neste laco principal eh preciso fazer
        * nada.
        */
   }
}