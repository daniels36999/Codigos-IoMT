ResTemp=0;
ResPeso=0;
ResAltura=0;
ResImc=0;
ResO2Sat=0;

// Create a client instance
client = new Paho.MQTT.Client("test.mosquitto.org", 8080, "myclientid_" + parseInt(Math.random() * 100, 10))
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;
client.connect({ onSuccess: onConnect });

function onConnect() {
    console.log("CONECTADO")
    console.log("onConnect");
    client.subscribe("Cabina/IoMT/Datos");
    message = new Paho.MQTT.Message("SE HA ESTABLECIDO LA CONEXIÓN");
    console.log("SE HA ESTABLECIDO LA CONEXIÓN")
    message.destinationName = "Cabina/IoMT/Conexion";
    client.send(message);
	
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("Conexión Perdida:" + responseObject.errorMessage);
    }
}

// called when a message arrives
function onMessageArrived(message) {
	sms=(message.payloadString);
	console.log(sms);
	VectorDatos=sms.split(";")
	ResTemp=VectorDatos[0];
	document.getElementById("TEMP1").innerHTML=ResTemp;
	ResPeso=VectorDatos[1];
	document.getElementById("PES1").innerHTML=ResPeso;
	ResAltura=VectorDatos[2];
	document.getElementById("EST1").innerHTML=ResAltura;
	ResImc=VectorDatos[3];
	document.getElementById("IMC1").innerHTML=ResImc;
	ResO2Sat=VectorDatos[4];
	document.getElementById("O2SAT1").innerHTML=ResO2Sat;
}