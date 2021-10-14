 #include <Wire.h>
 //#include <PubSubClient.h>
 #include <SoftwareSerial.h>
 #include <DHT.h>
 
 #define DHTTYPE DHT22
 #define DHTPIN 7
 #define RELAY 6
 #define TX 2
 #define RX 3

 int state = 0;
 int setTemp = 10;
 String subscribe_data;
 String bus;


 unsigned long lastMsg = 0;
 
 DHT dht(DHTPIN, DHTTYPE);
 
 void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  pinMode(RELAY, OUTPUT);
  pinMode(13, OUTPUT);
  
  dht.begin(); //LCD 초기화
}

void loop() {
  delay(2000);

  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  if(Serial.available())
  {
    subscribe_data = Serial.readString();
    state = subscribe_data.substring(0,1).toInt();
    setTemp = subscribe_data.substring(2,4).toInt();
  }

  String strTemp = String(temp) + String(humi) + String(setTemp);

  if(isnan(humi) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //float hic = dht.computeHeatIndex(temp, humi, false);

  Serial.write((char*)strTemp.c_str());
  Serial.println("");

  if(state == 1){
    if(temp < setTemp) //히터 on
    {
      digitalWrite(RELAY,HIGH);
    }
    else //히터 off
    {
      digitalWrite(RELAY,LOW);
    }
  }
  else {
    digitalWrite(RELAY, LOW);
  }

  
}
