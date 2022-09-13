#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <StringSplitter.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const char *host = "api.thingspeak.com";
const int httpPort = 80;  
const char* url1 = "/apps/thinghttp/send_request?api_key=D196HG3VXOBCALPB"; 
HTTPClient http; 

String cases;
String death;
String recover;

void setup() 
{
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  WiFi.begin("iot", "project1234");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  http.begin(host,httpPort,url1); 
  int httpCode = http.GET();
  String data = http.getString();
  Serial.println(httpCode);
  Serial.println("***************        Data Read        ***************");
  Serial.println(data);
  
  StringSplitter *splitter = new StringSplitter(data, '/', 3);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  Serial.println("***************        Data Splited        ***************");
  Serial.println("Item count: " + String(itemCount));

  for(int i = 0; i < itemCount; i++)
  {
    String item = splitter->getItemAtIndex(i);
    Serial.println("Item @ index " + String(i) + ": " + String(item));
    if(i == 0)
    {
      cases = item;
    }
    if(i == 1)
    {
      death = item;
    }
    if(i == 2)
    {
      recover = item;
    }
  }
  cases.remove(0,26);
  cases.remove(cases.length()-2,cases.length());
  death.remove(0,13);
  death.remove(death.length()-1,death.length());
  recover.remove(0,13);
  recover.remove(recover.length()-8, recover.length());
  
  Serial.println("***************        Data Filtered        ***************");
  Serial.println(cases);
  Serial.println(death);
  Serial.println(recover);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("COVID-19 India");
  lcd.setCursor(0,1);
  lcd.print("Cases: " + (String) cases);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("COVID-19 India");
  lcd.setCursor(0,1);
  lcd.print("Deaths: " + (String) death);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("COVID-19 India");
  lcd.setCursor(0,1);
  lcd.print("Recover: " + (String) recover);
  delay(3000);
}
