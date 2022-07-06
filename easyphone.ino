#include <CapacitiveSensor.h>

#include <Servo.h>

Servo servo;
int angle = 10;
//Parameters
bool autocal  = 0;
const int numReadings  = 10;
long readings [numReadings];
int readIndex  = 0;
long total  = 0;
const int sensitivity  = 1000;
const int thresh  = 200;
const int csStep  = 10000;
CapacitiveSensor cs  = CapacitiveSensor(7, 6);

long int t1,t2;
//long int th = 500;
long int th = 2 * 1000;
int angle_max = 30;
int angle_min = angle_max + 90;

void setup() {
  // put your setup code here, to run once:

  t1 = -th;
  servo.attach(8);
  servo.write(angle_min);
  angle = angle_min;

  
  //Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  //Init cs
  if (autocal == 0) {
    {
      cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  int s = smooth();
  Serial.print(s);
  Serial.print(" ");
  Serial.println(t1);

  t2 = millis();

  if(s > 10 && (t2 - t1 > th )){  
    if(angle == angle_min){
      angle = angle_max;
    }else{
      angle = angle_min;
    }
    servo.write(angle);   
    t1 = millis(); 
  }
}


long smooth() { /* function smooth */
  ////Perform average on sensor readings
  long average;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the sensor:
  readings[readIndex] = cs.capacitiveSensor(sensitivity);
  // add value to total:
  total = total + readings[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;

  return average;
}
