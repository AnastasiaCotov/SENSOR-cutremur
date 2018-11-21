#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>
SoftwareSerial pitch(13, 15, false, 128);
SoftwareSerial roll(12, 14, false, 128);

int buzzer = 14;
int fum = A0;
int sensorThres = 400;
uint32_t analog_Smoke_Sensor;

const char* ssid = "no";
const char* password = "14151415";


int buf[50];

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(fum, INPUT);
  
  Serial.begin(115200);
  pitch.begin(115200);
  roll.begin(115200);
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  //Serial.print("Wait for WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
  delay(50);
 /* Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/
  
  //server stuff
  const uint16_t port = 443;
  const char * host = "http://fathomless-citadel-40087.herokuapp.com"; // ip or dns
  /*Serial.print("connecting to ");
  Serial.println(host);*/
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    /*Serial.println("connection failed");
    Serial.println("wait 5 sec...");*/
    delay(5000);
    return;
  }
  else{
   // Serial.println("connected");
  }
}

void loop() {
  analog_Smoke_Sensor = analogRead(fum);
  Serial.print("sens smoke/gas value: ");
  Serial.println(analog_Smoke_Sensor);
  if (analog_Smoke_Sensor > sensorThres + 300)
  {
    int j = 200; // The starting pitch
    while(j < 500)
    {
      j++;
      tone(buzzer, j); // Emit the noise
      delay(5);
    }
  }
  else
  {
    noTone(buzzer);
  }
    
  HTTPClient http;
  String postData = String("http://fathomless-citadel-40087.herokuapp.com/device/1") + String("?data=") + String(analog_Smoke_Sensor);
  http.begin(postData);  
  int httpCode = http.GET(); 
  //Serial.println(httpCode);
  int b = rx();
  Serial.print("pitch = ");
  Serial.println(b);
  String postPitch = String("http://fathomless-citadel-40087.herokuapp.com/device/2") + String("?data=") + String(b);
  http.begin(postPitch);
  int httpCode1 = http.GET();
  
  int c = rx1();
  Serial.print("roll = ");
  Serial.println(c);
  String postRoll = String("http://fathomless-citadel-40087.herokuapp.com/device/3") + String("?data=") + String(c);
  http.begin(postRoll);
  int httpCode2 = http.GET();
  
  if (httpCode > 0) { //Check the returning code
  String payload = http.getString();   //Get the request response payload
  //Serial.println(payload);                     //Print the response payload
  }
  http.end();   //Close connection
  delay(3000);
}
int rx()
{
  if (pitch.available() > 0)
  {
    int p = pitch.read();
    return p;
  }
}
int rx1()
{
  if (roll.available() > 0)
  {
    int r = roll.read();
    return r;
  }
}
