const labels = [
    'node1',
    'node2'
];

const data = {
    labels: labels,
    datasets: [{
        data: [0 , 1 , 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100],
        label: "Valor do sensor",
        backgroundColor: "purple",
        borderColor: "purple"
    }]
};

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
    data: data,
    options:{
        resposive: true
    }
});


const contador_max = 10;
console.log("Dado recebido: data/ Node x/ pino/ informação:", msg.data + " - " + msg.node + "----->"+ msg.pino + " :: " + msg.informacao);
XMLDocument

