import joblib
import paho.mqtt.client as mqtt

# Load AI model
model = joblib.load("model.pkl")

# Store latest values
battery = {
    "voltage": 0.0,
    "current": 0.0,
    "temperature": 0.0,
    "soc": 0.0
}

# MQTT Connected
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT Broker!")

    client.subscribe("bms/voltage")
    client.subscribe("bms/current")
    client.subscribe("bms/temperature")
    client.subscribe("bms/soc")

# MQTT Message Received
def on_message(client, userdata, msg):

    topic = msg.topic
    value = float(msg.payload.decode())

    if topic == "bms/voltage":
        battery["voltage"] = value

    elif topic == "bms/current":
        battery["current"] = value

    elif topic == "bms/temperature":
        battery["temperature"] = value

    elif topic == "bms/soc":
        battery["soc"] = value

    data = [[
        battery["voltage"],
        battery["current"],
        battery["temperature"],
        battery["soc"]
    ]]

    prediction = model.predict(data)[0]

    print("\n========== LIVE BMS ==========")
    print("Voltage     :", battery["voltage"], "V")
    print("Current     :", battery["current"], "A")
    print("Temperature :", battery["temperature"], "°C")
    print("SoC         :", battery["soc"], "%")
    print("Prediction  :", prediction)

    client.publish("bms/ai_prediction", prediction)

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883)

client.loop_forever()