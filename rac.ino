// C++ code
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin

// with the arduino pin number it is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int upPin = A0;
int upBS;

const int dnPin = A1;
int dnBS;

const int moPin = A3;
int mode;

const int buzzerPin = A2;
bool ringing = false;
bool triggered = false;

const int IR1 = 10;
int IR1state;

const int leftF = 7;
const int leftR = 6;

const int rightF = 9;
const int rightR = 8;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long iniH = 17;
int iniM = 14;
int iniS = 0;

long time = iniH * 3600 + iniM * 60 + iniS;
long max_time = 86400;

long alarm_time = 61200 + 60*13;
long s10 = 0;

void setup()
{
   lcd.begin(16,2);
  
  pinMode(upPin, INPUT);
  pinMode(dnPin, INPUT);
  pinMode(moPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(7,OUTPUT);   //left motors   forward
  pinMode(6,OUTPUT);   //left motors reverse
  Serial.begin(9600); 
}

void print_time(){
  lcd.setCursor(0,0);
  int hour = time / 3600;
  if (hour < 10){
    lcd.print(0);
  }
  lcd.print(hour);
  lcd.print(":");
  
  int minute = (time % 3600) / 60;
  if (minute < 10){
    lcd.print(0);
  }
  lcd.print(minute);
  lcd.print(":");
  
  int second = time % 60;
  if (second < 10){
    lcd.print(0);
  }
  lcd.print(second);
  
  if (!triggered && alarm_time <= time && alarm_time - time >= -60){
    ringing = true;
    triggered = true;
    //tone(buzzerPin, 261); 
  }
  
  if (time - alarm_time > 61 || alarm_time - time > 61){
    triggered = false;
  }
}


void manageAlarmTime(int upBS, int dnBS){
  bool toChange = false;
  if (upBS == HIGH){
    alarm_time += 60;
    lcd.setCursor(0,1);
    lcd.print("Alarm at: ");
    toChange = true;
  }
  if (dnBS == HIGH){
    alarm_time -= 60;
    lcd.setCursor(0,1);
    lcd.print("Alarm at: ");
    toChange = true;
  }
  if (toChange){
    int hour = alarm_time / 3600;
  if (hour < 10){
    lcd.print(0);
  }
  lcd.print(hour);
  lcd.print(":");
  
  int minute = (alarm_time % 3600) / 60;
  if (minute < 10){
    lcd.print(0);
  }
  lcd.print(minute);
  
  }
}

void manageNormalTime(int upBS, int dnBS){
  bool toChange = false;
  if (upBS == HIGH){
    time += 60;
  }
  if (dnBS == HIGH){
    time -= 60;
  }
}

void loop(){
  delay(100);
  s10 += 1;
  upBS = digitalRead(upPin);
  dnBS = digitalRead(dnPin);
  mode = digitalRead(moPin);
  IR1state = digitalRead(IR1);

  if (IR1state == LOW){
    //tone(buzzerPin, 261);
    Serial.print("IR");
  }
  else if (!ringing){
    //noTone(buzzerPin);
  }

  
  if (upBS){
    Serial.print("UP");
  }
  
  if (dnBS){
    Serial.print("DN");
  }

  if (mode){
    Serial.print("MODE");
  }

  if (s10 % 10 == 0){
    time += 1;
    if (time >= max_time){
      time = 0;
      s10 = 0;
    }
    print_time();
  }
  
  
  if (ringing){
    tone(buzzerPin, 261);
    digitalWrite(leftF, HIGH);
    digitalWrite(rightF, HIGH);
    if (upBS == HIGH || dnBS == HIGH || mode == HIGH){
      ringing = false;
      noTone(buzzerPin);
      digitalWrite(leftF, LOW);
      digitalWrite(rightF, LOW);

    }
    return;
  }
  
  
  if (mode == LOW) {
      manageAlarmTime(upBS, dnBS);
  }
  else{
    manageNormalTime(upBS, dnBS);
  }  
}