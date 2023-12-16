#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
#define WIFI_SSID "AndroidHotspot1440"
#define WIFI_PASSWORD "84571440"

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

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
  server.handleClient();
  webSocket.loop();

  // 전송 값 설정
  sprintf(buffer, "%ld", millis());
  // 웹 소켓으로 값 전송
  webSocket.broadcastTXT(buffer);
}
