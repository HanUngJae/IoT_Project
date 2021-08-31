Arduino UNO1 (DHT22온습도 센서, 열선패드)
Arduino UNO2 (LCD)
Wemos D1 R1 (mqtt brocker 통신)

UNO1 센서에서 온습도 측정 및 열선 패드 제어 

UNO1에서 UNO2로 온습도 데이터 전송 

UNO2에서 온습도 데이터 수신 및 LCD 출력

UNO2에서 Wemos로 온습도 데이터 전송

Wemos에서 온습도데이터 mqtt 송신 및 제어 값 수신

/** 아래는 미완성 **/

Wemos에서 UNO1으로 제어값 전송

UNO1에서 제어값에 따른 열선 패드 제어