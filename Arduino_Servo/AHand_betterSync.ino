#include<Servo.h>

Servo Thumb, Index, Middle, Ring, Pinky;
//char specifier;
//int input;
char t,i,m,r,p;
int tt = 90;
int ii = 90;
int mm = 90;
int rr = 90;
int pp = 90;

void setup() {
  Serial.begin(9600);
  Thumb.attach(5);
  Index.attach(6);
  Middle.attach(9);
  Ring.attach(10);
  Pinky.attach(11);
}  

void loop() {
  if(Serial.available())
  {  
    t = Serial.read();
    tt = Serial.parseInt();
    i = Serial.read();
    ii = Serial.parseInt();
    m = Serial.read();
    mm = Serial.parseInt();
    r = Serial.read();
    rr = Serial.parseInt();
    p = Serial.read();
    pp = Serial.parseInt();

    Serial.println(tt);
    Serial.println(" ");
    Serial.println(ii);
    Serial.println(" ");
    Serial.println(mm);
    Serial.println(" ");
    Serial.println(rr);
    Serial.println(" ");
    Serial.println(pp);

//    Thumb.write(tt);
//    Index.write(ii);
//    Middle.write(mm);
//    Ring.write(rr);
//    Pinky.write(pp);
    
//      specifier = Serial.read();
//      input = Serial.parseInt();
//      if(specifier=='T'||specifier=='t')
//      { 
//        Serial.println("Thumb");
//        Thumb.write(input);   
//      }
//      else if(specifier=='I'||specifier=='i')
//      { 
//        Serial.println("Index");
//        Index.write(input);
//      }
//      else if(specifier=='M'||specifier=='m')
//      {
//        Serial.println("Middle");
//        Middle.write(input);
//      }
//      else if(specifier=='R'||specifier=='r')
//      {
//        Serial.println("Ring");
//        Ring.write(input);
//      }
//      else if(specifier=='P'||specifier=='p')
//      {
//        Serial.println("Pinky");
//        Pinky.write(input);
//      }
  }
  Thumb.write(tt);
  Index.write(ii);
  Middle.write(mm);
  Ring.write(rr);
  Pinky.write(180-pp);
  delay(20); 
}
