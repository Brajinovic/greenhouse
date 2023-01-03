int relay = 3;

void setup(){
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  }

void loop(){
  digitalWrite(relay, HIGH);
  Serial.println("HIGH");
  delay(5000);
  digitalWrite(relay, LOW);
  Serial.println("LOW");
  delay(5000);
  }
