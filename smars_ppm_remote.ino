#include <PPMReader.h>
#include <Servo.h>
#include <Math.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

const int A1A = 8;//define pin 2 for A1A
const int A1B = 9;//define pin 3 for A1B

const int B1A = 10;//define pin 8 for B1A
const int B1B = 11;//define pin 9 for B1B

// Initialize a PPMReader on digital pin 3 with 6 expected channels.
int interruptPin = 3;
int channelAmount = 6;
PPMReader ppm(interruptPin, channelAmount);

int minReceiverValue = 1000;
int maxReceiverValue = 2000;

void setup() {
  myservo.attach(4);
  Serial.begin(9600);


  pinMode(B1A,OUTPUT);// define pin as output
  pinMode(B1B,OUTPUT);
  
  pinMode(A1A,OUTPUT);
  pinMode(A1B,OUTPUT);    


}

int calcRoundedPercent(float ppmValue, int precision)
{
  float p = 100/((float)maxReceiverValue-(float)minReceiverValue)*((float)maxReceiverValue-(float)ppmValue);
  p = (int) ((int)p / precision) * precision;
  return p;
}

void printChannelsData()
{
  // Print latest valid values from all channels
    for (int channel = 1; channel <= channelAmount; ++channel) {
        unsigned long value = ppm.latestValidChannelValue(channel, 0);
        Serial.print(String(value) + " ");
    }
}

void loop() {
  unsigned long value = ppm.latestValidChannelValue(5, 0);
  int deg = calcRoundedPercent(value, 5);

  value = ppm.latestValidChannelValue(2, 0);
  int spForward = calcRoundedPercent(value, 1);
  int spWheel = calcRoundedPercent(ppm.latestValidChannelValue(1, 0), 1);
  Serial.println(spForward + " " + spWheel);

  if (spForward >= 70) {
    moveForward();
  } else if (spForward<=30) {
    moveBackward();
  } else {
    stopMotors();
  }

  if (spWheel <=30 ) {
    motorA('L');
    motorB('L');
  } else if (spWheel >= 70) {
    motorA('R');
    motorB('R');
  }

  myservo.write(deg);  
  // printChannelsData();
  delay(50);
}

void moveForward() {
  motorA('R');
  motorB('L');
}

void moveBackward() {
  motorA('L');
  motorB('R');
}


void stopMotors() {
  motorA('S');
  motorB('S');
}
/*
 * @motorA
 * activation rotation of motor A
 * d is the direction
 * R = Right
 * L = Left
 */
void motorA(char d)
{
  if(d =='R'){
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,HIGH); 
  }else if (d =='L'){
    digitalWrite(A1A,HIGH);
    digitalWrite(A1B,LOW);    
  }else{
    //Robojax.com L9110 Motor Tutorial
    // Turn motor OFF
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,LOW);    
  }
}// motorA end


/*
 * @motorB
 * activation rotation of motor B
 * d is the direction
 * R = Right
 * L = Left
 */
void motorB(char d)
{

    if(d =='R'){
      digitalWrite(B1A,LOW);
      digitalWrite(B1B,HIGH); 
    }else if(d =='L'){
      digitalWrite(B1A,HIGH);
      digitalWrite(B1B,LOW);    
    }else{
    //Robojax.com L9110 Motor Tutorial
    // Turn motor OFF      
      digitalWrite(B1A,LOW);
      digitalWrite(B1B,LOW);     
    }

}// motorB end
