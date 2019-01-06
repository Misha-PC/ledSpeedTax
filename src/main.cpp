#include <Arduino.h>
#include "const.cpp"

int speedInt, valInt;
int speedALed_, valALed_;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  if(LED_IN_SPEED >= LED_IN_TAX){
    for(int i = FIRST_PIN; i <= LED_IN_SPEED; i++){
      pinMode(i, OUTPUT);
    }
  }
  else{
    for(int i = FIRST_PIN; i <= LED_IN_TAX; i++){
      pinMode(i, OUTPUT);
    }
  }

  pinMode(PIN_SPD, OUTPUT);
  pinMode(PIN_TAX, OUTPUT);
} 

void parsSerial(){
      String speedStr, valStr;
      char currentChar;
      int currentMeta = 3;

      while (Serial.available()) {
        currentChar = Serial.read();
        if(currentChar == 'V'){
          currentMeta = 0;
        }
        else if(currentChar == 'S'){
          currentMeta = 1;
        }
        else{
          if(currentMeta){
              speedStr += currentChar;
          }
          else{
            valStr += currentChar;
          }
        }
      }

      valInt = valStr.toInt();
      speedInt = speedStr.toInt();

      speedALed_ = map(speedInt, 0, MAX_SPEED, 0, LED_IN_SPEED);
      valALed_ = map(valInt, 0, MAX_VAL, 0, LED_IN_TAX);
}

void showVal(int speed, int val) {
  delay(UPDATE_DELAY);

  digitalWrite(PIN_TAX, HIGH);
  digitalWrite(PIN_SPD, LOW);

  for(int i = FIRST_PIN; i <= LED_IN_SPEED; i++){
    if(i <= speed){
      digitalWrite(i, HIGH);
    } else{
      digitalWrite(i, LOW);
    }
  }
  delay(UPDATE_DELAY);

  digitalWrite(PIN_TAX, LOW);
  digitalWrite(PIN_SPD, HIGH);

  for(int i = FIRST_PIN; i <= LED_IN_TAX; i++){
    if(i <= val){
      digitalWrite(i, HIGH);
    } else{
      digitalWrite(i, LOW);
    }
  }
}

void loop() {
  if(Serial.available()){
    parsSerial();
  }
  else{
    showVal(speedALed_, valALed_);
  }
}
