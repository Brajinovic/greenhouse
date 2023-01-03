#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <dht.h> 

dht DHT;
#define DHT11_PIN 7                      
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2); 
   
float air_temperature = 0;
float air_humidity = 0;
int pin_soil_moisture = A1; 
float soil_moisture;
float soil_mositure_percentage;     
int pin_heater = 8;
int pin_fan = 9;
unsigned long current_time = 0;
unsigned long previous_time = 0;
int interval = 5000;
int soil_mosture_ref = 10;//in percentage
int pin_water_pump = 10;

void setup() {
  Serial.begin(9600); 
  pinMode(pin_air_humidity, INPUT);
  pinMode(pin_heater, OUTPUT);
  pinMode(pin_fan, OUTPUT);
  pinMode(pin_water_pump, OUTPUT);
  pinMode(7, INPUT);  //dht-11
  lcd.init();
  lcd.backlight();
  digitalWrite(pin_heater, HIGH);
  digitalWrite(pin_pump, HIGH);
  digitalWrite(pin_fan, LOW);
}

//============================================================================================================================
void pause(){
  previous_time = current_time;
  while (current_time - previous_time <= interval){ 
    //Serial.println("vlaznost");
    current_time = millis();  
    }
  }

  
void loop() {
  //=================================================================================================================================
  printAirTemperature();
  pause();              
  //delay(10000);
  reset_lcd();          
  
  printAirHumidity();
  pauza();             
  
  reset_lcd();
  
  printSoilMoisture();
  pauza();

  air_control();     
  pauza();
  digitalWrite(pin_grijac, HIGH);
  digitalWrite(pin_ventilator, LOW);
  soil_control();      
  pauSe();
  digitalWrite(pin_pumpa, HIGH);
  digitalWrite(pin_ventilator, LOW);
  }
  
//==========================================================================================================

void printAirTemperature(){
  
  int dht = DHT.read11(DHT11_PIN);
  air_temperature = DHT.temperature;
  //Serial.print("hello");
  lcd.setCursor(0,0);
  lcd.print("Temp. zraka je: ");
  
  lcd.setCursor(0, 1);
  lcd.print(temperatura_zraka);
  lcd.write(223); //character °
  lcd.write("C");
  
  }
  
//====================================================================================================

 void printAirHumidity(){
  
  air_humidity = DHT.humidity;
  
  lcd.setCursor(0, 0);
  lcd.print("Vlaz. zraka je: ");
  lcd.setCursor(0, 1);
  lcd.print(vlaznost_zraka);
  lcd.write("%");
  
  }
  
//======================================================================================================

void soil_management(){
  
  soil_moisture = analogRead(pin_soil_mositure);
  soil_mositure_percentage = 100 - ((soil_moisture / 1023) * 100);


  lcd.setCursor(0, 0);
  lcd.print("Vlaz. tla je: ");
  
  lcd.setCursor(0, 1);
  lcd.print(postotak_vlaznosti_tla);
  
  lcd.write("%");
  
}
//=========================================================================================================
void reset_lcd(){
  //for preventing leftover characters on the lcd
  lcd.setCursor(0, 0);            
  lcd.print("                ");  
  
  lcd.setCursor(0, 1);            
  lcd.print("                ");      
  lcd.write("                ");
  
  }

//=======================================================================================================
void air_control(){
  //Serial.println("air_controll");
  Serial.println(air_temperature);
 
    if (air_temperature < 26){                                                  
      //Serial.println("cold");
      digitalWrite(pin_grijac, LOW);
      digitalWrite(pin_ventilator, HIGH);
      
      }else{
        
        digitalWrite(pin_grijac, HIGH);  
        digitalWrite(pin_ventilator, LOW);
        
        }                
      
    if (air_temperature > 30){  
      
      digitalWrite(pin_ventilator, HIGH);
    }
    
  }
//===============================================================================================================
void soil_control(){
  //Serial.println("soil_controll");
  soil_moisture = analogRead(pin_soil_moisture);
  soil_moisture_percentage = 100 - ((soil_moisture / 1023) * 100);

  if((soil_moisture_percentage - soil_mositure_ref) > 5){  //+- 5% tolerance
    digitalWrite(pin_ventilator, HIGH);
    //Serial.println("odstupanje");
    }else if((soil_moisture_percentage - soil_mositure_ref) < -5) {  //soil moisture too low
      digitalWrite(pin_ventilator, LOW);                              
      digitalWrite(pin_water_pump, LOW);                               
      //Serial.println("manje odslupanje");
      }else if((soil_moisture_percentage - soil_mositure_ref) > 5){  //soil moisture too high
      digitalWrite(pin_ventilator, HIGH);                             
      //Serial.println("171");
      digitalWrite(pin_water_pump, HIGH);                                  
  }
}
//===============================================================================================================
