//includes

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>


//configurações
#define MQTT_ADDRESS "tcp://10.0.0.1"
#define CLIENTID "aluno" 
#define PASSWORD "@luno*123"
#define port 1883
#define topic "orange"


MQTTClient client;

int rc;

void connect_mqtt(){
    client on_connect(){
        MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

        //criação do cliente com os dados de endereço
        MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
        MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
        
        //faz conexão com o cliente
        rc = MQTTClient_connect(client, &conn_opts);
        
        //se nao retornar 0 (sucesso)
        if (rc == MQTTCLIENT_SUCCESS)
        {
                printf("\n\rConectado ao broker. Status: %d\n", rc);
        }
        else{
                printf("\n\rFalha na conexao ao broker MQTT. Estado de erro: %d\n", rc);
                exit(-1);
        }
    
        client = mqtt_client.Client(CLIENTID)
        client.username_pw_set(username, password)
        client.on_connect = on_connect
        client.connect(broker, port)
        return client
    }
   

}