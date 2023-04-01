
// gaz sensor
#include <Servo.h>
int esikDegeri = 600;    
int buzzerPin = 4; 
int deger=0; 
float g;


// led yakma
int data;
int aydinlatma = 7;

// zil calma
//int buzzerPin = 4;

// sicaklik nem
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0,1);
#include <DHT.h>
#define DHTpin 2
#define DHTTYPE DHT11
DHT dht(DHTpin,DHTTYPE);
float t;
float h;



// RFID kapi
#include <MFRC522.h>
#include <SPI.h>


int RST_PIN = 9;
int SS_PIN = 10;
int greenPin = 8;
int servoPin = 5;

Servo servoRFID;
MFRC522 rfid(SS_PIN, RST_PIN);

byte id[4] = 
 {236,
  230,
  211,
  51};

//stor perde
//#include <Servo.h>
int potpin = A1;
int val;
Servo servostor;

//pencere kontrol
Servo servopen;
int derece = 0;




void setup() {
  pinMode(buzzerPin, OUTPUT); 
  
  pinMode(aydinlatma,OUTPUT);
  
  Serial.begin(9600);

  mySerial.begin(9600);
  dht.begin();
  
  servoRFID.attach(5);
  servoRFID.write(0);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(greenPin, OUTPUT);
  
  servostor.attach(6);
  servopen.attach(3); 
  
}
void loop() {
 
 
 val = analogRead(potpin);
 val = map(val, 0, 1023, 0, 180);
 servostor.write(val);
 delay(15);

 h = dht.readHumidity();
 t = dht.readTemperature();
 mySerial.print(t,1);
 mySerial.print("|");
 mySerial.print(h,1);
  Serial.print(t);
  Serial.print("|");
  Serial.print(h);
  delay(1000);
  
  deger = analogRead(A0);
  if (deger > esikDegeri) { 
    //Serial.print("deger: ");
    servopen.write(90);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    } 
    else { 
    servopen.write(0);
    digitalWrite(buzzerPin, LOW);
    Serial.println(deger);
    delay(1);
  }

 if (Serial.available()){
   data = Serial.read();
   Serial.println(data);
   delay(100);
   
 if (data == '0'){
   
 for (int derece = 0; derece < 90; derece++)
 {
 servopen.write(derece);
 }
 }
 if (data == '1')
 {
 for (int derece = 90; derece > 1; derece--)
 {
 servopen.write(derece);
 }
 }
   if (data=='4'){
   digitalWrite(aydinlatma,HIGH);
   }
   if (data=='3') {
   digitalWrite(aydinlatma,LOW);
   }
   delay(100);
 if (data == '2')
 {
   digitalWrite(buzzerPin, HIGH);
   delay(1000);
 }
   digitalWrite(buzzerPin, LOW);
  
 }


 if (!rfid.PICC_IsNewCardPresent()) {
    return;

  }
  if (!rfid.PICC_ReadCardSerial()) {
    return;


  }
  if (rfid.uid.uidByte[0] == id[0] &&
      rfid.uid.uidByte[1] == id[1] &&
      rfid.uid.uidByte[2] == id[2] &&
      rfid.uid.uidByte[3] == id[3]) {
    
    digitalWrite(greenPin, HIGH);
    servoRFID.write(90);
    delay(3000);
    servoRFID.write(0);
    delay(1000);
    digitalWrite(greenPin, LOW);
     //return; 
   } 
  else {
    digitalWrite(greenPin, LOW);
    

  }
}
