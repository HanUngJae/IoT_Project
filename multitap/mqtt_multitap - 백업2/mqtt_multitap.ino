#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "HUJ";
const char* password = "159753asd";
const char* mqtt_server = "192.168.0.80";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
float distance = 0;

int trigPin = D4; // 초음파센서 출력핀
int echoPin = D5; // 초음파센서 입력핀
int relay = D3; // 릴레이모듈 출력핀

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

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
  
  if(topic_a.equals("multitab")){
    if ((char)payload[0] == '1') { //on 입력했을 경우
        digitalWrite(relay, HIGH);
        Serial.println("1.1");
      } else if((char)payload[0] == '0') { //off 입력했을 경우
        digitalWrite(relay, LOW);
        Serial.println("1.0");
      } else if((char)payload[0] == '2') { //auto 입력했을 경우
        Serial.println("1.2");
        while(1){
            digitalWrite(trigPin, HIGH);
            delay(10);
            digitalWrite(trigPin, LOW);
            float duration = pulseIn(echoPin, HIGH); //echoPin이 HIGH를 유지한 시간(초음파가 보냈다가 다시 들어온 시간)
            distance = ((float)(340 * duration) / 10000) / 2; // 물체와의 거리

            Serial.print(distance);
            Serial.println("cm");          

            if(distance <= 30) { //30cm내에 물체가 있을경우
              digitalWrite(relay, HIGH); // 릴레이 on
            }
            else{
              digitalWrite(relay, LOW); // 없을경우 릴레이 off
            }
            delay(200);

            if((char)payload[0] == '0'){ //off를 입력했을경우
              break; //거리로 멀티탭 조정을 멈춤
            }
          }  
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
      Serial.println(client.connect(clientId.c_str()));
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("multitap");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);    
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
    root["multitab"] = distance;
    root.printTo(msg);
    //snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("/shc/multi", msg);
    client.subscribe("multitab");
  }
}
