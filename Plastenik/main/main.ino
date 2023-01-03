#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <dht.h> //importujem librarie

dht DHT;
#define DHT11_PIN 7 //definiram digitalni 7 pin kao pin 
                    //koji ocitaje vrijednost temperature
                    // i vlaznosti zraka(samo temperature u ovom slucaju
//adresa ldc-a 0x3F                      
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2); //konfiguracija lcd displeja
   
float temperatura_zraka = 0;
float vlaznost_zraka = 0;
int pin_vlaznosti_tla = A1; //pin za vlaznost tla
float vlaznost_tla;
float postotak_vlaznosti_tla;     
int pin_grijac = 8;
int pin_ventilator = 9;
unsigned long trenutno_vrijeme = 0;
unsigned long prethodno_vrijeme = 0;
int interval = 5000;
int vlaznost_tla_idealno = 10;//izrazava se u postotcima
int pin_pumpa = 10;

void setup() {
  Serial.begin(9600); //zapocinjem komunikaciju 9600 bps
  pinMode(pin_vlaznosti_tla, INPUT);    //vile, pin
  pinMode(pin_grijac, OUTPUT);      //grijac relej
  pinMode(pin_ventilator, OUTPUT);  //ventilator mosfet
  pinMode(pin_pumpa, OUTPUT); //pumpa reley
  pinMode(7, INPUT);  //dht-11
  lcd.init();//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
  digitalWrite(pin_grijac, HIGH);
  digitalWrite(pin_pumpa, HIGH);
  digitalWrite(pin_ventilator, LOW);
}

//============================================================================================================================
void pauza(){
  prethodno_vrijeme = trenutno_vrijeme;
  while (trenutno_vrijeme - prethodno_vrijeme <= interval){ //ispisiuj vlaznost zraka za odredjeni interval
    //Serial.println("vlaznost");
    trenutno_vrijeme = millis();  //ocitaj trenutno vrijeme i pohrani u trenutno_vrijeme
    }
  }

  
void loop() {
  //=================================================================================================================================
  dht11_temperature();  //ispis temperature zraka na lcd
  pauza();              //pauziraj odredjeni vremenski interval
  //delay(10000);
  reset_lcd();          //resetiraj lcd
  
  dht11_moisture();     //ispis vlaznosti zraka na lcd
  pauza();              //pauziraj odredjeni vremenski interval
  
  reset_lcd();
  
  soil_management();    //ispis vlaznosti tla
  pauza();

  air_controll();       //upravljanje zrakom
  pauza();
  digitalWrite(pin_grijac, HIGH);     //ugasi grijac i ventilator nakon rada odredjenog vremenskog intervala
  digitalWrite(pin_ventilator, LOW);
  soil_controll();      //upravljane tlom
  pauza();
  digitalWrite(pin_pumpa, HIGH);
  digitalWrite(pin_ventilator, LOW);
  }//void loop zatvoren

  
//==========================================================================================================

void dht11_temperature(){
  
  int dht = DHT.read11(DHT11_PIN);  //ocitavam vrijednosti dht11 i pohranjujem ih u varijablu
  temperatura_zraka = DHT.temperature;
  Serial.print("hello");
  lcd.setCursor(0,0); //Postavi kursor u prvi red, na prvo polje
  lcd.print("Temp. zraka je: "); //Estetski ispis teksta
  
  lcd.setCursor(0, 1);  //Postavi kursor u drugi red, na prvo polje
  lcd.print(temperatura_zraka); //Ispis vijednosti temperature zraka
  lcd.write(223); //Estetski ispis znaka °
  lcd.write("C"); //Estetski ispis slova C
  
  }
  
//====================================================================================================

 void dht11_moisture(){
  
  vlaznost_zraka = DHT.humidity;  //ocitavanje
  
  lcd.setCursor(0, 0);
  lcd.print("Vlaz. zraka je: ");  //estetski ispis

  lcd.setCursor(0, 1);
  lcd.print(vlaznost_zraka);      //ispis vlaznosti zraka u drugi red

  lcd.write("%");
  
  }
  
//======================================================================================================

void soil_management(){
  
  vlaznost_tla = analogRead(pin_vlaznosti_tla);       //ocitavanje vlaznosti tla
  postotak_vlaznosti_tla = 100 - ((vlaznost_tla / 1023) * 100); //pretvaranje vlaznosti tla u postotke


  lcd.setCursor(0, 0);
  lcd.print("Vlaz. tla je: ");  //estetski ispis
  
  lcd.setCursor(0, 1);
  lcd.print(postotak_vlaznosti_tla);  //ispis vlaznosti tla
  
  lcd.write("%");
  
}
//=========================================================================================================
void reset_lcd(){
  
  lcd.setCursor(0, 0);            //Brisanje prethodnog prikaza na LCD-u
  lcd.print("                ");  //Potrebno je jer se znakovi samo prekucavaju...
  
  lcd.setCursor(0, 1);            //Ako koristim u prvom slucaju 5 slova u prvom redu
  lcd.print("                ");      //A u drugom slucaju 4, ostane mi zadnje slovo od prvog slucaja
  lcd.write("                ");
  
  }

//=======================================================================================================
void air_controll(){
  Serial.println("air_controll");
  Serial.println(temperatura_zraka);
 
    if (temperatura_zraka < 26){                                                  
      Serial.println("cold");
      digitalWrite(pin_grijac, LOW);
      digitalWrite(pin_ventilator, HIGH);
      
      }else{
        
        digitalWrite(pin_grijac, HIGH);  
        digitalWrite(pin_ventilator, LOW);
        
        }                
      
    //kraj upravljanja komponentama ako je temperatura manja od zadane
    //pocetak upravljanja komponentama ako je temperatura veca od zadane
    if (temperatura_zraka > 30){  
      
      digitalWrite(pin_ventilator, HIGH); //ako je temperatura veca od 30 stupnjeva, pali ventilator
    }
    
    //kraj upravljanja komponentama ako je temperatura veca od zadane
   
  }

  //kraj logike za grijanje zraka
  
  
//===============================================================================================================
void soil_controll(){
  Serial.println("soil_controll");
  vlaznost_tla = analogRead(pin_vlaznosti_tla);
  postotak_vlaznosti_tla = 100 - ((vlaznost_tla / 1023) * 100);

  if((postotak_vlaznosti_tla - vlaznost_tla_idealno) > 5){  //+- 5% odstupanje
    digitalWrite(pin_ventilator, HIGH);
    Serial.println("odstupanje");
    }else if((postotak_vlaznosti_tla - vlaznost_tla_idealno) < -5) {  //ako je vlaznost tla manja od zadane gasi ventilator, pali pumpu
      digitalWrite(pin_ventilator, LOW);
      digitalWrite(pin_pumpa, LOW);//pali pumpu, log 0 treba dovest na pin releja
      Serial.println("manje odslupanje");
      }else if((postotak_vlaznosti_tla - vlaznost_tla_idealno) > 5){  //ako je vlaznsot tla veca od zadane pali ventilator, gasi pumpu
      digitalWrite(pin_ventilator, HIGH);
      Serial.println("171");
      digitalWrite(pin_pumpa, HIGH);//gasi pumpu, log1 treba dovesti na pin releja
  }
}
//===============================================================================================================
