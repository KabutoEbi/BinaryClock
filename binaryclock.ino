String inputTime = "";
int hours = 0;
int minutes = 0;
int seconds = 0;
unsigned long previousMillis = 0;
const long interval = 1000;  // 1秒ごとの更新間隔
int hourPins[] = {2, 3, 4, 5};  // 時を表示するピン
int minutePins[] = {6, 7, 8, 9, 10, 11};  // 分を表示するピン

void setup() {
  Serial.begin(9600);
  // ピンモードを出力に設定
  for (int i = 0; i < 4; i++) {
    pinMode(hourPins[i], OUTPUT);
  }
  for (int i = 0; i < 6; i++) {  // 分を表示するためのピンを6つに設定
    pinMode(minutePins[i], OUTPUT);
  }
}

void loop() {
  // シリアル入力がある場合、時刻データを読み取る
  if (Serial.available() > 0) {
    inputTime = Serial.readStringUntil('\n');
    if (parseTime(inputTime)) {
      // 時刻が正しく解析された場合、LEDに表示
      int displayHour = convertTo12HourFormat(hours);  // 12時間制に変換
      displayBinary(displayHour, hourPins, 4);  // 12時間制で表示
      displayBinary(minutes, minutePins, 6);  // 分を表示
    }
  }
  // 1秒ごとの更新処理
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    seconds++;
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }
    if (hours >= 24) {
      hours = 0;  // 24時間制としてリセット
    }
  }
}
// 入力された時刻を解析して設定する関数
bool parseTime(String timeString) {
  timeString.trim();  // 不要な空白を削除
  int firstSpace = timeString.indexOf(' ');
  int secondSpace = timeString.indexOf(' ', firstSpace + 1);
  if (firstSpace == -1 || secondSpace == -1) {
    return false;  // フォーマットが正しくない場合
  }
  String hourString = timeString.substring(0, firstSpace);
  String minuteString = timeString.substring(firstSpace + 1, secondSpace);
  String secondString = timeString.substring(secondSpace + 1);
  hours = hourString.toInt();
  minutes = minuteString.toInt();
  seconds = secondString.toInt();
  // 時間、分、秒の範囲をチェック（24時間制で入力）
  return (hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60 && seconds >= 0 && seconds < 60);
}
// 24時間制を12時間制に変換する関数
int convertTo12HourFormat(int hour24) {
  if (hour24 == 0) {
    return 12;  // 0時は12時として表示
  } else if (hour24 > 12) {
    return hour24 - 12;  // 13時以上は12を引いて12時間制にする
  } else {
    return hour24;  // 1～12時はそのまま表示
  }
}
// 2進数をLEDに表示する関数
void displayBinary(int value, int pins[], int length) {
  for (int i = 0; i < length; i++) {
    int bit = (value >> i) & 0x01;
    digitalWrite(pins[i], bit);  // ピンに反映
  }
}
