#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

DHT dht(DHTPIN, DHT22);

void setup() 
{
    Serial.begin(9600); 
    Serial.println("************ DHTxx test! ************");
    delay(2000);
}

void loop() 
{
  switch(dht.read())
  {
    case DHT_OK:
      Serial.print("Humidity: "); 
      Serial.print(dht.humidity);
      Serial.print(" %\t");
      Serial.print("Temperature: "); 
      Serial.print(dht.temperature);
      Serial.println(" *C");
      break;
    case DHT_ERR_CHECK:
        Serial.println("DHT CHECK ERROR");break;
    case DHT_ERR_TIMEOUT:
        Serial.println("DHT TIMEOUT EEROR");break;
    default:
        Serial.println("UNKNOWN EEROR");break;
    }
    delay(2000);
}
