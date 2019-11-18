#include <OneWire.h>
#include <DallasTemperature.h>
#include <GSM.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

#define ONE_WIRE_BUS 5
#define PINNUMBER ""
#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[100]; 
uint16_t redBuffer[100];  
#else
uint32_t irBuffer[100]; 
uint32_t redBuffer[100];  
#endif

GSM gsmAccess;
GSM_SMS sms;
MAX30105 particleSensor;
TinyGPSPlus gps;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celcius=0;
int adc;
boolean counter;
int count;
unsigned long millisBefore;
unsigned long beatTime=20000;
const int threshold=450; 
String stringone,stringtwo,stringthree,stringfour,stringfive,stringsix,stringseven;

const char *gpsStream =
"$GPRMC,135813.000,A,1254.4697,N,07733.9333,E,0.08,59.26,220419,,,A*5B\r\n";

int32_t bufferLength; 
int32_t spo2; 
int8_t validSPO2;
int32_t heartRate; 
int8_t validHeartRate; 

byte pulseLED = 11;
byte readLED = 13;

void setup() {
 
  Serial.begin(115200);
  boolean notConnected = true;
  counter = true;
  Serial.println("=====================================================================================");
  Serial.println("");
  Serial.println("Open Day Project Exhibition 2019");
  Serial.println("Dynamic Information Gathering On Vital Statistics Of Soldiers For Optimum Medication");
  Serial.println("Batch Number- R40");
  Serial.println("");
  Serial.println("=====================================================================================");
  stringone=String("Temperature(C):");
  stringtwo=String(" Pulse(BPM):");
  stringthree=String(" Oxygen Saturation(%):");
  stringfour=String();
  stringfive=String();
  stringsix=String();
  stringseven=String("Location:");
 

  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("GSM Not Connected");
      delay(1000);
    }
  }
  Serial.println("GSM Initialized");

  while (*gpsStream)
    if (gps.encode(*gpsStream++))
      gps_location();

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("PulseOximetry Not Initialized."));
    while (1);
  }

  Serial.read();

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  
  
}

void loop()
{ 
 char remoteNum[20]={'8','5','5','3','2','1','2','7','2','1'};
 bufferLength = 100;

 temperature();
 pulse_oximetry();
 gps_location();

 millisBefore = millis();
 
 
 int Bpm;
  
  while((millis() - millisBefore) < beatTime) {
    adc = analogRead(A0);
    if (counter == true) {
      if (adc >= threshold) {
        count++;
        delay(550);
      }
  }  
  }
    
    Serial.println("Health Statistics of Soldier:");
    Serial.print("BPM:");
    Serial.print(count*(60000/beatTime));
    Serial.println("♥"); 
    Serial.print("Body Temperature:");
    Serial.print(Celcius);
    Serial.println(" C");
    Serial.print(F("Oxygen Saturation:"));
    Serial.print(spo2, DEC);
    Serial.println("%");
   
    
    Bpm=count*(60000/beatTime);
    stringfour=stringone + Celcius + stringtwo + Bpm + stringthree + spo2;
    
 if(Celcius>40)
 {
  Serial.print("==========LIFE THREATENING==========\n"); 
  stringfive="LIFE THREATENING";
 }
 else if(Celcius>38.5 and Celcius<=40)
 {
  if(Bpm<40 and spo2<87){
  Serial.print("===========HYPER-THERMIA============\n");
  Serial.println("Possible reasons");
  Serial.println("1)Exposure to extreme temperature - possibly in desert combat areas");
  Serial.println("2)Brain Lesion");
  stringfive="HYPER-THERMIA possible reasons: 1)Exposure to extreme temperature - possibly in desert combat areas 2) Brain Lesion";
  }
  else{
  }
 }
 else if(Celcius<36 and Celcius>=35){
 Serial.print("");
 }
 else if(Celcius<35 and Celcius>=34)
 {
 Serial.print("=========SUBNORMAL CONDITION========\n");
 stringfive="SUBNORMAL CONDITION";
 }
 else if(Celcius<34)
 {
  if(Bpm>100 or spo2<87){
  Serial.print("============HYPO-THERMIA============\n");
  Serial.println("Possible reasons");
  Serial.println("1)Cold weather exposure");
  Serial.println("2)Immersion in cold water");
  stringfive="HYPO-THERMIA possible reasons: 1)Cold weather exposure 2)Immersion in cold water";
  }
  else{
  }
 }
 else{ 
 Serial.print("\n");
 } 

 stringsix=stringseven+gps.location.lat()+gps.location.lng();

 sms.beginSMS(remoteNum);
 sms.print(stringfour);
 sms.print(stringfive);
 sms.print(stringsix);
 sms.endSMS();
    
 count=0;
 millisBefore = millis();
  }

void temperature(){
sensors.requestTemperatures(); 
Celcius=sensors.getTempCByIndex(0);
delay(10);
}

void pulse_oximetry(){
  
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) 
      particleSensor.check(); 

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); 
  }

 
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  while(validSPO2==0){
  for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

  for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) 
        particleSensor.check(); 

      digitalWrite(readLED, !digitalRead(readLED)); 

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); 
  }
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }

}

void gps_location()
{
  Serial.println("");
  Serial.print(F("Position Coordinates: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
  }
}