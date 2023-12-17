#include <WiFi.h>
#include <WebSocketsClient.h>
#define WIFI_SSID "AndroidHotspot1440"
#define WIFI_PASSWORD "84571440"

// 클라이언트의 웹 서버 소켓을 생성한다.
WebSocketsClient webSocket;

// 건네 받은 데이터그램의 type에 따라서 할 행동들을 정의한다.
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch(type)
  {
    // 만약 DISCONNECTED가 도착했다면
    case WStype_DISCONNECTED:
      // Disconnected라는 메세지를 출력하고
      Serial.printf("[WSc] Disconnected\n");
      // switch 문 탈출
      break;
    case WStype_CONNECTED:
      // 만약 연결에 성공했다면 아래와 같은 메세지를 출력한다.
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      webSocket.sendTXT("Connected");
      break;
    case WStype_TEXT:
      // 연결에 성공한 상태에서, payload의 값을 출력한다. 즉, 서버로부터 받은 사용자가 활동하지 않은 시간을 보여준다.
      Serial.println((char*)payload);
      break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }
}

// 와이파이에 연결한다. 자세한 코드 내용은 Server코드 참고
void connectWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
}

// 웹소켓을 설정한다. begin 함수의 IP에는 서버의 IP와 포트 넘버를 제시한다.
void webSocketSetting() {
  webSocket.begin("192.168.18.99", 81, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

// setup 함수는 아두이노 내장 함수로, loop 함수 실행 전에 한번만 실행된다.
void setup() {
  // put your setup code here, to run once:
//  Connect WiFi that same WiFi with connect server
  Serial.begin(115200);
  connectWifi();
  webSocketSetting();
  delay(1000);
}

// loop 함수또한 아두이노 내장 함수로, 보드의 전원이 꺼질때까지 계속해서 반복하는 코드이다.
void loop() {
  // put your main code here, to run repeatedly:
  // webSocketEvent 함수를 반복 실행한다.
  webSocket.loop();
}
