#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
#define WIFI_SSID "AndroidHotspot1440"
#define WIFI_PASSWORD "84571440"
#define BUTTON_PIN 18
#define LED_PIN 21

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

int button_state = 0;
int led_state = LOW;
unsigned long unused_time = 0;
unsigned long prevMillis = 0;
unsigned long curMillis = 0;

// 전송 HTML
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

void handle_root(){
  server.send(200, "text/html", index_html);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  unsigned long ms = millis();


  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status()!= WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);

  server.begin();
  webSocket.begin();
  
}

char buffer[100];
void loop() {
  // put your main code here, to run repeatedly:
  button_state = digitalRead(BUTTON_PIN);
  if(button_state == LOW) {
    digitalWrite(LED_PIN, HIGH);
    prevMillis = millis();
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
  curMillis = millis();
  unused_time = curMillis - prevMillis;
  
  server.handleClient();  
  webSocket.loop();

  // 전송 값 설정
  sprintf(buffer, "%ld", unused_time/1000);
  // 웹 소켓으로 값 전송
  webSocket.broadcastTXT(buffer);
}
