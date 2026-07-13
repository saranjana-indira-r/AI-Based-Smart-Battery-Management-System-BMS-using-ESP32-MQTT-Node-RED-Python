/*****************************************************
 * AI Based Smart Battery Management System
 * Phase 2 - Part 1
 * ESP32 + OLED + Wokwi
 *****************************************************/

#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
//================ OLED ====================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);
const char* ssid = "Wokwi-GUEST";
const char* password = "";
//================ Pins ====================

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define TEMP_PIN    32

#define GREEN_LED   18
#define YELLOW_LED  19
#define RED_LED     23

#define BUZZER      25

//================ Variables ===============

float voltage = 0;
float current = 0;
float temperature = 0;

int soc = 0;
int soh = 98;

String status = "NORMAL";
String fault = "NONE";

// Screen Management
int currentScreen = 0;
unsigned long previousScreenMillis = 0;
const unsigned long screenInterval = 3000;

//================ Function Prototypes =================

void startupAnimation();

void readSensors();

void calculateSOC();

void protectionLogic();

void updateOutputs();

void drawBattery(int x, int y, int percent);

void dashboardScreen();

void batteryScreen();

void protectionScreen();

void serialOutput();
void connectWiFi()
{
  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("================================");
  Serial.println("WiFi Connected Successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("================================");
}
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT...");

    String clientId = "ESP32-BMS-";
    clientId += String(random(1000));

    if (client.connect(clientId.c_str()))
    {
      Serial.println(" Connected!");
    }
    else
    {
      Serial.print(" Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying...");
      delay(2000);
    }
  }
}
void publishData()
{
  client.publish("bms/voltage", String(voltage,2).c_str());

  client.publish("bms/current", String(current,2).c_str());

  client.publish("bms/temperature", String(temperature,1).c_str());

  client.publish("bms/soc", String(soc).c_str());

  client.publish("bms/status", status.c_str());
}
//================ SETUP ===================

void setup()
{
  Serial.begin(115200);
  connectWiFi();
  
  client.setServer(mqtt_server, 1883);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  Wire.begin(21,22);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
  {
    Serial.println("OLED Failed");

    while(true);
  }

  startupAnimation();
}

//================ LOOP ====================

void loop()
{
  if (!client.connected())
{
  reconnectMQTT();
}

client.loop();
  readSensors();

  calculateSOC();

  protectionLogic();

  updateOutputs();

  if(millis()-previousScreenMillis>screenInterval)
  {
      currentScreen++;

      if(currentScreen>2)
          currentScreen=0;

      previousScreenMillis=millis();
  }

  if(currentScreen==0)
      dashboardScreen();

  else if(currentScreen==1)
      batteryScreen();

  else
      protectionScreen();

  serialOutput();

  delay(300);
  publishData();
}
//==================================================
// STARTUP ANIMATION
//==================================================

void startupAnimation()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(12,8);
  display.println("SMART");

  display.setCursor(30,34);
  display.println("BMS");

  display.display();

  delay(1500);

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(18,12);
  display.println("Initializing...");

  display.drawRect(10,35,108,10,SSD1306_WHITE);

  for(int i=0;i<=100;i+=5)
  {
      int bar = map(i,0,100,0,106);

      display.fillRect(11,36,bar,8,SSD1306_WHITE);

      display.display();

      delay(50);
  }

  delay(500);
}

//==================================================
// READ SENSOR VALUES
//==================================================

void readSensors()
{

  int rawVoltage = analogRead(VOLTAGE_PIN);

  int rawCurrent = analogRead(CURRENT_PIN);

  int rawTemp = analogRead(TEMP_PIN);

  // Voltage
  voltage = map(rawVoltage,0,4095,300,420)/100.0;

  // Current
  current = map(rawCurrent,0,4095,0,1000)/100.0;

  // Temperature
  temperature = map(rawTemp,0,4095,20,80);

}

//==================================================
// CALCULATE BATTERY PERCENTAGE
//==================================================

void calculateSOC()
{

  soc = map(voltage*100,300,420,0,100);

  if(soc>100)
      soc=100;

  if(soc<0)
      soc=0;

}
//==================================================
// PROTECTION LOGIC
//==================================================

void protectionLogic()
{
  // Reset status
  status = "NORMAL";
  fault = "NONE";

  // -------- Voltage Protection --------
  if (voltage > 4.20)
  {
    status = "CRITICAL";
    fault = "OVER VOLTAGE";
  }
  else if (voltage < 3.00)
  {
    status = "WARNING";
    fault = "LOW BATTERY";
  }

  // -------- Temperature Protection --------
  if (temperature > 60)
  {
    status = "CRITICAL";
    fault = "OVER TEMPERATURE";
  }
  else if (temperature > 45 && status != "CRITICAL")
  {
    status = "WARNING";
    fault = "HIGH TEMPERATURE";
  }

  // -------- Current Protection --------
  if (current > 5.0)
  {
    status = "CRITICAL";
    fault = "OVER CURRENT";
  }
}

//==================================================
// UPDATE LEDS & BUZZER
//==================================================

void updateOutputs()
{
  // Turn everything OFF first
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  if (status == "NORMAL")
  {
    digitalWrite(GREEN_LED, HIGH);
  }
  else if (status == "WARNING")
  {
    digitalWrite(YELLOW_LED, HIGH);
  }
  else if (status == "CRITICAL")
  {
    digitalWrite(RED_LED, HIGH);

    // Alarm
    digitalWrite(BUZZER, HIGH);
  }
}
//==================================================
// DRAW BATTERY ICON
//==================================================

void drawBattery(int x, int y, int percent)
{
  // Outline
  display.drawRect(x, y, 26, 12, SSD1306_WHITE);

  // Terminal
  display.drawRect(x + 26, y + 3, 2, 6, SSD1306_WHITE);

  // Battery fill
  int fill = map(percent, 0, 100, 0, 24);

  if(fill < 0)
    fill = 0;

  if(fill > 24)
    fill = 24;

  display.fillRect(x + 1, y + 1, fill, 10, SSD1306_WHITE);
}

//==================================================
// DASHBOARD SCREEN
//==================================================

void dashboardScreen()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.println("SMART BMS");

  drawBattery(95,0,soc);

  display.drawLine(0,12,127,12,SSD1306_WHITE);

  display.setCursor(0,18);
  display.print("Voltage : ");
  display.print(voltage,2);
  display.println("V");

  display.print("Current : ");
  display.print(current,2);
  display.println("A");

  display.print("Temp    : ");
  display.print(temperature,1);
  display.println("C");

  display.display();
}

//==================================================
// BATTERY SCREEN
//==================================================

void batteryScreen()
{
  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(25,0);
  display.println("BATTERY");

  drawBattery(48,15,soc);

  display.setCursor(25,35);
  display.print("SoC : ");
  display.print(soc);
  display.println("%");

  display.print("SoH : ");
  display.print(soh);
  display.println("%");

  display.display();
}

//==================================================
// PROTECTION SCREEN
//==================================================

void protectionScreen()
{
  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(18,0);
  display.println("PROTECTION");

  display.drawLine(0,12,127,12,SSD1306_WHITE);

  display.setCursor(0,18);

  display.print("Status : ");
  display.println(status);

  display.print("Fault  : ");
  display.println(fault);

  display.print("Battery: ");

  if(status=="NORMAL")
      display.println("SAFE");

  else if(status=="WARNING")
      display.println("WARNING");

  else
      display.println("CRITICAL");

  display.display();
}

//==================================================
// SERIAL MONITOR
//==================================================

void serialOutput()
{
  Serial.println();

  Serial.println("===================================");

  Serial.println("SMART BATTERY MANAGEMENT SYSTEM");

  Serial.println("===================================");

  Serial.print("Voltage      : ");
  Serial.print(voltage,2);
  Serial.println(" V");

  Serial.print("Current      : ");
  Serial.print(current,2);
  Serial.println(" A");

  Serial.print("Temperature  : ");
  Serial.print(temperature,1);
  Serial.println(" C");

  Serial.print("Battery SoC  : ");
  Serial.print(soc);
  Serial.println(" %");

  Serial.print("Battery SoH  : ");
  Serial.print(soh);
  Serial.println(" %");

  Serial.print("Status       : ");
  Serial.println(status);

  Serial.print("Fault        : ");
  Serial.println(fault);

  Serial.println("===================================");
}