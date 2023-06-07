#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <Servo.h>

//ssid and pass
char ssid[] = "KelinciPercobaan";
char pass[] = "qwerty123";

//char ssid[] = "Vector3";
//char pass[] = "Ecourmc2";

// mqtt server di android
#define mqttserver "broker.hivemq.com"  //"i-ot.net"
#define mqttport 1883 // dengan port 1883
//#define mqttUser "fahmi"
//#define mqttPassword "zXaSqW1@"

WiFiClient EspClient1;
PubSubClient client (EspClient1);

// variabel connection pins:

//Servo pintu;
#define pirPin D8
#define ldrpin A0

int motion_aktif;
int ledredPin = D3;
int ledgreenPin = D4;
int ledwifi = D1;
//int lock = D5;
int buzzer = D5;
int lamp = D6;
int tombol = D7;

int lampauto = 0;
int datai = 0; //btnstate

void callback1(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; 1 < length; i++) {
    Serial.print((char)payload[i]);
  }

  //motion aktif
    if ((char)payload[0] == '1') {
    motion_aktif = 1; // motion auto on *toilet lock
    Serial.println("Motion Auto Aktif");
  } else if((char)payload[0] == '0') {
    motion_aktif = 0; // motion auto off *toilet unlock
    Serial.println("Motion Auto Mati");
  }

  // lampu manual
    if (((char)payload[0] == '5') && (lampauto == 0)) {
    digitalWrite(lamp, HIGH);
    Serial.println("lampu nyala");
  } else if(((char)payload[0] == '6')  && (lampauto == 0)) {
    digitalWrite(lamp, LOW);
    Serial.println("lampu mati");
  }
  
  //lampu auto
    if ((char)payload[0] == '7') {
    lampauto = 1;
    Serial.println("Lampu Auto Aktif");
  } else if((char)payload[0] == '8') {
    lampauto = 0;
    Serial.println("Lampu Auto Mati");
  }
}

void reconnected() {
  while (!client.connected()) {
    Serial.println("Menghubungkan kembali ke MQTT..");
    if (client.connect("client_kelinci")) {
      Serial.println("Sukses terhubung Kembali..");
    } else {
      Serial.print("Gagal,..");
      Serial.print(client.state());
      Serial.println("tunggu 3 detik..");
      delay(3000);
    }
  }
  client.subscribe("kontroller1/data");
}

void toilet1() {
  for (int delay1 = 40000; delay1 >= 0; delay1--) {
    pintu.write(180);
    digitalWrite(lock, LOW);
    digitalWrite(ledredPin, HIGH);
    digitalWrite(ledgreenPin, LOW);
    if (input1 == LOW) {
      datai = 0;
      break;
    }
  }
}

void button() {
  while (true) {
    int btn = digitalRead(tombol);
    Serial.print("BTN : ");
    Serial.println(btn);
    Serial.println(" ");
    digitalWrite(ledredPin, HIGH);
    digitalWrite(ledgreenPin, LOW);
    pintu.write(180);
    delay(2000);
    digitalWrite(lock, HIGH);
    if (btn == 1) {
      datai = 0;
      break;
    }
  }
}

void normal1() {
  char pir1[2];
  
  int val = digitalRead(pirPin);
  if (val == 0) {
    digitalWrite(ledredPin, HIGH);
    digitalWrite(ledgreenPin, LOW);
    Serial.println("Ada Gerakan!");
    pintu.write(180);
    delay(2000);
    digitalWrite(lock, HIGH);
    tone(buzzer, 300);
  }
  else {
    digitalWrite(ledredPin, LOW);
    digitalWrite(ledgreenPin, HIGH);
    Serial.println("Tidak Ada Gerakan!");
    delay(2000);
    pintu.write(0);
    digitalWrite(lock, LOW);
    tone(buzzer, 0);
  }
  dtostrf(val, 1, 0, pir1);
  dtostrf(notifku, 2, 0, notif);
  client.publish("kontroller1/pir", pir1);
}

void lampu() {
  char lampu1[4];
  char indi[2];
  int ledlampu;

  int datax = analogRead(ldrpin);
  //Serial.print("LDR : ");
  //Serial.println(datax);
  if (datax < 32) {
    digitalWrite(lamp, HIGH);
    ledlampu = 1;
  } else {
    digitalWrite(lamp, LOW);
    ledlampu = 0;
  }
  dtostrf(datax, 2, 0, lampu1);
  dtostrf(ledlampu, 1, 0, indi);
  client.publish("kontroller1/lede", indi);
  client.publish("kontroller1/ldr", lampu1);
}

void setup() {
  // Configure the pins as input or output:
  pinMode(ledredPin, OUTPUT);
  pinMode(ledgreenPin, OUTPUT);
  pinMode(ledwifi, OUTPUT);
  //pinMode(lock, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ldrpin, INPUT);
  pinMode(tombol, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pintu.attach(4);
  // Debug console
  Serial.begin(9600);

  wifi_mode();
}

void wifi_mode(){
  // Mengatur WiFi ----------------------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
    while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledwifi, LOW);
    Serial.print(".");
    delay(500);
    yield();
  }
  client.setServer(mqttserver, mqttport);
  client.setCallback(callback1);
  Serial.println(" ");
  Serial.println("Wifi Connected");
  digitalWrite(ledwifi, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  // Print the IP address ---------------------------------------------------
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  Serial.print(" ");
}

void loop() {
    if (lampauto == 1){
    lampu();
    }
    if(motion_aktif == HIGH){
    normal1();
    }
        if(input1 == HIGH){
          toilet1();
        }else {
          if(datai == 1){
              button();    
      }
    
    if (!client.connected()) {
      reconnected();
    }
    client.loop();
    Serial.println("System Online Mode");
  
delay(1000);
}
