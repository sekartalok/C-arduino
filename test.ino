#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#define geter D0
#define sensorPin A0
#define  maxloop 30

struct confwif
{
char ssid[100];  
char password[100];
};

struct speak
{
long myChannelNumber; //channel ID
char myWriteAPIKey[100]; //write AP
};

struct confwif wifistring;
struct speak speakstring;

WiFiClient client; 

long vib()
{
  return pulseIn(geter,1);
}
float tempr()
{
  int sensorValue = analogRead(sensorPin); 
  return (sensorValue*300) /1023;
}

bool wifi()
{
  Serial.print("Connecting to ");  
  Serial.println(wifistring.ssid);  
  WiFi.begin(wifistring.ssid, wifistring.password);  

  int i=0;
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
      i++;
      if(i > maxloop)
      {
      return false;
      }
  }
  return true;
}
bool thinginit()
{
  bool cona = 0;
  bool conb = 0;
  int i=0;
  int tesa;
  int tesb;
  ThingSpeak.begin(client);
  delay(500);
  Serial.println("connecting thingspeak");
  while(i< maxloop )
  {
    if(!cona)
    {
      tesa = ThingSpeak.writeField(speakstring.myChannelNumber,2 ,100, speakstring.myWriteAPIKey);
    }
      delay(2000);
    if(!conb)
    {
      tesb = ThingSpeak.writeField(speakstring.myChannelNumber, 1,100, speakstring.myWriteAPIKey);
    }
       Serial.print(".");
       if( 200==tesa )
       {
        cona= 1;
       }
       if( 200==tesb)
       {
        conb=1;
       }
       if( cona && conb)
       {
        Serial.println("berhasil");
        break;
       }
      delay(1000);
    i++;
  }
  Serial.println("");


  
  if(!(cona && conb))
  {
    return false;
  }  
  return true;
}

void inwifi()
{
    
  Serial.println("");  
  wifistring.ssid[0]='\0';
  wifistring.password[0]='\0';
 
  Serial.println("masukan ssid wifi");
  while (Serial.available() >= 0 && '\0'==  wifistring.ssid[0]) 
  {
    int bytesRead = Serial.readBytesUntil('\n',wifistring.ssid, sizeof( wifistring.ssid)-1);
     wifistring.ssid[bytesRead] = '\0';
    delay(1000);
  }
 Serial.println(wifistring.ssid);
 Serial.println("masukan password wifi");
   while (Serial.available() >= 0 && '\0'==  wifistring.password[0]) 
  {
    int bytesRead = Serial.readBytesUntil('\n',wifistring.password, sizeof(wifistring.password)-1);
     wifistring.password[bytesRead] = '\0';
     delay(1000);
  }
   Serial.println(wifistring.password);
   delay(500);
 
}

void inth()
{

 speakstring.myChannelNumber=0;
 speakstring.myWriteAPIKey[0]='\0';

  Serial.println("masukan channel");
  while (Serial.available() >= 0 && 0==  speakstring.myChannelNumber) 
  {
     speakstring.myChannelNumber=Serial.parseInt(); 
    
  }
  delay(500);
 Serial.println( speakstring.myChannelNumber);
 Serial.println("masukan ap");
   while (Serial.available() >= 0 && '\0'==  speakstring.myWriteAPIKey[0]) 
  {
    int bytesRead = Serial.readBytesUntil('\n', speakstring.myWriteAPIKey, sizeof( speakstring.myWriteAPIKey)-1);
      speakstring.myWriteAPIKey[bytesRead] = '\0';
  }
   Serial.println( speakstring.myWriteAPIKey);
   delay(500);
 
}
void idle()
{
  while(!WiFi.localIP())
  {
    Serial.println("idle no connection");
    delay(50000);
  }
}
void setup() 
{

 Serial.begin(9600);
 delay(2500);

  do
  {
    inwifi();
  }
  while(!wifi());

  Serial.println("WiFi connected"); 
  Serial.println("ganyu wangi");  
  do
  {
     inth();
  }while(!thinginit());
}

  bool cona=0;
  bool conb=1;
  int tmpa=0;
  int tmpb=0;
  float tm =0;
  int vibrat =0;

void loop() 
{
  if(!WiFi.localIP())
  {
    idle(); 
  }
  
  if(!cona)
  {
  vibrat =vib();
  Serial.print("getar");
  Serial.println(vibrat);
  tmpa=ThingSpeak.writeField(speakstring.myChannelNumber,2 , vibrat, speakstring.myWriteAPIKey);
  }
  delay(1000);
  if(!conb)
  {
   tm =tempr();
  Serial.print("suhu:");
  Serial.println(tm);
  tmpb=ThingSpeak.writeField(speakstring.myChannelNumber, 1, tm, speakstring.myWriteAPIKey);
 
  }
  if(200==tmpa||(-401!=tmpa && 0!=tmpa ))
  {
  Serial.println("a");
  conb=0;
  cona=1;
  tmpa=0;
  }
  if(200==tmpb||(-401!=tmpb && 0!=tmpb))
  {
  
  Serial.println("b");
  cona=0;
  conb=1;
  tmpb=0;
  }
  delay(1000);
}
