//includes

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>


//configurações
#define broker "tcp://10.0.0.1"
#define username "aluno" 
#define password "@luno*123"
#define port 1883
#define topic "orange"


MQTTClient client;


void connect_mqtt():


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