###########################
Kewin Lima

26/11/2018
###########################

// definicao de pinos
const int pin_step = D5; 
const int pin_dir  = D6; 
const int pin_step1 = D1; 
const int pin_dir1  = D2; 
 
void setup() {
  // configuracao dos pinos
  pinMode(pin_step,OUTPUT); 
  pinMode(pin_dir, OUTPUT);
  pinMode(pin_step1,OUTPUT); 
  pinMode(pin_dir1, OUTPUT);

}
void loop() {
  digitalWrite(pin_dir,HIGH); 
  //200 pulsos para uma volta completa em fullstep no Nema 17
  for(int i=0;i<200;i++) {
    digitalWrite(pin_step,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(pin_step,LOW); 
    delayMicroseconds(500); 
  }
  digitalWrite(pin_dir1,HIGH); 
  //200 pulsos para uma volta completa em fullstep no Nema 17
  for(int i=0;i<200;i++) {
    digitalWrite(pin_step1,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(pin_step1,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000);
  
  digitalWrite(pin_dir,LOW);
  for(int i=0;i<200;i++) {
    digitalWrite(pin_step,HIGH);
    delayMicroseconds(500);
    digitalWrite(pin_step,LOW);
    delayMicroseconds(500);
  }
  digitalWrite(pin_dir1,LOW);
  for(int i=0;i<200;i++) {
    digitalWrite(pin_step1,HIGH);
    delayMicroseconds(500);
    digitalWrite(pin_step1,LOW);
    delayMicroseconds(500);
      }
    delay(1000);
 }
