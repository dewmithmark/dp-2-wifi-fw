#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 15

const char* ssid = "iPhone"; 
const char* password = "dewmith112"; 
String received_current;
String received_voltage;

WebServer server(80);
char timerStatus = '0'; 
uint8_t counter = 0;


void send_relay_status(char status)
{
  if(status == '1')
  {
    Serial.print('A');
  }
  else if(status == '0')
  {
    Serial.print('B');
  }
}

void send_data_to_app() 
{
  String message = "voltage=" + received_voltage + "&current=" + received_current;
  Serial.println("Sending data: " + message);
  server.send(200, "text/plain", message);
}

void setup() 
{

  Serial.begin(9600);

  WiFi.begin(ssid, password);

  pinMode(LED_PIN , OUTPUT);

  while (WiFi.status() != WL_CONNECTED)
   {
    digitalWrite(LED_PIN , HIGH);
    delay(500);
    digitalWrite(LED_PIN , LOW);
    delay(500);
  }

    /wifi completely connected/
    digitalWrite(LED_PIN , HIGH);
    delay(3000);
    digitalWrite(LED_PIN , LOW);
   
    

  server.on("/status", HTTP_GET, []() {
    if (server.hasArg("value")) {
      timerStatus = server.arg("value")[0]; // Get the first character of the "value" parameter
      send_relay_status(timerStatus);
      server.send(200, "text/plain", "Status updated");
      digitalWrite(LED_PIN , HIGH);
      delay(200);
      digitalWrite(LED_PIN , LOW);
      delay(200);
    } else {
      server.send(400, "text/plain", "Bad request");
    }
  });

  server.on("/data", HTTP_GET, send_data_to_app);

  server.begin();
}


void loop()
{
  server.handleClient();

  if(Serial.available() > 0)
  {
    counter++;
    if(counter % 2 == 0)
    {
      received_current = Serial.read();
    }
    else if(counter % 2 == 1)
    {
      received_voltage = Serial.read();
      counter = 0;
    } 
    
  }
}