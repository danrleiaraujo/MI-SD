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
    