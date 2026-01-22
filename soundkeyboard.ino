//=============================
// ヘッダーファイル読み込み
//=============================
#define TONE_USE_INT
#define TONE_PITCH 440
#include <WiFiS3.h>          // UNO R4 WiFi用
#include <TonePitch.h>
#include "config/config.h"   // SSID, PASS, NOTE定義など
#include <ArduinoOSCWiFi.h>  // ★ OSC用

//=============================
// ピン設定
//=============================
#define SW_C4 2   // ド
#define SW_D4 3   // レ
#define SW_E4 4   // ミ
#define SW_F4 5   // ファ
#define SW_G4 6   // ソ
#define BUZZ_PIN 10 // ブザーのピン

int status = WL_IDLE_STATUS;

//=============================
// OSC / 通信設定
//=============================

// ★ 送り先PCのIPアドレス（自分のIPv4に合わせて書き換え済みの想定）
const char* OSC_HOST = "192.168.0.119";
// ★ Max側の受信ポート
const uint16_t OSC_PORT = 5000;

//=============================
// セットアップ
//=============================
void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  pinMode(SW_C4, INPUT_PULLUP);
  pinMode(SW_D4, INPUT_PULLUP);
  pinMode(SW_E4, INPUT_PULLUP);
  pinMode(SW_F4, INPUT_PULLUP);
  pinMode(SW_G4, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("WiFi接続テスト開始");

  // Wi-Fiモジュール確認
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("⚠️ WiFiモジュールが見つかりません");
    while (true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
  }

  // Wi-Fi接続
  while (status != WL_CONNECTED) {
    Serial.print("接続中: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(5000);
  }

  Serial.println("✅ WiFi接続成功");
  Serial.print("ArduinoのIPアドレス: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

//=============================
// メインループ
//=============================
void loop() {
  if (digitalRead(SW_C4) == LOW) {          // ド
    buzzSound(NOTE_C4);
    sendNoteToPC("C4");
  } else if (digitalRead(SW_D4) == LOW) {   // レ
    buzzSound(NOTE_D4);
    sendNoteToPC("D4");
  } else if (digitalRead(SW_E4) == LOW) {   // ミ
    buzzSound(NOTE_E4);
    sendNoteToPC("E4");
  } else if (digitalRead(SW_F4) == LOW) {   // ファ
    buzzSound(NOTE_F4);
    sendNoteToPC("F4");
  } else if (digitalRead(SW_G4) == LOW) {   // ソ
    buzzSound(NOTE_G4);
    sendNoteToPC("G4");
  } else {
    noTone(BUZZ_PIN);   // 何も押されてないときは消音
  }

  // ★ OSCの受信/Publishをする場合の更新処理（送信だけでも呼んでおいてOK）
  OscWiFi.update();
}

//=============================
// 音を鳴らす関数
//=============================
void buzzSound(int frequency) {
  tone(BUZZ_PIN, frequency);
  delay(50);
}

//=============================
// PCにOSCで通知を送る関数
//=============================
void sendNoteToPC(const char* note) {
  // /note アドレスで 「note」という文字列1つを送る
  // 例: /note "C4"
  OscWiFi.send(OSC_HOST, OSC_PORT, "/note", note);

  Serial.print("OSC送信: /note ");
  Serial.println(note);
}
