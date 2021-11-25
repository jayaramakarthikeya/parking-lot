// pin numbers
#define TRIGGERPIN D1
#define ECHOPIN    D2
#define BUZZERPIN D7
#define REDPIN D0
#define GREENPIN D3
#define BLUEPIN D4
#include <math.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

long duration;
float distance,prevDes;
int i = 0,j = 0;
float total_dist = 140;

const char* ssid = "karthik";
const char* password = "karthikeya";
const char* mqtt_server = "192.168.43.95";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("led_topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

float distance_calc(){

  float dist;

  digitalWrite(TRIGGERPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN , HIGH);

  dist = duration*0.034/2;
  return dist;

}

void buzzer(){
  digitalWrite(BUZZERPIN,HIGH);
  digitalWrite(REDPIN,LOW);
  digitalWrite(GREENPIN,HIGH);
  digitalWrite(BLUEPIN,LOW);
  delay(5000);
  digitalWrite(BUZZERPIN,LOW);
  delay(1000);
}

void setup() {

  pinMode(TRIGGERPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(BUZZERPIN,OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {

 if (!client.connected()) {
    reconnect();
  }
  client.loop();

 distance = distance_calc();

 long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    value= (int)distance;
    snprintf (msg, 75, "%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("sensor_data", msg);
  }
 
 while(i<5){

  prevDes = distance;

  if((int)ceil(distance)>20){
    i = 0;
    j = 0;
    digitalWrite(REDPIN,LOW);
    digitalWrite(GREENPIN,LOW);
    digitalWrite(BLUEPIN,HIGH);
    delay(1000);
    break;
  }
  else if((distance < 20 && (int)ceil(distance) == (int)prevDes) || (distance < 20 && (int)floor(distance) == (int)prevDes)){
        i++;
        distance = distance_calc();

        now = millis();
        delay(500);
        if (now - lastMsg > 2000) {
            lastMsg = now;
            value= (int)distance;
            snprintf (msg, 75, "%ld", value);
            Serial.print("Publish message: ");
            Serial.println(msg);
            client.publish("sensor_data", msg);
          }
  }

  delay(1000);

 }
 
 if(i==5){
  if(j==0 && distance < 20){
      buzzer();
      i = 0;
      j = j + 1;
      
  }
  else if(j>0 && distance < 20){    
    digitalWrite(REDPIN,LOW);
    digitalWrite(GREENPIN,HIGH);
    digitalWrite(BLUEPIN,LOW);
    delay(5000);
  }
  else{
    digitalWrite(REDPIN,LOW);
    digitalWrite(GREENPIN,LOW);
    digitalWrite(BLUEPIN,HIGH);
    delay(2000);
    i = 0;
  }
 }
}
