// const labels = [
//     'node1',
//     'node2'
// ];
$(document).ready(function () {

    function add_data(label, data) {
        myChart.data.labels.push(label);
        myChart.data.datasets.forEach((dataset) => {
            dataset.data.push(data);
        });
        myChart.update();
    }

    function remove_firstData() {
        myChart.data.labels.splice(0, 1);
        myChart.data.datasets.forEach((dataset) => {
            dataset.data.shift();
        });
    }

    const ctx = document.getElementById('myChart').getContext("2d");
    const myChart = new Chart(ctx, {
        type: 'line',
        data: {
            datasets: [{ label: "Valor do sensor", }],
            backgroundColor: "purple",
            borderColor: "purple",
        },
        options:{
            resposive: true
        }
    });


    const contador_max = 10;

    const mqtt = require('mqtt')
    const url = 'tcp://10.0.0.101:1883@@luno*123'
    const topic = 'front'
    const resultado = []

    // Cria configuração de conexão mqtt
    const options = {
    clean: true,
    clientId: 'SBC',
    username: 'aluno',
    password: '@luno*123',
    }
    const client  = mqtt.connect(url, options)

    client.on('connect', function () {
        if(!err){
            console.log('------------ Conectado! -------------');
            client.subscribe(topic);
        } else{
            console.log("Não conectado!");
        }
    });

    client.on(topic, function (message) {
        console.log(message.toString());
        let messageTemp = message.toString();

        resultado = messageTemp.split(",", 4);  //data/ Node x/ pino/ informação
        // var data = resultado[0]
        // var node = resultado[1]
        // var pino = resultado[2]
        // var informacao = resultado[3]

        console.log("Recebendo:", + resultado[0] + "Node: " + 
                    resultado[1] + "----->" + resultado[2] + " : " + resultado[3]);

        if(myChart.data.labels.length > contador_max) {
            remove_firstData();
        }
        add_data(msg.date, msg.value);

        client.end();
    });



});







// Receive messages
// client.on('message', function (topic, message) {
//   // message is Buffer
//   console.log(message.toString())
//   client.end()
// })

//https://www.emqx.com/en/blog/mqtt-js-tutorial