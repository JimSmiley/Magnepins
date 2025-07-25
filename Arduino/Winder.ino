#include <AccelStepper.h>
int  count;
int oldCount;
int revolution;
int revsPerWind = 630;
bool dir;
const int dirPin = 13;
const int stepPin = 12;
//const int enablePin = 0;
bool atEnd;
int motorSpeed;
int passes;
// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  
  
  Serial.begin(115200);
 //pinMode(enablePin, OUTPUT);
 pinMode(11, OUTPUT);
 pinMode(10, INPUT); // set to INPUT for HIGH impedence on L1 and L2 = 0.5a current limit. change to output and 
 pinMode(9, INPUT);
  pinMode(8, OUTPUT);
 pinMode(7, INPUT);
 pinMode(6, INPUT);
   pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);

 digitalWrite(11, HIGH); // slp
digitalWrite(4, HIGH);
digitalWrite(3, LOW);
//digitalWrite (5, HIGH);
digitalWrite(8, HIGH); // QUARTER STEP. Low for full step
 

  myStepper.setMaxSpeed(10000);
  myStepper.setAcceleration(10000);
  myStepper.setSpeed(10000);
//  digitalWrite(enablePin, LOW);
 attachInterrupt(digitalPinToInterrupt(7), Interrupt, RISING);
}
//One step = 0.125mm so 50 steps for .625mm. 200 for 2.5

void loop() {
motorSpeed = analogRead(A4);
int scaleVal = map(motorSpeed, 0, 1023, 0, 255);
analogWrite(5, scaleVal);
  if (count != oldCount)
  {
   // Serial.println(count);
    oldCount = count;
    if (count >=20)
    {
      count = 0;
      
      
      if (revolution > revsPerWind)
      {
        passes ++;
        revolution = 0;
        dir = !dir;
        Serial.println (passes);
      }
      moveStepper(dir);
      revolution ++;
    }
  }
  
myStepper.run();

   
   }
void moveStepper(bool up)
{
  if (up)
  {
  myStepper.moveTo(myStepper.currentPosition()+1);
  }
  else
  {
    myStepper.moveTo(myStepper.currentPosition()-1);
  }
  
}
   void Interrupt()
{
 if (digitalRead(6) ==HIGH)
 {
  count --;
 }

 else
 {
  count++;
 }
}
 

  
 
