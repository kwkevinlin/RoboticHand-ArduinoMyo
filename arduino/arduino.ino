#include <Servo.h>

Servo servo;
const int pinServo = 2;
unsigned int angle;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	servo.attach(pinServo);

	servo.write(0);
		
}

void loop() {
    // put your main code here, to run repeatedly:
    if(Serial.available()>0) {	
        
        angle = Serial.read();

		if(angle <= 179) {
			servo.write(angle);
		}
        
	}
}
