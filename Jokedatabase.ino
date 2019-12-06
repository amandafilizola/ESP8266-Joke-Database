#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

const char* ssid = "AndroidAPDF90";
const char* password = "ibfu0845";

String piada = "";
int stringStart = 0;
int stringStop = 0;
int scrollCursor = 0;
int tamanho = 0;
int roda = 0;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup()
{
   pinMode(D4, INPUT_PULLUP);
   roda=0;

   WiFi.begin(ssid, password);
   Serial.begin(9600);
   lcd.init();            
   lcd.backlight();
   lcd.setCursor(4, 1);          
    while (WiFi.status() != WL_CONNECTED)
    {    
      Serial.println("Connecting..");
      lcd.setCursor(4, 1);
      lcd.print("Conectando   ");
      delay(250);
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("Conectando.  ");
      delay(250);
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("Conectando.. ");
      delay(250);
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("Conectando...");
      delay(250);
      lcd.clear();
    }
    lcd.setCursor(6, 1);
    lcd.print("Conectado");
    Serial.println("Conectou");
    delay(500);
}

void loop()
{
  if(digitalRead(D4) == 0)
  {
    lcd.clear();
    scrollCursor = 0;
    stringStart = 0;
    stringStop = 0;
    
    delay(600);
    if(digitalRead(D4)==1)
      {
      piada = request();
      Serial.println(piada);
      }
  }
    tamanho=piada.length();
    lcd.setCursor(0, 0);
    scroll_sup();
    Serial.print(stringStart);
    Serial.print("  ");
    Serial.print(stringStop);
    Serial.print("  ");
    Serial.println(scrollCursor);
    lcd.print(piada.substring(stringStart,stringStop));
    delay(200);
    if(stringStart == tamanho)
    {
      stringStart = 0;
      stringStop = 0;
    } 
}

String request() 
{
  Serial.println("entrou no request");
  String payload;
  if (WiFi.status() == WL_CONNECTED)
  {  
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://joke-database.herokuapp.com/piadas");  //Specify request destination

    int httpCode = http.GET();   //Send the request
    if (httpCode > 0)
    { //Check the returning code
      payload = http.getString();   //Get the request response payload
    }
    http.end();   //Close connection
  }
  return payload;
}


void scroll_sup()
{ 
  if(stringStart == 0 && scrollCursor > 0)
  {
    scrollCursor--;
    stringStop++;
  }
  else if(stringStart == stringStop)
  {
    lcd.clear();
    stringStart = stringStop = 0;
    scrollCursor = 20;
  }
  else if(stringStop == piada.length() && scrollCursor == 0)
  {
    lcd.clear();
    stringStart++;
  }
  else
  {
    stringStart++;
    stringStop++;
  }
}
