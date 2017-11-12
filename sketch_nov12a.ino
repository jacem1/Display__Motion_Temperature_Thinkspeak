// Author Mohamed Jacem Guezguez
// Email: mohamed.jacem[AT]gmail.com

#include <SoftwareSerial.h>

SoftwareSerial ESP8266(10, 11);

String NomduReseauWifi = "My_Wifi"; // Garder les guillements            
String MotDePasse      = "Pass"; // Garder les guillements                             
String IP = "api.thingspeak.com";
String Port = "80";
String OK = "0";

const int sensorPin = A5;


/****************************************************************/
/*                             INIT                             */
/****************************************************************/

int pirPin = 3;    //the digital pin connected to the PIR sensor's output
unsigned long previousMillis = 0; 
 
const long interval = 5000;  
 
void setup()
{


 /// ************** Sensor calibration
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  //digitalWrite(pirPin, LOW);

  ESP8266.begin(115200);  
  initESP8266();

  pinMode(7,OUTPUT);
  digitalWrite(7, LOW);
}
/****************************************************************/
/*                        BOUCLE INFINIE                        */
/****************************************************************/
void loop()
{

int sensorVal=analogRead(sensorPin);
float voltage = (sensorVal/1024.0) * 5.0;
float temperature = (voltage - .5) * 100;

OK="0";
  unsigned long currentMillis = millis();

if (digitalRead(pirPin) == HIGH) 
{
  OK="1";
}

  if (currentMillis - previousMillis >= interval) {

Serial.println(digitalRead(pirPin));
    
//    // save the last time you blinked the LED
    previousMillis = currentMillis;

Serial.println("Motion Event: "+OK);
Serial.print("Temperature: ");
Serial.println(temperature);

 if (OK == "1") 
{
   Serial.println("Motion Detected");
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);

  
  }//if (OK == "1") 


    Serial.println("********************** Start sending ***********************");
envoieAuESP8266("AT+CIPSTART=0,\""+ TCP + "\",\"" + IP + "\",80");


  recoitDuESP8266(500);
envoieAuESP8266("AT+CIPSEND=0,86");
recoitDuESP8266(500);

  envoieAuESP8266("GET https://api.thingspeak.com/update?api_key=YOUR_KEY_IS_HERE&field1="+OK+"&field2="+String(temperature));
 recoitDuESP8266(500);

  Serial.println("************************ End sending *********************");
    
  }// fin if (currentMillis - previousMillis >= interval) {
  
 
} // end loop
/****************************************************************/
/*                Fonction qui initialise l'ESP8266             */
/****************************************************************/
void initESP8266()
{  
//  Serial.println("**********************************************************");  

envoieAuESP8266("AT+RST");
recoitDuESP8266(2000);
  Serial.println("****************** Start Wifi connection ******************");
  envoieAuESP8266("AT+CWMODE=1");
  recoitDuESP8266(2000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CWJAP=\""+ NomduReseauWifi + "\",\"" + MotDePasse +"\"");
  recoitDuESP8266(2000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIFSR");
  recoitDuESP8266(2000);
    Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIPMODE=0");
  recoitDuESP8266(2000);
  Serial.println("**********************************************************");
   envoieAuESP8266("AT+CIPMUX=1");
  recoitDuESP8266(2000);

  Serial.println("**********************************************************");
  Serial.println("***************** INITIALISATION TERMINEE ****************");
  Serial.println("**********************************************************");
  Serial.println("");  
  delay(5000);
 
}

/****************************************************************/
/*        Fonction qui envoie une commande à l'ESP8266          */
/****************************************************************/
void envoieAuESP8266(String commande)
{  
  ESP8266.println(commande);
}
/****************************************************************/
/*Fonction qui lit et affiche les messages envoyés par l'ESP8266*/
/****************************************************************/
void recoitDuESP8266(const int timeout)
{
  String reponse = "";
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(ESP8266.available())
    {
      char c = ESP8266.read();
      reponse+=c;
    }
  }
  Serial.print(reponse);   
}
