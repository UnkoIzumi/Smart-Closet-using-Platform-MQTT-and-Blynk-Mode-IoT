#define LDR A0

void setup() {
  //setingan default 12bit
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LDR, INPUT);
}

void loop() {
  int data = analogRead(LDR);
  Serial.print("LDR : ");
  Serial.println(data);

  if(data < 500){
    digitalWrite(LED_BUILTIN, LOW);
  }else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
 delay(1000);
}
