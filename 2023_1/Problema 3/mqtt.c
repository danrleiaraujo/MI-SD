#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

//dontpad dopbl
 
#define ADDRESS     "tcp://10.0.0.101:1883@@luno*123"
#define CLIENTID_PUB    "SBC"
#define CLIENTID_SUB    "NODEMCU"
#define USERNAME	"aluno"
#define PASSWORD	"@luno*123"

#define MQTT_PUBLISH_TOPIC     "comandos"
#define MQTT_SUBSCRIBE_TOPIC   "respostas"

/* binário */
#define PAYLOAD     "0b00100001" // Alternar estado do led 

#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void connlost(void *context, char *cause);
int publisher();
int subscriber();
 
int main(int argc, char* argv[])
{
    publisher();
    delay(1000);
    subscriber();
}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
 
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
 
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
 
int publisher(){
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        conn_opts.username = USERNAME;
        conn_opts.password = PASSWORD;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID_PUB,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);


    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
 
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, MQTT_PUBLISH_TOPIC, &pubmsg, &token);
 
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, MQTT_PUBLISH_TOPIC, CLIENTID_PUB);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    rc = MQTTClient_disconnect(client, 10000);

    MQTTClient_destroy(&client);
    return rc;
}

int subscriber(){
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        conn_opts.username = USERNAME;
        conn_opts.password = PASSWORD;
    int rc;
 
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID_SUB,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto exit;
    }
 
    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }
 
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }
 
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", MQTT_SUBSCRIBE_TOPIC, CLIENTID_SUB, QOS);
    if ((rc = MQTTClient_subscribe(client, MQTT_SUBSCRIBE_TOPIC, QOS)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to subscribe, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }
    else
    {
        int ch;
        do
        {
                ch = getchar();
        } while (ch!='Q' && ch != 'q');
 
        if ((rc = MQTTClient_unsubscribe(client, MQTT_SUBSCRIBE_TOPIC)) != MQTTCLIENT_SUCCESS)
        {
                printf("Failed to unsubscribe, return code %d\n", rc);
                rc = EXIT_FAILURE;
        }
    }
 
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        rc = EXIT_FAILURE;
    }
    destroy_exit:
        MQTTClient_destroy(&client);
    exit:
    return rc;
}