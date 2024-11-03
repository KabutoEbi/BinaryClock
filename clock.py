import serial
import time
from datetime import datetime

# Arduinoとのシリアル通信を初期化
arduino = serial.Serial('COM12', 9600)
time.sleep(2)  # Arduinoが起動するまで待機

while True:
    # 現在の時刻を取得
    now = datetime.now()
    current_time = now.strftime("%H %M %S")  # HH MM SSフォーマットに変換

    # 現在の時刻をArduinoに送信
    arduino.write(current_time.encode('utf-8') + b'\n')
    print(f"Sent Time: {current_time}")  # 送信した時刻を表示

    time.sleep(1)  # 1秒ごとに送信
