#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
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
  if(!(ThingSpeak.begin(client)))
  {
    return false;
  }  
  return true;
}

//serial com for inputing wifi setup
void inwifi()
{
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

//serial com to enter inputing thingspeak
void inth()
{

 speakstring.myChannelNumber=0;
 speakstring.myWriteAPIKey[0]='\0';

  Serial.println("masukan channel");
  while (Serial.available() >= 0 && 0==  speakstring.myChannelNumber) 
  {
     speakstring.myChannelNumber=Serial.parseInt(); 
    
  }
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

 //setup
void setup() 
{

   Serial.begin(9600);
 delay(2500);

  do
  {
    inwifi();
  }
  while(!wifi());
  
  Serial.println("");  
  Serial.println("WiFi connected"); 
  Serial.println(WiFi.localIP());  
  do
  {
     inth();
  }while(!thinginit());
}


//loop
void loop() 
{
  int var=0;
  ThingSpeak.writeField(speakstring.myChannelNumber,1, var, speakstring.myWriteAPIKey);

  delay(2000);
}
