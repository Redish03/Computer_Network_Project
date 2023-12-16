#include <WiFi.h>
#include <WebSocketsClient.h>
#define WIFI_SSID "AndroidHotspot1440"
#define WIFI_PASSWORD "84571440"

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch(type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      webSocket.sendTXT("Connected");
      break;
    case WStype_TEXT:
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

void connectWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
}

void webSocketSetting() {
  webSocket.begin("192.168.31.153", 81, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void setup() {
  // put your setup code here, to run once:
//  Connect WiFi that same WiFi with connect server
  Serial.begin(115200);
  connectWifi();
  webSocketSetting();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
}
