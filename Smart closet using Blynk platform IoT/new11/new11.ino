#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

Servo pintu;
// variabel connection pins:
#define pirPin D8
#define ldrpin A0
#define VPIN0_button V0
#define VPIN1_button V1
#define VTN1_input V15
#define VTN2_input V16

int input1,input2;
int ledredPin = D3;
int ledgreenPin = D4;
int lock = D5;
int lamp = D6;
int tombol = D7;
int VTN1;
int VTN2;

int btn1_state = LOW;
int btn2_state = LOW;

// Create variables:
int val;  // PIR
int data; // LDR
int btn; // button
bool motionState = false; // We start with no motion detected.

// You should get Auth Token in the Blynk App.
char auth[] = "DzLBxfsZzjTzJbWi7JOhDYzRPbZ3vBB8";
//"as8qY_JLfNuDO0fuLSnWUlcTxUv_dbo4";

char ssid[] = "KelinciPercobaan";
char pass[] = "qwerty12";

//char ssid[] = "Vector3";
//char pass[] = "Ecourmc2";

BLYNK_CONNECTED() {

  // Request the latest state from the server

  Blynk.syncVirtual(VPIN0_button);
  Blynk.syncVirtual(VPIN1_button);

}

BLYNK_WRITE(VPIN0_button) {
  btn1_state = param.asInt();
  input1 = btn1_state;
}

BLYNK_WRITE(VPIN1_button) {
  btn2_state = param.asInt();
  input2 = btn2_state;
}

BLYNK_WRITE(VTN1_input) {
  VTN1 = param.asInt();
  //toilet1();
}

BLYNK_WRITE(VTN2_input) {
  VTN2 = param.asInt();
  //toilet2();
}

void toilet1(){
  int OTP1 = random(20);
  Serial.print("angka random");
  Serial.println(OTP1);
  Blynk.notify(OTP1);
  for(int delay1 = 40000; delay1 >= 0; delay1--){
      pintu.write(180);
      Blynk.virtualWrite(V3, 255);
      Blynk.virtualWrite(V2, 0);
      digitalWrite(lock, LOW);
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
      Blynk.virtualWrite(V4, 0);
      digitalWrite(lock, LOW);
      Blynk.virtualWrite(VPIN1_button, HIGH);
      if(OTP2 == VTN2){
        break;
    }
  }
   Blynk.virtualWrite(VPIN1_button, LOW);
   pintu.write(0);
}

void normal1() {
  // Read out the pirPin and store as val:
  val = digitalRead(pirPin);
  btn = digitalRead(tombol);
  // If motion is detected (pirPin = HIGH), do the following:
  if (val == 1) {
    digitalWrite(ledredPin, HIGH);
    digitalWrite(ledgreenPin, LOW);
    Blynk.virtualWrite(V3, 255);
    Blynk.virtualWrite(V2, 0);
    // Change the motion state to true (motion detected):
    if (motionState == false) {
      Serial.println("Ada Gerakan!");
      motionState = true;
      delay(5000);
      pintu.write(180);
      delay(1000);
      digitalWrite(lock, HIGH);
    }
     if(btn == 1){
       digitalWrite(ledredPin, LOW);
       digitalWrite(ledgreenPin, HIGH);  
       digitalWrite(lock, LOW);
       delay(1000);
       pintu.write(0);
     }
  }
  // If no motion is detected (pirPin = LOW), do the following:
  else {
    digitalWrite(ledredPin, LOW);
    digitalWrite(ledgreenPin, HIGH);
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V3, 0);
    // Change the motion state to false (no motion):
    if (motionState == true) {
      Serial.println("Tidak Ada Gerakan!");
      motionState = false;
      digitalWrite(lock, LOW);
      delay(1000);
      pintu.write(0);
    }
  }
}

void normal2(){
      Blynk.virtualWrite(V4, 255);
      Blynk.virtualWrite(V5, 0);
}

void lampu(){
  data = analogRead(ldrpin);
  //Serial.println(data);
  if(data < 500){
      digitalWrite(lamp, LOW);
  }else{
      digitalWrite(lamp,HIGH);    
  }
}

void setup() {
  // Configure the pins as input or output:
  Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  pinMode(ledredPin, OUTPUT);
  pinMode(ledgreenPin, OUTPUT);
  pinMode(lock, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ldrpin, INPUT);
  pinMode(tombol, INPUT);
  pintu.attach(4);
  // Debug console
  Serial.begin(9600);
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Wifi Connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  Serial.print(" ");
}

void loop(){
  lampu();
  
  if(input1 == HIGH){
    toilet1();
  }else{
    normal1();
  }
  
  if(input2 == HIGH){
    toilet2();
  }else{
      normal2();
  }
  Blynk.run();
  delay(1000);
}
