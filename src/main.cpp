#include <Arduino.h>
#include "const.cpp"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);     // SDA - A4 SCL - A5

int speedInt, valInt, speedALed_, valALed_;


void fillLed(int led_, int led_max){
    Serial.println("-----------------");
    Serial.print("Led: ");
    Serial.println(led_);
  for(int i = FIRST_PIN; i < FIRST_PIN + led_max; i++){
    if(i < FIRST_PIN + led_){
      digitalWrite(i, WORK_STATE);
      Serial.print(i);
      Serial.println(" to HIGH");

    }
    else{
      digitalWrite(i, !WORK_STATE);
      Serial.print(i);
      Serial.println(" to LOW"); // S60
    }
  }
  Serial.println("-----------------");
}


void setup(){
  lcd.begin();
  lcd.backlight();
  lcd.print("Start!");
  Serial.begin(9600);
  Serial.setTimeout(100);

  if(LED_IN_SPEED >= LED_IN_TAX){
    for(int i = FIRST_PIN; i <= LED_IN_SPEED + FIRST_PIN - 1; i++){
      pinMode(i, OUTPUT);
      if(DEV_MOD){
        Serial.print(i);
        Serial.println("Set to OUTPUT");
      }
    }
  }
  else{
    for(int i = FIRST_PIN; i <= LED_IN_TAX + FIRST_PIN - 1; i++){
      pinMode(i, OUTPUT);
      if(DEV_MOD){
        Serial.print(i);
        Serial.println("Set to OUTPUT");
      }
    }
  }

  pinMode(PIN_SPD, OUTPUT);
  pinMode(PIN_TAX, OUTPUT);
  if(DEV_MOD){
    Serial.println("-----------------");
  }


}

void showLed(int sl, int vl){
  if(sl && vl){   // обороты  > 0 && скорость > 0

  }
  else if(sl){  // скорость > 0

  }
  else if(vl){  // обороты > 0

  }
  else{   // обороты  = 0 && скорость = 0

  }

}

void parsSerialStr() {

  char currentChar, currentMeta;
  String inStr = Serial.readString();
  String speedStr, valStr;
  for(unsigned int i = 0; i < inStr.length(); i++){
    currentChar = inStr.charAt(i);  // current char
    if(currentChar >= 48 && currentChar <= 57){
      if (currentMeta == 'S') {
        speedStr += currentChar;
      }
      else if (currentMeta == 'V') {
        valStr += currentChar;
      }
    }
    else{
      currentMeta = currentChar;
    }
  }

  speedInt = constrain(speedStr.toInt(), 0, MAX_SPEED);
  valInt = constrain(valStr.toInt(), 0, MAX_VAL);

  valALed_ = constrain(map(valInt, 0, MAX_VAL, 0, LED_IN_TAX), 0, LED_IN_TAX);
  speedALed_ = constrain(map(speedInt, 0, MAX_SPEED, 0, LED_IN_SPEED), 0, LED_IN_SPEED);

  if (speedInt != 0 && speedALed_ == 0) {
    speedALed_ = 1;
  }
  if (valInt != 0 && valALed_ == 0){
    valALed_ = 1;
  }

}

void showToIC(int speed, int val) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.print(speed);
  lcd.setCursor(0, 1);
  lcd.print("tax: ");
  lcd.print(val);
}

void loop() {
  if(Serial.available()){
    parsSerialStr();
    showToIC(speedInt, valInt);
    fillLed(speedALed_, 10);
  }
  else{
    showLed(speedALed_, valALed_);
  }
}
// S200V4000
