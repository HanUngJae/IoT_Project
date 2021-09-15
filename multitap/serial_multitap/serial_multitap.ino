int trigPin = D4; // 초음파센서 출력핀
int echoPin = D5; // 초음파센서 입력핀
int relay = D3; // 릴레이모듈 출력핀

void setup() {  
  pinMode(relay, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  
  if(Serial.available())
  {
    byte data = Serial.read();
    
    if(data == '1'){ //on 입력했을 경우
      digitalWrite(relay, HIGH);
    }
    else if(data == '0'){ //off 입력했을 경우
      digitalWrite(relay, LOW);
    }
    else if(data == '2'){ //auto 입력했을 경우
      while(1){
        digitalWrite(trigPin, HIGH);
        delay(10);
        digitalWrite(trigPin, LOW);
        float duration = pulseIn(echoPin, HIGH); //echoPin이 HIGH를 유지한 시간(초음파가 보냈다가 다시 들어온 시간)
        float distance = ((float)(340 * duration) / 10000) / 2; // 물체와의 거리

        Serial.print(distance);
        Serial.println("cm");

        if(distance <=30) { //30cm내에 물체가 있을경우
          digitalWrite(relay, HIGH); // 릴레이 on
        }
        else{
          digitalWrite(relay, LOW); // 없을경우 릴레이 off
        }
        delay(200);

        if(Serial.read() == '0'){ //off를 입력했을경우
          break; //초음파센서로 멀티탭 조정을 멈춤
        }
      }
    }
  }
}
