#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu;
#include <LiquidCrystal.h>
const int RS = 7;
const int E  = 6;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
// Ultrasonic pins numbers
const int trigPin = A0;
const int echoPin = A1;
// defines variables
long duration;
int dCm;
int dcent;

// Choose two Arduino pins to use for software serial
int RXPin = 10;
int TXPin = 11;
int GPSBaud = 9600;
// Create a TinyGPS++ object
TinyGPSPlus gps;
// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);
String latlann;
String latlan = "";
// Key
int ek = A2;
int key= 0;
//Buzzer
int buzzer = 13;
int msg = 0;
int count = 0;
int motion = 0;
int incomingByte = 0;
//L293D
//Left Motor A
const int L1  = 12;  // Pin 14 of L293
const int L2  = A0;  // Pin 10 of L293
//Right Motor B
const int R1  = 8;  // Pin  7 of L293
const int R2  = 9;  // Pin  2 of L293
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
  pinMode(ek, INPUT);
  //L293D Output
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("  ROAD SURFACE ");
  lcd.setCursor(0,1);
  lcd.print("    ANALYSIS  ");
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  // Try to initialize!
  if (!mpu.begin()) {
    lcd.setCursor(0,1);
    lcd.print(" MPU6050 Failed   ");
    while (1) {
      delay(10);
    }
  }
  Serial.println("");
  delay(100);
  gpsSerial.begin(GPSBaud);
  delay(1500);
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
  lcd.clear();
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  delay(100);
}
void loop() {
  // put your main code here, to run repeatedly:
  // Ultrasonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  dCm= duration*0.034/2;
  if(dCm > 99){
    dcent = 99;
  }
  else{
    dcent = dCm;
  }
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  // Key
  key = analogRead(ek);
  key = key/110;
  count = 0;
  lcd.setCursor(0,0);
  lcd.print("D:");
  lcd.print(dcent);
  lcd.print(" ");
  lcd.setCursor(8,0);
  lcd.print("P:");
  lcd.print(key);
  lcd.print("  ");
  lcd.setCursor(0,1);
  lcd.print("X:");
  lcd.print(a.acceleration.x);
  lcd.print("  ");
  lcd.setCursor(8,1);
  lcd.print("Y:");
  lcd.print(a.acceleration.y);
  lcd.print("  ");
  if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    digitalWrite(buzzer, HIGH);
//    count++;
//    if(count > 3){
//      motion = 1;
//    }
//    else{
//      motion = 0;
//      count = 0;
//    }
//    digitalWrite(buzzer, LOW);
  }
  else{
    digitalWrite(buzzer, LOW);
  }
  if(dCm < 15)
  {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
  Serial.print(a.acceleration.x);
  Serial.print(" ");
  Serial.print(a.acceleration.y);
  //Serial.print(" ");
  //Serial.print(motion);
  Serial.print(" ");
  Serial.print(dcent);
  Serial.print(" ");
  Serial.print(key);
  Serial.print(" ");
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    //while(true);
  }
  //Serial.print(latlan);
  Serial.println();
  serialmotor();
  delay(700);
}
void displayInfo()
{
  if (gps.location.isValid())
  {
    //Serial.print("Latitude: ");
    //Serial.println(gps.location.lat(), 6);
    //latt = (gps.location.lat(),6);
    //Serial.print(" ");
    //Serial.println(gps.location.lng(), 6);
    //longi = (gps.location.lng(),6);
    //Serial.print("http://maps.google.com/?q=");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" ");
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  delay(100);
}
void serialmotor()
{
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
  }
  switch(incomingByte)
  {
    case '1': 
    digitalWrite(L1, LOW); // control for stop
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    //Serial.println("Stop\n");
    incomingByte='*';
    break;
    case '2': 
    digitalWrite(L1, HIGH); // control for right
    digitalWrite(L2, LOW);
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
    //Serial.println("Back\n");
    incomingByte='*';
    break;
    case '3': 
    digitalWrite(L1, LOW); // control for left
    digitalWrite(L2, HIGH);
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
    //Serial.println("Forward\n");
    incomingByte='*';
    break;
    case '4': 
    digitalWrite(L1, HIGH); // control for forward
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
    //Serial.println("Left\n");
    incomingByte='*';
    break;
    case '5': 
    digitalWrite(L1, LOW); // control for backward
    digitalWrite(L2, HIGH);
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
    //Serial.println("Right\n");
    incomingByte='*';
    break;
  }
}
