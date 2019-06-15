#include <Keyboard.h>

#define KEY_F13  0xF0 // 0x68 + 0x88

int ledPin = 17;  // LED connected to digital pin 13
int powerInPin = 10;    // pushbutton connected to digital pin 7
int sensorInPin = 9;
int optoPin = 8;
int val = 0;      // variable to store the read value
int sens = 0;
int ACT=0;
int MSR=2;
int CYW=1;
int CYLIMIT=100;

void setup() {
  pinMode(ledPin, OUTPUT);  // sets the digital pin 13 as output
  pinMode(powerInPin, INPUT);    // sets the digital pin 7 as input
  pinMode(sensorInPin, INPUT);
  pinMode(optoPin, OUTPUT);
  Serial.begin(9600);
  ACT=0;
  MSR=2;
  CYW=0;
  CYLIMIT=200;
}

void loop() {
  sens = digitalRead(sensorInPin);
  val = digitalRead(powerInPin);   // read the input pin

  // just for show: output power to ledPin and sensor data to TXLED
  digitalWrite(ledPin, val);
  //Serial.println("Power status");
  //Serial.println(val);
  
  if(sens == HIGH){
    TXLED1;
    //Serial.println("Sensor in high state");
    //Serial.println(ACT);
  }else{
    TXLED0;
    //Serial.println("Sensor in low state");
    //Serial.println(ACT);
  }

  if ( sens == HIGH && ACT == 0) {
    USBDevice.wakeupHost();
    Keyboard.press( KEY_F13 );
    Keyboard.release( KEY_F13 );
    Serial.println("Detected rising edge");
    ACT=1;
    MSR = val;
  }

  if(ACT == 1  && CYW < CYLIMIT){
    if(MSR != val){ // we have a change in the power line: we are in sleep mode
      digitalWrite(optoPin, HIGH);
      delay(600);
      digitalWrite(optoPin, LOW);
      Serial.println("Pray you never see this: we are in sleep mode");
      ACT=0;
      CYW=1;
      MSR=2;
    }  else { 
      Serial.println("Increasing CYW:");
      CYW=CYW+1;
      Serial.println(CYW);
    }
  }

  if(ACT == 1 && CYW >= CYLIMIT){ // we have overwaited,the computer must be ON. Do nothing, just reset.
    Serial.println("The computer is ON. Resetting.");
    ACT=0;
    CYW=1;
    MSR=2;
  }
  

  delay(5);
}
