#include <M5Stack.h>

void setup() {
  M5.begin();  
  M5.Lcd.setTextSize(2);
}

void loop() {
  M5.update();

  // カーソル初期化
  M5.Lcd.setCursor(0, 0);

  // 現在押されているか？
  M5.Lcd.print("BtnA.isPressed():");
  M5.Lcd.println( M5.BtnA.isPressed() );

  // 現在離しているか？
  M5.Lcd.print("BtnA.isReleased():");
  M5.Lcd.println( M5.BtnA.isReleased() );

  //（1度だけ取得可能）
  if ( M5.BtnA.wasPressed() ) {
    Serial.println("BtnA.wasPressed() == TRUE");
  }
  //Serial.println("loop");

  delay(300);
}