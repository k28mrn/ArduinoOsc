#include <ArduinoOSC.h>

//WiFiルータ Settings
const char* ssid = "ssid";
const char* pwd = "xxxxxxxxx";

const IPAddress gateway(192, 168, 77, 1);
const IPAddress subnet(255, 255, 255, 0);

//送受信ポート
const int port = 3333;

//送信先IP
const char* host = "192.168.77.11";

bool isConnecting = false;
int testCount = 0;

void setup() {
  Serial.begin(115200);
  

  isConnecting = true;

  // 自分のIPを決定する
  IPAddress ip(192, 168, 77, 120);

  //初期設定
  WiFi.disconnect(true, true); 
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);

  int cnt = 0;
  bool isConnected = true;
  delay(1000);
  
  Serial.print("INFO : Wi-Fi Start Connect.");
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print(".");
    delay(500);
    if (cnt == 5) {
      isConnected = false;
      break;
    }
    cnt += 1;
  }
  
  Serial.println("");
    
  if (isConnected) {
    Serial.println("INFO : Wi-Fi Connected.");
    //受信のリスナー設定
    OscWiFi.subscribe(port, "/app/status", onOscReceivedStatus);
  } else {
    Serial.println("INFO : Wi-Fi Connect Failed.");
  }

  //コネクト処理終了フラグ
  isConnecting = false;
}

/**
 * 受信 : デバイスステータス
 */
void onOscReceivedStatus(OscMessage& m) {
  int statusVal = m.arg<int>(0);
  Serial.print("onOscReceivedStatus : ");
  Serial.println(statusVal);
  //device.setStatus(statusVal);
}


void loop() {
  OscWiFi.update();

  /**
   * OSC送信 テスト
   */
  if (testCount == 20000) {
    OscWiFi.send(host, port, "/app/status", 0);
    testCount = 0;
    Serial.println("Send OSC: /app/status : 0");
  }
  testCount += 1;
}
