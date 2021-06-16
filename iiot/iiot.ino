#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

byte NTCPin = A0;   //temp sensort anlog i/p
int flame = 16;     //flame sensor i/p
int  val;           //flame sensor i/p var
String sendval, postData;
char sendval2;

void setup() {
  Serial.begin(9600);

  //Start the WiFi Module in AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP("NodeMCU", "nodemcur");

  delay(200);

  //print ... till connection is established
  while (WiFi.softAPgetStationNum() != 1) {
    Serial.print(".");
    delay(100);
  }
  delay(500);
}

void loop() {
  //read sensor values

  val = digitalRead(flame);
  float ADCvalue;
  ADCvalue = analogRead(NTCPin);
  float temperature = getTemp(ADCvalue);
  // print sensor values
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Flame: ");
  Serial.println(val);
  delay(450);


  HTTPClient http;
  sendval = float(temperature);
  if (val == 1) {
    sendval2 = 89;
  }

  else {
    sendval2 = 78;
  }
  postData = "sendval=" + sendval + "&sendval2=" + sendval2;
  
// connect host to where the database is connected; send values via php script 

  http.begin("http://192.168.4.2/dbwrite.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  Serial.println("Values are, sendval = " + sendval + " and sendval2 = " + sendval2 );

  if (httpCode == 200) {
    Serial.println("Values uploaded successfully.");
    Serial.println(httpCode);
    String webpage = http.getString();
    Serial.println(webpage + "\n");

    delay(1000);

  } else {

    Serial.println(httpCode);
    Serial.println("Failed to upload values. \n");
    http.end();
    return;
  }
}

//temperature calculation for the digital temperature module
float getTemp(float analogVal) {
  static const int SERIESRESISTOR = 10000;
  static const int NOMINAL_RESISTANCE = 10000;
  static const int NOMINAL_TEMPERATURE = 25;
  static const int BCOEFFICIENT = 3950;
  float Resistance;
  Resistance = (1023 / (analogVal * 2.6)) - 1;
  Resistance = SERIESRESISTOR / Resistance;

  float temperature;
  temperature = Resistance / NOMINAL_RESISTANCE; // (R/Ro)
  temperature = log(temperature); // ln(R/Ro)
  temperature /= BCOEFFICIENT; // 1/B * ln(R/Ro)
  temperature += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/To)
  temperature = 1.0 / temperature; // Invert
  temperature -= 273.15; // convert to C
  return temperature;
}
