#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <DS3231.h> //importujem librarie
#include <dht.h>

//adresa ldc-a 0x3F
dht DHT;

#define DHT11_PIN 7 //definiram digitalni 7 pin kao pin 
DS3231  rtc(SDA, SCL);  //koji ocitaje vrijednost temperature
                        // i vlaznosti zraka(samo temperature u ovom slucaju
                        
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //konfiguracija lcd displeja
Time  t;
   
float temperatura_zraka = 0;
int pin_vlaznosti_tla = A1; //pin za vlaznost tla
float vlaznost_tla;
float postotak_vlaznosti_tla;     
int pin_grijac = 8;
int pin_ventilator = 9;
int vrijeme_iskljucenja = 0;
int vrijeme_sekunde = 0;
int vrijeme_minute = 0;


void setup() {
  Serial.begin(9600); //zapocinjem komunikaciju 9600 bps
  rtc.begin();        //inicializiram rtc
  pinMode(pin_vlaznosti_tla, INPUT);
  pinMode(7, INPUT);
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
}

void loop() {
  //rtc pocetak
  t = rtc.getTime();
  vrijeme_sekunde = t.sec;   //postavi u varijablu trenutno vrijeme(trenutne sekunde)
  vrijeme_minute = t.min;
  
  Serial.print("seconds ");
  Serial.println(vrijeme_sekunde);
  delay(1000);
  
  if (vrijeme_iskljucenja > 60){ 
          
    vrijeme_isljucenja -=60; //ako je na kraju zbroj veci od 60(ne smije biti, beskonacna petlja, nikada nece biti vise od 60 sekundi na satu)
                                    //oduzmi 60 od tog broja kako bi "formatirali" broj
     }
  
  //izvrsi svakih 5 minuta
  if (vrijeme_minute % 5){
    //rtc kraj
    //dht11 pocetak
    int dht = DHT.read11(DHT11_PIN);  //ocitavam vrijednosti dht11 i pohranjujem ih u varijablu
    temperatura_zraka = DHT.temperature;
    
    Serial.print("Temperature: ");
    Serial.println( temperatura_zraka);
    
    //dht11 kraj
    //soil pocetak
    vlaznost_tla = analogRead(pin_vlaznosti_tla);
    postotak_vlaznosti_tla = 100 - ((vlaznost_tla / 1023) * 100);
    
    Serial.print("Vlaznost tla je ");   //ispis vlaznosti tla u serial monitoru
    Serial.print(postotak_vlaznosti_tla);
    Serial.println("%");
    
    //soil kraj
    //pocetak logike za grijanje zraka
    
    if (not (temperatura_zraka < 26 || temperatura_zraka > 30)){  //ako su vrijednosti temperature izvan dopustene vrijednosti
                                                                 
        //pocetak podesavanja vremena aktivnog rada komponenti                 
        vrijeme_iskljucenja = vrijeme_sekunde + 5;   //koliko dugo ce trajati aktivno vrijeme //obratiti paznju da se prepravi u odgovarajuci broj
                                             //rada komponenti npr. 5 sekundi         //kako bi se sprijecila beskonacna petlja 
        //ako 5 sekundi ostane raditi grijac, ako je trenutno sekundi > 55
        
        }
        //kraj podesavanja vremena aktivnog rada komponenti
        //pocetak upravljanja komponentama ako je temperatura manja od zadane
        if (temperatura_zraka < 26){                                                  
          
          while (sekunde != vrijeme_isljucenja){
            
            digitalWrite(pin_grijac, HIGH);         //upali komponente na odredjeno vrijeme
            digitalWrite(pin_ventilator, HIGH);
            
            }
            
          digitalWrite(pin_grijac, LOW);            //ugasi komponente nakon ukljucenja
          digitalWrite(pin_ventilator, LOW);
          
          }                
          
        //kraj upravljanja komponentama ako je temperatura manja od zadane
        //pocetak upravljanja komponentama ako je temperatura veca od zadane
        if (temperatura_zraka > 30){  
          
          while (sekunde != vrijeme_isljucenja){  //mozda je potrebno duze vrijeme da se ohladi unitrasnjost
            digitalWrite(pin_ventilator, HIGH);    //ali unatoc tome, samo ce opet raditi 5 sekundi
            }
            
          digitalWrite(pin_ventilator, LOW);
          
        }
        
        //kraj upravljanja komponentama ako je temperatura veca od zadane
        
      }
    
  //kraj logike za grijanje zraka
  
     
  //pocetak ispisa informacija na lcd displej
  lcd.setCursor(0,0); //Defining positon to write from first row,first column .
  lcd.print(" Tech Maker "); //You can write 16 Characters per line .
  delay(1000);//Delay used to give a dynamic effect
  //kraj ispisa informacija na lcd displej

}
