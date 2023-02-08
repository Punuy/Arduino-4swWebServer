#include <WiFi.h>
#include <WebServer.h>
/* ajout information temérature interne*/

#ifdef __cplusplus
  extern "C" {
 #endif
 
  uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();
/* FIN ajout information temérature interne*/

/*Put your SSID & Password*/
const char* ssid     = "WIFI";
const char* password = "CODEWIFI";

WebServer server(80);

uint8_t LED1pin = 4;
bool LED1status = LOW;

uint8_t LED2pin = 5;
bool LED2status = LOW;

uint8_t LED3pin = 19;
bool LED3status = LOW;



void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


// Convert raw temperature in F to Celsius degrees
  Serial.print("Temperature: ");
  Serial.print((temprature_sens_read() - 32) / 1.8);
  Serial.println("°C");

  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  
}
void loop() {
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}

if(LED3status)
  {digitalWrite(LED3pin, HIGH);}
  else
  {digitalWrite(LED3pin, LOW);}
  
}

 
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF | GPIO19 Status: OFF" );
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status));
 }

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status,LED3status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status,LED3status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true,LED3status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false,LED3status)); 
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,true)); 
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("GPIO19 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat,uint8_t led3stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Controleur de LED</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #34495e;}\n";
  ptr +=".button-on:active {background-color: #34495e;}\n";
  ptr +=".button-off {background-color: #008000;}\n";
  /*ptr +=".button-off:active {background-color: #FF0000;}\n";*/
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="q {font-size: 14px;color: #888;margin-bottom: 10px;}";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Serveur WEB</h1>\n";

{
  ptr +="<p>Temperature Interne ESP32 : ";
  ptr +=((temprature_sens_read() - 32) / 1.8);
  ptr +=" &deg;C </p>";
  }
  
  ptr +="<h3>Test de Controle de LED</h3>\n";
 
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  if(led3stat)
  {ptr +="<p>LED3 Status: ON</p><a class=\"button button-off\" href=\"/led3off\">OFF</a>\n";}
  else
  {ptr +="<p>LED3 Status: OFF</p><a class=\"button button-on\" href=\"/led3on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

}
void loop() {
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}

if(LED3status)
  {digitalWrite(LED3pin, HIGH);}
  else
  {digitalWrite(LED3pin, LOW);}
  
}

 
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF | GPIO19 Status: OFF" );
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status));
 }

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status,LED3status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status,LED3status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true,LED3status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false,LED3status)); 
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,true)); 
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("GPIO19 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat,uint8_t led3stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Controleur de LED</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #34495e;}\n";
  ptr +=".button-on:active {background-color: #34495e;}\n";
  ptr +=".button-off {background-color: #008000;}\n";
  /*ptr +=".button-off:active {background-color: #FF0000;}\n";*/
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="q {font-size: 14px;color: #888;margin-bottom: 10px;}";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Serveur WEB</h1>\n";

{
  ptr +="<p>Temperature Interne ESP32 : ";
  ptr +=((temprature_sens_read() - 32) / 1.8);
  ptr +=" &deg;C </p>";
  }
  
  ptr +="<h3>Test de Controle de LED</h3>\n";
 
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  if(led3stat)
  {ptr +="<p>LED3 Status: ON</p><a class=\"button button-off\" href=\"/led3off\">OFF</a>\n";}
  else
  {ptr +="<p>LED3 Status: OFF</p><a class=\"button button-on\" href=\"/led3on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
