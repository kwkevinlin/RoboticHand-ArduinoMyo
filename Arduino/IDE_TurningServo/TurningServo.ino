#include<Servo.h>

Servo servo1;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  servo1.attach(9);
  Serial.println("Controlled by Serial Input. Type in dash and number 0~180, eg. -0, -90, -180");
  int input;
  for(;;)
  {
    if(Serial.available())
    {
      input = Serial.read();
      input=Serial.parseInt();
      Serial.println(input);
      servo1.write(input);
    }
    delay(20);
  }
}
