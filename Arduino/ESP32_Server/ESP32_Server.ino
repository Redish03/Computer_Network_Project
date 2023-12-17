#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
// 와이파이 SSID와 비밀번호, 개발자의 모바일 핫스팟을 사용했다.
#define WIFI_SSID "AndroidHotspot1440"
#define WIFI_PASSWORD "84571440"
// LED와 버튼 제어를 위한 PIN 지정 
#define BUTTON_PIN 18
#define LED_PIN 21

// 웹 서버 객체 생성
WebServer server(80);
// 소켓 생성
WebSocketsServer webSocket = WebSocketsServer(81);

int button_state = 0;
int led_state = LOW;
unsigned long unused_time = 0;
unsigned long prevMillis = 0;
unsigned long curMillis = 0;

// 전송할 HTML 파일 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<div id="value_1">0</div>
<script>
websock = new WebSocket('ws://'+window.location.hostname+':81/');
websock.onmessage=function(evt)
{
  document.getElementById("value_1").innerHTML = evt.data;
}
</script>
</html>
)rawliteral";

// 서버에 index_html 파일 전송
void handle_root(){
  server.send(200, "text/html", index_html);
}

// ESP32 모듈에 연결할 LED와 Switch 핀을 지정을 해준다.
void pin_setting(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

// 와이파이에 연결한다. 
void connectWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // 와이파이가 연결될때까지 0.3초마다 .을 시리얼모니터에 찍는다.
  while(WiFi.status()!= WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  // IP 주소를 시리얼 모니터에 보여준다.
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// 서버를 열고, 웹소켓도 연다.
void webServerSocketSetting(){
  server.on("/", handle_root);

  server.begin();
  webSocket.begin();
}

// setup 함수는 아두이노 내장 함수로, loop 함수 실행 전에 한번만 실행된다.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pin_setting();
  connectWifi();
  webServerSocketSetting();
}
// 전송하기 전 버퍼 생성
char buffer[100];

// loop 함수또한 아두이노 내장 함수로, 보드의 전원이 꺼질때까지 계속해서 반복하는 코드이다.
void loop() {
  // put your main code here, to run repeatedly:
  // 버튼의 상태를 읽어온다. 즉, 누른 상태인지 아닌지 확인한다.
  button_state = digitalRead(BUTTON_PIN);

  // 버튼이 눌린 상태라면
  if(button_state == LOW) {
    // LED를 켠다.
    digitalWrite(LED_PIN, HIGH);
    // prevMillis는 버튼이 마지막으로 눌린 시간을 기록한다.
    prevMillis = millis();
  }
  // 버튼이 눌리지 않은 상태라면
  else {
    // LED를 끈다.
    digitalWrite(LED_PIN, LOW);
  }
  // 현재 초를 저장한다.
  curMillis = millis();
  // 현재 초와 마지막으로 버튼을 누른 시간을 빼서 사용자가 사용하지 않은 시간을 구한다. 
  unused_time = curMillis - prevMillis;

  // 서버가 Client와 연결을 활성화 한다.
  server.handleClient();  
  // 서버 웹소켓이 작동한다.
  webSocket.loop();

  // 전송 값 설정
  sprintf(buffer, "%ld", unused_time/1000);
  // 웹 소켓으로 값 전송
  webSocket.broadcastTXT(buffer);
}
