#include <SoftwareSerial.h>

#define verde 6
#define vermelho 3
#define sensorPIR 11
#define buzzer 2
int PIRval = 0;

SoftwareSerial s(0, 1);

void setup() {
  pinMode(sensorPIR, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  s.begin(9600);
}

void loop(){
//  digitalWrite(buzzer,0);
  if(digitalRead(5)==1)
  {
    Serial.println(s.read());
  }
  PIRval = digitalRead(sensorPIR);
  Serial.println(PIRval);
  digitalWrite(buzzer, HIGH);
  if (PIRval == 1)
  {
      digitalWrite(5, HIGH);
      digitalWrite(buzzer,LOW);
  }else{
    digitalWrite(buzzer, HIGH); 
  }
}
