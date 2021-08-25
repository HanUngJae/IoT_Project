 #include <Wire.h>
 #include <WiFi.h>
 //#include <PubSubClient.h>

 #include <DHT.h>
 #include <LiquidCrystal_I2C.h>
 //#include <SoftwareSerial.h>
 //SoftwareSerial ESP_Serial(2,3); //RX, TX
 
 #define DHTTYPE DHT22
 #define DHTPIN 7

 int state = 0;
 //const char* mqtt_server = "broker.mqtt-dashboard.com";
 //const int mqtt_port = 1883;
 //const char* mqtt_user = "arduino";
 //const char* mqtt_password = "arduino";
 //WiFiClient espClient;
 //PubSubClient mqttClient(espClient);

 LiquidCrystal_I2C lcd(0x27,16,2);
 DHT dht(DHTPIN, DHTTYPE);
 
 void setup() {
  Serial.begin(9600);
/**
  ESP_Serial.begin(115200);
  
  setup_wifi();
  mqttClient.setServer(mqtt_server,1883);
  mqttClient.setCallback(callback);

  while(!mqttClient.connected()){
    Serial.println("Connecting to MQTT...");
    
    if (mqttClient.connect("esp8266mqtt", mqtt_user, mqtt_password )) {
      Serial.println("connected");
    }else{
      Serial.print("failed with state ");
      Serial.println(mqttClient.state());  //If you get state 5: mismatch in configuration
      delay(2000);
    }
  }
**/
  Serial.println("DHTxx test!");
  pinMode(13, OUTPUT);
  
  dht.begin(); //LCD 초기화
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Hello, LeeHJ");
  lcd.setCursor(2,1);
  lcd.print("HyeongJun Temp");
  delay(2000);
  lcd.clear();
}
/**
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
  Serial.println(message);
  Serial.println("-----------------------");
}
**/

/**
void setup_wifi(){
  ESP_Serial.println("AT");
  delay(1000);
  while(ESP_Serial.available()){
    Serial.write(ESP_Serial.read());
  }
  Serial.println("===== Ready");

  ESP_Serial.println("AT+CWMODE=1");
  delay(1500);

  while(ESP_Serial.available()){
    Serial.write(ESP_Serial.read());
  }

  ESP_Serial.println("AT+CWJAP=\"hotspot3883\",\"a1111111\"");

  while(!ESP_Serial.find("OK")){
    delay(300);
  }

  while(ESP_Serial.available()){
    Serial.write(ESP_Serial.read());
  }

  Serial.println("===== Connected!");

  ESP_Serial.println("AT+CIFSR");

  delay(1000);

  while(ESP_Serial.available()){
    Serial.println(ESP_Serial.read());
  }

  Serial.println();
  ESP_Serial.println("AT+CIPMUX=1");
  delay(1000);

  while (ESP_Serial.available()){
     Serial.write(ESP_Serial.read());
  }

  ESP_Serial.println("AT+CIPSERVER=1,80");
  delay(1000);

  while (ESP_Serial.available()){
    Serial.write(ESP_Serial.read());
  }
  
}
**/
void loop() {
  delay(2000);
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  if(isnan(humi) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hic = dht.computeHeatIndex(temp, humi, false);

  lcd.setCursor(0,0);
  lcd.print("Temp = ");
  lcd.print(temp);
  lcd.print(" ");
  lcd.print(char(223));
  lcd.print("C ");
  lcd.setCursor(0,1);
  lcd.print("Humi = ");
  lcd.print(humi);
  lcd.print(" % ");

  if(temp < 31) //히터 on
  {
    state = 1;
    Serial.write(state);
  }
  else //히터 off
  {
    state = 2;
    Serial.write(state);
  }
}
