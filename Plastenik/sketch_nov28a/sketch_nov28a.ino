#include <RTClib.h>

//#include <DS3231.h>

RTC_DS3231 rtc;
 
void setup()
{
  Serial.begin(9600);
  //rtc.adjust(DateTime(2018, 11, 28, 18, 0, 6));
   
  //rtc.setDOW(WEDNESDAY);     // postavite dan u tjednu
  //rtc.setTime(9, 17, 0);     // postavite vrijeme, 24h format (sat, minuta, sekunda)
  //rtc.setDate(17, 2, 2016);  // postavite datum (dan, mjesec, godina)
}
 
void loop()
{
  DateTime now = rtc.now();
  Serial.println("godine");
  Serial.println(now.year());
  Serial.println("sekunde");
  Serial.println(now.second(), DEC);
  delay(1000);
}
