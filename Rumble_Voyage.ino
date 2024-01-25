#include <string.h>

struct MessageValue
{
  String message;
  String value;
};

struct MessageValue getMessage(String inputtedStr) 
{
  struct MessageValue result;

  char charArr[50];
  inputtedStr.toCharArray(charArr, 50);
  char* ptr = strtok(charArr, "||");
  result.message = String(ptr);
  ptr = strtok(NULL, "||");

  if (ptr == NULL) {
    result.value = String("");
    return result;
  }

  result.value = String(ptr);

  return result; 
}

// RGB LED
const int redPin1 = 9;
const int redPin2 = 10;
const int redPin3 = 11;

// const int redPin = 9;
// const int greenPin = 10;
// const int bluePin = 11;

// Touch
const int touchPin = 7;
// Sound
const int soundPin = A0;

int touchState = 0;
int soundState = 0;
boolean change = false;

// WTACHME: need to test the threshold
int soundthreshold = 0;

int HPnum = 3;
int oldSound = 0; 

struct MessageValue receivedData;

void setup() {
  
  // 3 * LED
  pinMode(redPin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(redPin3, OUTPUT);
  // Touch
  pinMode(touchPin, INPUT_PULLUP);
  // Sound
  pinMode(soundPin, INPUT);

  Serial.begin(9600);
  Serial.setTimeout(20);

}

void loop() {

  // 常态灯
  if(HPnum == 3)
  {
    analogWrite(redPin1, 255);
    analogWrite(redPin2, 255);
    analogWrite(redPin3, 255);
  }
  if(HPnum == 2)
  {
    analogWrite(redPin1, 255);
    analogWrite(redPin2, 255);
    analogWrite(redPin3, 0);
  }
  if(HPnum == 1)
  {
    analogWrite(redPin1, 255);
    analogWrite(redPin2, 0);
    analogWrite(redPin3, 0);
  }
  if(HPnum == 0)
  {
    analogWrite(redPin1, 0);
    analogWrite(redPin2, 0);
    analogWrite(redPin3, 0);
  }

  // // SOUND
  // int soundValue = analogRead(soundPin);
  // if (soundValue > soundthreshold) 
  // {
  //   soundState = soundValue;
  //   delay(200);
  // }else
  // {
  //   soundState = 0;
  // }
  // // A-P 声音1/0传给Protopie
  // String soundmessage = "SOUNDNUM||" + String(soundState);
  // Serial.println(soundmessage); // sent soundmessage to protopie

  // SOUND
  int soundValue = analogRead(soundPin);
  
  if (soundthreshold == 0)
  {
    soundthreshold = soundValue + 2;
  }

  if (soundValue <=soundthreshold)
  {
    // idle状态
    soundState = 0;
  }else
  {
    soundState = soundValue;
  }
  String soundmessage = "SOUNDNUM||" + String(soundState);
  // delay(50);
  Serial.println(soundmessage); // sent soundmessage to protopie



  // // 原件test
  // int soundValue = analogRead(soundPin);
  // delay(20);
  // String soundmessage = "SOUNDNUM||" + String(soundValue);
  // Serial.println(soundmessage); // sent soundmessage to protopie

  // TOUCH
  touchState = digitalRead(touchPin);
  // 如果触摸
  if (touchState == HIGH)
  {
    touchState = 0;
  }else // 没有触摸
  {
    touchState = 1;
    delay(500);
  }
  String touchmessage = "TOUCHSTATE||" + String(touchState);
  // 发送给Protopie触摸信息
  // 1：按下button 0: 没有按下
  Serial.println(touchmessage); 


  // 读数据 
  while (Serial.available() > 0)
  {
    String receivedString = Serial.readStringUntil('\0');
    receivedData = getMessage(receivedString);
  }
  // 根据血量信息调整灯
  if (receivedData.message.equals("HPNUM"))
  {
    HPnum = receivedData.value.toInt();
  }

}
