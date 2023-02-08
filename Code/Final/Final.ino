#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "WIFI";
const char* password = "CODEWIFI";

IPAddress local_ip(192,168,1,5);
IPAddress gateway(192,168,1,1);  
IPAddress subnet(255,255,255,0);
WebServer server(80);

uint8_t LED1pin = 27;
bool LED1status = LOW;

uint8_t LED2pin = 26;
bool LED2status = LOW;

uint8_t LED3pin = 25;
bool LED3status = LOW;

uint8_t LED4pin = 33;
bool LED4status = LOW;

void setup() 
{
  Serial.begin(115200);
  delay(10);
  
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
  pinMode(LED4pin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
  server.on("/led4on", handle_led3on);
  server.on("/led4off", handle_led3off);  
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() 
{
  server.handleClient();
  if(LED1status)
  {
    digitalWrite(LED1pin, HIGH);
    }
  else
  {
    digitalWrite(LED1pin, LOW);
    }
  
  if(LED2status)
  {
    digitalWrite(LED2pin, HIGH);
    }
  else
  {
    digitalWrite(LED2pin, LOW);}

  if(LED3status)
  {
    digitalWrite(LED3pin, HIGH);
    }
  else
  {
    digitalWrite(LED3pin, LOW);
    }
  
  if(LED4status)
  {
    digitalWrite(LED4pin, HIGH);
    }
  else
  {
    digitalWrite(LED4pin, LOW);
    }
}






void handle_OnConnect()
{
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF | GPIO19 Status: OFF" );
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,LED4status));

}





void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status,LED3status,LED4status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status,LED3status,LED4status)); 
}





void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true,LED3status,LED4status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false,LED3status,LED4status)); 
}





void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,true)); 
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("GPIO19 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,false)); 
}





void handle_led4on() {
  LED4status = HIGH;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,true)); 
}

void handle_led4off() {
  LED4status = LOW;
  Serial.println("GPIO19 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,LED3status,false)); 
}





void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}





String SendHTML(uint8_t led1stat,uint8_t led2stat,uint8_t led3stat,uint8_t led4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
 
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

  if(led4stat)
  {ptr +="<p>LED4 Status: ON</p><a class=\"button button-off\" href=\"/led4off\">OFF</a>\n";}
  else
  {ptr +="<p>LED4 Status: OFF</p><a class=\"button button-on\" href=\"/led4on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}