#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MQTTClient.h"
#include <netdb.h>

app = Flask(__name__)
app.config['SECRET_KEY'] = 'donsky!'
socketio = SocketIO(app, cors_allowed_origins='*');

///////////         data/ Node x/ pino/ informação:



time_t agora;
char datahora[100];

/* Recupera a quantidade de segundos desde 01/01/1970 */
agora = time(NULL);

/* Formata a data e a hora da forma desejada */
strftime( datahora, sizeof(datahora), "%d.%m.%Y - %H:%M:%S", localtime( &agora ) );

printf( "Data/Hora: %s\n", datahora );



////////////////////IMPORTANTE
socketio.emit('resposta_node', {'data': datahora, "node": dataH+, "pino":+vazao,  "informacacao": dataH+})
            socketio.sleep(1)   
////////////////////IMPORTANTE

data/ Node x/ pino/ informação:
"""
Ele chama a thread que se inscreve no tópico do hidrômetro
"""
@socketio.on('connect')
def connect():
    global thread
    print('Client connected')

    global thread
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(background_thread)


"""
Quando disconectamos
"""
@socketio.on('disconnect')
def disconnect():
    print('Client disconnected',  request.sid)

//inicializando a API
if __name__ == '__main__':
    socketio.run(app, host='172.16.103.6', port=5002) //ip da máquina com a api 172.16.103.6
    





time_t agora;
char datahora[100];
agora = time(NULL);
strftime( datahora, sizeof(datahora), "%d.%m.%Y - %H:%M:%S", localtime( &agora ));
msg_front = datahora + "," + unidadeescolhida + "," + "A0," + valoranalog;
publisher(TOPICFRONT, msg_front);

/*-------------- Criacao da mensagem -----------*/
pubmsg.payload = unidadeEscolhida;
pubmsg.payloadlen = (int)strlen(unidadeEscolhida);
pubmsg.qos = QOS;
pubmsg.retained = 0;
/*-------------- Tentativa de publicacao -----------*/
if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS){
    printf("Falha no envio, tente novamente\n");
    //exit(EXIT_FAILURE);
}    


                            //data/ Node x/ pino/ informação


 /* ========================= MQTT -  ENVIO PARA O FRONT =============================*/
    agora = time(NULL);
    strftime( datahora, sizeof(datahora), "%d.%m.%Y - %H:%M:%S", localtime( &agora ));
    msg_front = datahora + "," + unidadeSelecionada + "," + "A0," + valoranalog;
    publisher(TOPICFRONT, msg_front);    
/* =================================================================================*/                           

    char msg_front[100];

    
    time_t agora;
    char datahora[100];
    
    /* ========================= MQTT -  ENVIO PARA O FRONT =============================*/
    agora = time(NULL);
    strftime( datahora, sizeof(datahora), "%d.%m.%Y - %H:%M:%S", localtime( &agora ));

    char aux_unidEscolhida = unidadeSelecionada+'0';
    char valorSensor = +'0';
    
    concatenar(msg_front, datahora);
    concatenar(msg_front, ",");
    concatenar(msg_front, aux_unidEscolhida);
    concatenar(msg_front, ",A0,");
    concatenar(msg_front, valor);    
    
    publisher(TOPICFRONT, msg_front);

    limpaVetor_comum (msg_front,100);
    limpaVetor_comum (datahora,100)
    /* =================================================================================*/                           
