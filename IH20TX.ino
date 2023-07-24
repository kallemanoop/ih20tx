#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4   // Change this to the GPIO pin number you have connected to the DS18B20 data line
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);



int flag_c;
float duration;  
float distance;
int flag=32;
int coolant=34;

const int trigPin = 12; // define the trigger pin
const int echoPin = 14; // define the echo pin

#include <WiFi.h> //Blynk
#include <BlynkSimpleEsp32.h> //Blynk
char auth[] = "5puqqtNHehC-v-eVC8hYmqAaCZqvKiD5"; //Blynk
char ssid[]= "Abhiram"; //Blynk
char pass[] = "Abhiram1709"; //Blynk


void setup() {
  Blynk.begin (auth, ssid, pass, "blynk.cloud", 80); //Blynk
  Serial.begin(9600); // initialize serial communication
  sensors.begin();
  pinMode(trigPin, OUTPUT); // set the trigger pin as output
  pinMode(echoPin, INPUT);// set the echo pin as input
  //pinMode(x,OUTPUT);
  pinMode (4, OUTPUT); //Blynk

}

void loop() {
  digitalWrite(trigPin, LOW); // set the trigger pin to low
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH); // set the trigger pin to high
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration=pulseIn(echoPin,HIGH);
  duration=duration/1000;
  distance=(duration*0.034/2)*1000; //0.034 is the conversion constant
  sensors.requestTemperatures();  // Request temperature reading from the DS18B20 sensor
  float tempC = sensors.getTempCByIndex(0);
  Blynk.virtualWrite(V1, tempC);


  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");
  
  Serial.print("Time taken : ");
  Serial.print(duration);
  Serial.print(" ms");
  Serial.println(" ");

  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.println(" ");
  delay(2000);// wait for a second before taking the next reading
  if(duration>0.61){
    flag=1;
    Serial.println("Pump is turned on");
    Serial.println(" "); 
    Blynk.virtualWrite(V2, "ON");
    Blynk.setProperty(V2, "color", "#00ff00");
    Blynk.virtualWrite(V3, "LOW");
       delay(2000);
    }
  else if(duration>0.45 && duration<0.61)
 {
  //digitalWrite(LED,HIGH);
  //flag=1;
  Serial.println("Water level is below 50%");
  Serial.println(" ");
  Blynk.virtualWrite(V3, "Below 50%");
   delay(2000);
 }
  else if(duration<0.22)
  {
    //digitalWrite(LED,LOW);
    Serial.println("Water level is MAXIMUM");
    Serial.println(" ");
    
    flag=0;
    Blynk.virtualWrite(V3, "MAXIMUM");
    Blynk.virtualWrite(V2, "OFF");
    Blynk.setProperty(V2, "color", "#000000");
    delay(2000);
  }
    else if(duration<0.45 && duration>0.22){
      Serial.println("Water level is above 50%");
      Serial.println(" ");
      
      Blynk.virtualWrite(V3, "Above 50%");
      delay(2000);
      }
  
 if((flag==0))
 {
  Serial.println("Water level is optimal and the pump is off");
  Serial.println(" ");
  delay(2000);
  digitalWrite(flag,HIGH); 
  delay(2000);               
  digitalWrite(flag,LOW);  
  delay(2000); 
 }
 if(tempC<16){
  flag_c=0;
  Blynk.virtualWrite(V4, "OFF");
  }
  else if(tempC>29){
    flag_c=1;
    Blynk.virtualWrite(V4, "ON");
    }
  if(flag_c==1){
    digitalWrite(coolant,HIGH);
    }
    else if(flag_c==0){
      digitalWrite(coolant,LOW);
      }
 Blynk.run();
 }
