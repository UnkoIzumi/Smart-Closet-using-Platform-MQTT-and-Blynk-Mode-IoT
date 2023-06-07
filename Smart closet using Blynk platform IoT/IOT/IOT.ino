#define BLYNK_PRINT Serial    
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

Servo pintu;
// variabel connection pins:
#define pirPin 7
#define VPIN0_button V0
#define VPIN1_button V1
#define VTN1.input V15
#define VTN2.input V16

int input1,input2;
int ledredPin = 3;
int ledgreenPin = 4;
int lock = 5;
int lamp = 6;
int VTN1;
int VTN2;

int btn1_state = LOW;
int btn2_state = LOW;

int pushbtn1 = HIGH;
int pushbtn2 = HIGH;

// Create variables:
int val;
bool motionState = false; // We start with no motion detected.

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "DzLBxfsZzjTzJbWi7JOhDYzRPbZ3vBB8";
//"as8qY_JLfNuDO0fuLSnWUlcTxUv_dbo4";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "KelinciPercobaan";
char pass[] = "qwerty12";

BLYNK_CONNECTED() {

  // Request the latest state from the server

  Blynk.syncVirtual(VPIN0_button);
  Blynk.syncVirtual(VPIN1_button);

}

BLYNK_WRITE(VPIN0_button) {
  btn1_state = param.asInt();
  digitalWrite(input1, btn1_state);
}

BLYNK_WRITE(VPIN1_button) {
  btn2_state = param.asInt();
  digitalWrite(input2, btn2_state);
}

BLYNK_WRITE(VTN1.input) {
  VTN1 = param.asInt();
  //toilet1();
}

BLYNK_WRITE(VTN2.input) {
  VTN2 = param.asInt();
  //toilet2();
}

void setup() {
  // Configure the pins as input or output:
  Serial.begin (9600);
  wifi.begin(9600);
  pinMode(ledredPin, OUTPUT);
  pinMode(ledgreenPin, OUTPUT);
  pinMode(lock, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(lamp, OUTPUT);
  pintu.attach(9);
  Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
}

void toilet1(){
  int OTP1 = random(20);
  Blynk.notify(OTP1);
  for(int delay1 = 0; delay1 <= 40000; delay1++){
      pintu.write(180);
      Blynk.virtualWrite(V3, 255);
      digitalWrite(lock, LOW);
      digitalWrite(lamp, HIGH);
      Blynk.virtualWrite(VPIN0_button, HIGH);
      if(OTP1 == VTN1){
        break;
      }
    }
}

void toilet2(){
  int OTP2 = random(20);
  Blynk.notify(OTP2);
  for(int delay2 = 0; delay2 <= 40000; delay2++){
      pintu.write(180);
      Blynk.virtualWrite(V5, 255);
      digitalWrite(lock, LOW);
      digitalWrite(lamp, HIGH);
      Blynk.virtualWrite(VPIN0_button, HIGH);
      if(OTP2 == VTN2){
        break;
      }
    }
}

void normal() {
  // Read out the pirPin and store as val:
  val = digitalRead(pirPin);
  // If motion is detected (pirPin = HIGH), do the following:
  if (val == 1) {
    digitalWrite(ledredPin, HIGH);
    digitalWrite(ledgreenPin, LOW);
    Blynk.virtualWrite(V3, 255);
    // Change the motion state to true (motion detected):
    if (motionState == false) {
      Serial.println("Ada Gerakan!");
      motionState = true;
      delay(10000);
      pintu.write(180);
      digitalWrite(lock, LOW);
      digitalWrite(lamp, HIGH);
    }
    if(btn == 1){
      digitalWrite(lock, HIGH);
      digitalWrite(lamp,LOW); 
      pintu.write(0);
    }
  }
  // If no motion is detected (pirPin = LOW), do the following:
  else {
    digitalWrite(ledredPin, LOW);
    digitalWrite(ledgreenPin, HIGH);
    Blynk.virtualWrite(V2, 255);
    // Change the motion state to false (no motion):
    if (motionState == true) {
      Serial.println("Tidak Ada Gerakan!");
      motionState = false;
      pintu.write(0);
      digitalWrite(lock, HIGH);
      digitalWrite(lamp,LOW);
    }
  }
}

void loop(){

  if(input1 == HIGH){
    toilet1();
  }else if(input2 == HIGH){
    toilet2();
  }else{
    normal();
  }
  
  Blynk.run();
}
