//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h>    // Install DHT11 Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "iot-ac-controller.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "6DqWTDIiz3QvjM3duaIcLFVLwpp38LLe4Y9FPM7F"
#define WIFI_SSID "osamah"
#define WIFI_PASSWORD "123456789"

#define DHTPIN 5    // Connect Data pin of DHT to D2
int AC = D5;     // Connect LED to D5


#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
//FirebaseData ACData;
FirebaseData ACTVData;

FirebaseJson json;


void setup()
{
 pinMode(AC, OUTPUT);
 
  Serial.begin(9600);

  dht.begin();
  pinMode(AC,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}
/**************************************************************************DHT11 Sensor********************************************************************************/
int sensorUpdate(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
 // // Read temperature as Fahrenheit (isFahrenheit = true)
 // float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
   
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.println(F("%"));
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F(" C"));
 
  if (Firebase.setFloat(firebaseData, "/Room 35/Temp", t))
  {
    Serial.println("PASSED temperature");
    
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

 if (Firebase.setFloat(firebaseData, "/Room 35/Hum", h))
  {
    Serial.println("PASSED humidity");
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
return h;  
}
/**********************************************************************************************************************************************************/
/***************************************************************************AC Opration*******************************************************************************/  
void ACopration(int h){
  
  if (Firebase.getString(ACTVData, "/Room 35/AC_operation")){
    Serial.print("ON/OFF :  ");
    Serial.println(ACTVData.stringData());
    if (ACTVData.stringData() == "1") {
    digitalWrite(AC, HIGH);
    ACData();
    PMV(h);
       
    }
  else if (ACTVData.stringData() == "0"){
    digitalWrite(AC, LOW);
    
    }
  }
}
/**********************************************************************************************************************************************************/
/******************************************************************************AC Data****************************************************************************/
int ACData(){
  
     if (Firebase.getString(ACTVData, "/Room 35/AC_Temperature")){
      Serial.print("ACTemp :  ");
      Serial.println(ACTVData.stringData()); 
      }
      
     if (Firebase.getString(ACTVData, "/Room 35/Fan")){
      Serial.print("Fan    :  ");
      Serial.println(ACTVData.stringData()); 
          }
      
     if (Firebase.getString(ACTVData, "/Room 35/Mode")){
      Serial.print("Mode   :  ");
      Serial.println(ACTVData.stringData()); 
          }
      
     if (Firebase.getString(ACTVData, "/Room 35/Brand")){
      Serial.print("Brand  :  ");
      Serial.println(ACTVData.stringData()); 
        }
  }
/**********************************************************************************************************************************************************/
/**********************************************************************************************************************************************************/
void PMV(int H){
float  Feel;
String val;
int Hum = H;  
int T;
float v;

if (Firebase.getString(ACTVData, "/Room 35/Feeling")){
   val = ACTVData.stringData();
   Feel = val.toFloat();  
        }
 
    if (Feel == -1){
      if (Hum>=0 && Hum<10){
        T = 24;
        v = 0.341;
        }
      else if (Hum>=10 && Hum<20){
        T = 23;
        v = 0.312;
        }
      else if (Hum>=20 && Hum<30){
        T = 22;
        v = 0.286;
        }
      else if (Hum>=30 && Hum<40){
        T = 21;
        v = 0.263;
        }
      else if (Hum>=40 && Hum<50){
        T = 21;
        v = 0.241;
        }
      else if (Hum>=50 && Hum<60){
        T = 20;
        v = 0.223;
        }
      else if (Hum>=60 && Hum<70){
        T = 19;
        v = 0.206;
        }
      else if (Hum>=70 && Hum<80){
        T = 18;
        v = 0.192;
        }
      else if (Hum>=80 && Hum<90){
        T = 17;
        v = 0.179;
        }
      else if (Hum>=90 && Hum<100){
        T = 16;
        v = 0.168;
        }
      else if (Hum == 100){
        T = 15;
        v = 0.159;
        }
      }
      
    else if (Feel == -0.5){
      if (Hum == 0){
        T = 25;
        v = 0.414;
        }
      else if (Hum == 10){
        T = 24;
        v = 0.377;
        }
      else if (Hum == 20){
        T = 24;
        v = 0.346;
        }
      else if (Hum == 30){
        T = 23;
        v = 0.317;
        }
      else if (Hum == 40){
        T = 22;
        v = 0.292;
        }
      else if (Hum == 50){
        T = 22;
        v = 0.268;
        }
      else if (Hum == 60){
        T = 21;
        v = 0.297;
        }
      else if (Hum == 70){
        T = 20;
        v = 0.229;
        }
      else if (Hum > 75 || Hum <= 85){
        T = 19;
        v = 0.212;
        }
      else if (Hum > 85 || Hum <= 95){
        T = 18;
        v = 0.197;
        }
      else if (Hum == 100){
        T = 17;
        v = 0.184;
        }
      }
    
    else if (Feel == 0){
      if (Hum == 0){
        T = 26;
        v = 0.520;
        }
      else if (Hum == 10){
        T = 26;
        v = 0.461;
        }
      else if (Hum == 20){
        T = 25;
        v = 0.417;
        }
      else if (Hum == 30){
        T = 24;
        v = 0.381;
        }
      else if (Hum == 40){
        T = 24;
        v = 0.350;
        }
      else if (Hum == 50){
        T = 23;
        v = 0.322;
        }
      else if (Hum == 60){
        T = 23;
        v = 0.297;
        }
      else if (Hum == 70){
        T = 22;
        v = 0.274;
        }
      else if (Hum == 80){
        T = 21;
        v = 0.253;
        }
      else if (Hum == 90){
        T = 20;
        v = 0.234;
        }
      else if (Hum == 100){
        T = 19;
        v = 0.217;
        }
      }
      
    else if (Feel == 0.5){
      if (Hum == 0){
        T = 28;
        v = 0.642;
        }
      else if (Hum == 10){
        T = 27;
        v = 0.636;
        }
      else if (Hum == 20){
        T = 26;
        v = 0.517;
        }
      else if (Hum == 30){
        T = 26;
        v = 0.462;
        }
      else if (Hum == 40){
        T = 25;
        v = 0.420;
        }
      else if (Hum == 50){
        T = 24;
        v = 0.385;
        }
      else if (Hum == 60){
        T = 24;
        v = 0.354;
        }
      else if (Hum == 70){
        T = 23;
        v = 0.327;
        }
      else if (Hum == 80){
        T = 23;
        v = 0.302;
        }
      else if (Hum == 90){
        T = 22;
        v = 0.279;
        }
      else if (Hum == 100){
        T = 21;
        v = 0.258;
        }
      }
      
    else if (Feel == 1){
      if (Hum == 0){
        T = 28;
        v = 0.709;
        }
      else if (Hum == 10){
        T = 28;
        v = 0.706;
        }
      else if (Hum == 20){
        T = 28;
        v = 0.680;
        }
      else if (Hum == 30){
        T = 27;
        v = 0.590;
        }
      else if (Hum == 40){
        T = 26;
        v = 0.515;
        }
      else if (Hum == 50){
        T = 26;
        v = 0.464;
        }
      else if (Hum == 60){
        T = 25;
        v = 0.423;
        }
      else if (Hum == 70){
        T = 25;
        v = 0.389;
        }
      else if (Hum == 80){
        T = 24;
        v = 0.358;
        }
      else if (Hum == 90){
        T = 23;
        v = 0.331;
        }
      else if (Hum == 100){
        T = 23;
        v = 0.306;
        }
      }
 Serial.print("Feel   :  ");
 Serial.println(Feel);
 Serial.print("Hum   :  ");
 Serial.println(Hum);
 Serial.println(T);
 Serial.println(v);
 int V;
 if (v>0 && v<=3.5){
  V =1;// slow
  IR(T,V);
  }
 else if (v>3.5 && v<=5.5){
  V =2;// midiam
  IR(T,V);
  }
 else if (v>5.5 && v<=8){
  V =3;// fast
  IR(T,V);
  }

  
  }

/**********************************************************************************************************************************************************/
/****************************************************************************IR LED******************************************************************************/
void IR (int T, float V){
 int temp = T;
 int flow = V;    
 Serial.println("....................................");
 Serial.println("....................................");
 Serial.println(temp);
 Serial.println(flow);
 Serial.println("....................................");   
  }
/**********************************************************************************************************************************************************/
void loop() {
   int Hum = sensorUpdate();
   ACopration(Hum);

   
 delay(100);
}
