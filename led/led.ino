/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "olleh_WiFi_65EB";//wifi id
const char* password = "0000001774";//wifi pw
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* device_id = "mqtt_led";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


int led1 = D2;
int led2 = D3;
int led3 = D4;
int led4 = D5;

int all_state = 0; // 0 = normal, 1 = all on , 2 = all off
int led1_state = 0; //0 = off , 1 = on
int led2_state = 0;
int led3_state = 0;
int led4_state = 0;

int sw1 = D6;
int sw2 = D7;
int sw3 = D8;
int sw4 = D9;


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

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

  String topic_a = topic;

  // Switch on the LED if an 1 was received as first character
  if(topic_a == "ROOM1"){
    if ((char)payload[0] == '0') {
      digitalWrite(led1, LOW);
    }
    else{
      digitalWrite(led1, HIGH);
    }
  }
  else if(topic_a == "ROOM2"){
    if ((char)payload[0] == '0'){
      digitalWrite(led2,LOW); 
    }
    else{
      digitalWrite(led2, HIGH);
    }
  }
  else if(topic_a == "ROOM3"){
    if ((char)payload[0] == '0'){
      digitalWrite(led3, LOW); 
    }
    else{
      digitalWrite(led3, HIGH);
    }
  }
  else if(topic_a == "ROOM4"){
    if ((char)payload[0] == '0'){
      digitalWrite(led4, LOW); 
    }
    else{
      digitalWrite(led4, HIGH);
    }
  }
  else if(topic_a == "ALL_ROOM"){
    if((char)payload[0] == '0'){
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      digitalWrite(led3,LOW);
      digitalWrite(led4,LOW);
    }else{
      digitalWrite(led1,HIGH);
      digitalWrite(led2,HIGH);
      digitalWrite(led3,HIGH);
      digitalWrite(led4,HIGH);
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("ROOM1");
      client.subscribe("ROOM2");
      client.subscribe("ROOM3");
      client.subscribe("ROOM4");
      client.subscribe("ALL_ROOM");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*void switch_led(){
  if(digitalRead(sw1)==LOW){// 버튼 제어
    if(led1_state == 0){
      digitalWrite(led1,HIGH);
      led1_state = 1;
    }
    else if(led1_state == 1){
      digitalWrite(led1,LOW);
      led1_state = 0;
    }
 }

if(digitalRead(sw2)==LOW){
    if(led2_state == 0){
      digitalWrite(led2,HIGH);
      led2_state = 1;
    }
    else if(led2_state == 1){
      digitalWrite(led2,LOW);
      led2_state = 0;
    }
 }

 if(digitalRead(sw3)==LOW){
    if(led3_state == 0){
      digitalWrite(led3,HIGH);
      led3_state = 1;
    }
    else if(led3_state == 1){
      digitalWrite(led3,LOW);
      led3_state = 0;
    }
 }

 if(digitalRead(sw4)==LOW){
    if(led4_state == 0){
      digitalWrite(led4,HIGH);
      led4_state = 1;
    }
    else if(led4_state == 1){
      digitalWrite(led4,LOW);
      led4_state = 0;
    }
}
delay(1000);
}*/

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);
  
  
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

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
  //switch_led();
}
