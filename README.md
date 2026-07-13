# AI-Based-Smart-Battery-Management-System-BMS-using-ESP32-MQTT-Node-RED-Python
Developed a simulated Battery Management System that monitors voltage, current, temperature, and State of Charge (SoC). Implemented protection logic with real-time MQTT communication, a Node-RED dashboard, and a Python-based prediction module that classifies battery status as NORMAL, WARNING, or CRITICAL.

Overview

The AI-Based Smart Battery Management System (BMS) is an IoT-based embedded system that monitors important battery parameters in real time. The system uses an ESP32 microcontroller to simulate battery voltage, current, temperature, and State of Charge (SoC). The collected data is displayed on an OLED display, transmitted through MQTT, visualized on a Node-RED dashboard, and analyzed using a Python-based prediction model.

This project demonstrates the integration of Embedded Systems, IoT, MQTT, Node-RED, and Machine Learning for smart battery monitoring and protection.

---

Objectives

- Monitor battery voltage, current, temperature, and SoC.
- Provide battery protection using predefined safety limits.
- Display real-time battery information on an OLED display.
- Send battery data to an MQTT broker through Wi-Fi.
- Visualize battery parameters on a Node-RED dashboard.
- Predict battery condition (NORMAL, WARNING, or CRITICAL) using Python.
- Demonstrate a low-cost smart BMS suitable for learning and academic projects.

---

Features

- Real-time battery monitoring
- Voltage monitoring
- Current monitoring
- Temperature monitoring
- State of Charge (SoC) estimation
- OLED display output
- LED status indication
- Buzzer alarm for abnormal conditions
- Wi-Fi communication
- MQTT data transmission
- Node-RED dashboard
- AI-based battery status prediction
- Wokwi simulation support

---

Hardware Components

- ESP32 DevKit V1
- SSD1306 OLED Display (I2C)
- 3 Potentiometers (Voltage, Current, Temperature simulation)
- Green LED
- Yellow LED
- Red LED
- Buzzer
- Breadboard
- Jumper Wires

---

Software Used

- Arduino IDE
- Wokwi Simulator
- Node-RED
- MQTT (HiveMQ Public Broker)
- Python 3.11
- Visual Studio Code
- Git & GitHub

---

Programming Languages

- Embedded C/C++
- Python
- JSON

---

Libraries Used

ESP32

- WiFi.h
- PubSubClient.h
- Wire.h
- Adafruit_GFX.h
- Adafruit_SSD1306.h

Python

- pandas
- scikit-learn
- joblib
- paho-mqtt

---

Project Workflow

1. Potentiometers simulate battery voltage, current, and temperature.
2. ESP32 reads analog values.
3. Battery parameters are calculated.
4. OLED displays live battery information.
5. Protection logic checks battery conditions.
6. LEDs and buzzer indicate battery status.
7. ESP32 publishes data using MQTT.
8. Node-RED displays real-time battery data.
9. Python predicts battery status.
10. AI prediction is displayed on the dashboard.

---

Protection Logic

Normal

- Voltage between safe limits
- Temperature below 45°C
- Current below 5 A

Warning

- Low battery voltage
- Temperature above 45°C

Critical

- Voltage greater than 4.20 V
- Temperature greater than 60°C
- Current greater than 5 A

---

MQTT Topics

Topic| Description
bms/voltage| Battery Voltage
bms/current| Battery Current
bms/temperature| Battery Temperature
bms/soc| Battery State of Charge
bms/status| Battery Status
bms/ai_prediction| AI Prediction

---

Folder Structure

AI-Based-Smart-Battery-Management-System
│
├── code/
│   └── main.ino
│
├── python/
│   ├── battery_data.csv
│   ├── train_model.py
│   ├── predict.py
│   ├── mqtt_ai.py
│   └── model.pkl
│
├── simulation/
│   ├── diagram.json
│   └── wokwi-project.json
│
├── node-red/
│   └── flow
│
├── images/
│   ├── circuit.png
│   ├── oled_output.png
│   ├── dashboard.png
│   └── ai_prediction.png
│
├── docs/
│   └── Project_Report.pdf
│
└── README.md

---

Results

- Successfully monitored battery voltage, current, temperature, and SoC.
- Displayed battery information on an OLED display.
- Implemented battery protection logic.
- Published battery data using MQTT.
- Visualized battery parameters on a Node-RED dashboard.
- Generated battery status predictions (NORMAL, WARNING, and CRITICAL) using a Python model.

---

Applications

- Electric Vehicles (EV)
- Solar Energy Storage Systems
- UPS Battery Monitoring
- Robotics
- Drones
- Portable Energy Storage Systems
- Smart IoT Battery Monitoring

---

Future Scope

- Battery State of Health (SoH) estimation
- Cell balancing algorithm
- Cloud database integration
- Mobile application for remote monitoring
- Predictive maintenance using larger datasets
- Support for real battery sensors instead of simulated inputs

---

Conclusion

This project demonstrates a low-cost, AI-assisted Smart Battery Management System using ESP32, MQTT, Node-RED, and Python. It combines embedded programming, IoT communication, dashboard visualization, and battery status prediction into a single integrated system. The project provides a practical platform for learning battery monitoring concepts and serves as a strong academic and portfolio project in the fields of Embedded Systems, IoT, Electric Vehicles, and Renewable Energy.

---

Author

Saranjana Indira R

Bachelor of Engineering (Electrical and Electronics Engineering)

Cambridge Institute of Technology, Bengaluru

---

License

This project is developed for educational and academic purposes.
