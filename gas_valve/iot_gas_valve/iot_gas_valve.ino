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
#include <Servo.h>
// Update these with values suitable for your network.

const char* ssid = "olleh_WiFi_65EB";//wifi id
const char* password = "0000001774";//wifi pw
const char* mqtt_server = "piflask.iptime.org";
const char* device_id = "mqtt_gas";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

Servo servo;

int angle = 0;

int Servo_Pin = D2;
int Servo_B = D3;
int Servo_Led = D4;
int Timer_B = D5;

int gas_state = 0; // 밸브 상태
int timer_state = 0; // 타이머 상태
int timer_count = 0; // 설정시간(서버에서 받은 값)
int timer_left = 0; // 남은시간

int timer_set = 0; // 시작시간
int timer_now = 0; // 지금시간 (지금시간-시작시간>설정시간 == 종료)

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

  //String ss = (char)payload[5]+(char)payload[6]+(char)payload[7];

  String topic_a = topic;

  // Switch on the LED if an 1 was received as first character
  if(topic_a == "valve"){
    if ((char)payload[0] == '1') {
      servo.write(90);
      digitalWrite(Servo_Led,HIGH);
      gas_state = 1;
    }
    else if ((char)payload[0] == '0'){
      servo.write(0);
      digitalWrite(Servo_Led,LOW);
      gas_state = 0;
    }
    if ((char)payload[2] == '1') {
      timer_state = 1;
      
      if(length<7){ //length참조하여 수정할 것 마지막 ? 넣는대서 +1 
        timer_count = (char)payload[4]-'0';
      }
      else if(length<8){
        timer_count = ((char)payload[4]-'0')*10+((char)payload[5]-'0');
      }
      else if(length<9){
        timer_count = ((char)payload[4]-'0')*100+((char)payload[5]-'0')*10+((char)payload[6]-'0');
      }
    }
   
    else if ((char)payload[2] == '0'){
      
      timer_state = 0;
      timer_count = 0;
      timer_left = 0;
    }
  }
}


void timer_end(){
    timer_now = millis();
    int i = 30*60000;
    int j = timer_now-timer_set;
    timer_left = ((i-j)/60000)+1;
    timer_count = timer_left;
    if(j>i){
      servo.write(0);
      digitalWrite(Servo_Led,LOW);
      gas_state = 0;
      timer_state = 0;
      timer_count = 0;
      timer_set = 0;
      timer_now = 0;
      timer_left = 0;
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
      //client.publish("/shc/valve", "{\"valve\" : \"connect\"}");
      // ... and resubscribe
      client.subscribe("valve");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void valve_switch(){
  if(digitalRead(Servo_B)==LOW){// 버튼 제어
    if(gas_state == 0){
      digitalWrite(Servo_Led,HIGH);
      servo.write(90);
      gas_state = 1;
    }
    else if(gas_state == 1){
      digitalWrite(Servo_Led,LOW);
      servo.write(0);
      gas_state = 0;
      
      timer_state = 0;
      timer_count = 0;
      timer_left = 0;
    }
 }
 if(digitalRead(Timer_B)==LOW){// 버튼 제어
    if(timer_state == 0){
      timer_state = 2;  // 물리 제어시 2의 값
      timer_count = 30;
      timer_set = millis();
    }
    else if(timer_state == 1){
      timer_state = 0;
      timer_count = 0;
      timer_left = 0;
      
    }
    else if(timer_state == 2){
      timer_state = 0;
      timer_count = 0;
      timer_left = 0;
    }
 }
 delay(1000);
}

void setup() {
  servo.attach(Servo_Pin);
  pinMode(Servo_Led,OUTPUT);
  pinMode(Servo_B, INPUT_PULLUP);
  pinMode(Timer_B, INPUT_PULLUP);

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
  valve_switch();

  
  if(timer_state == 2){
    timer_end();
  }
  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    snprintf (msg, MSG_BUFFER_SIZE, "{\"valveState\" : %ld, \"valveTime\" : %ld}", gas_state,timer_state);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("/shc/valve", msg);
  }
}
