/*
Ponavljanje operacija u određenom vremenskom periodu.
*/

// Konstanta se ne mjenja. Ovdje se postavlja broj pina:
const int ledPin =  LED_BUILTIN;

// Varijabla se mjenja:
int ledState = LOW;             // početno stanje varijable

unsigned long previousMillis = 0;        // početno stanje timera

const long interval = 1000;           // interval u kome će se radnje ponavljati (millisekunde)

void setup() {
 
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
  }
}
