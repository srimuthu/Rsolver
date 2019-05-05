
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Rsolver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define BAUD  9600

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define NUMMOTORS   8

#define BOTTOMGRIP  0
#define RIGHTGRIP   1
#define LEFTGRIP    2
#define TOPGRIP     3

#define BOTTOMSLIDE 4
#define RIGHTSLIDE  5     
#define LEFTSLIDE   6
#define TOPSLIDE    7

int neutralPos[8]   = {240, 240, 220, 230, 430, 430, 430, 470};
int extendedPos[8]  = {445, 455, 420, 445, 200, 180, 220, 200};

Rsolver comms(BAUD);
String command;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD);
  comms.SetLed(LED_BUILTIN);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(1000);
  PowerOnSelfTest();
  Serial.println("Ready");
}

void Extend(int motor)
{
  pwm.setPWM(motor, 0, extendedPos[motor]);
}

void Neutral(int motor)
{
  pwm.setPWM(motor, 0, neutralPos[motor]);
}

void SetMotorStates(String stateArray)
{
  for(int i=0; i<NUMMOTORS; i++)
  {
    if(stateArray.charAt(i)=='X') continue;
    else if(stateArray.charAt(i)=='N') Neutral(i);
    else if(stateArray.charAt(i)=='E') Extend(i);
  }
  delay(750);
}

void PowerOnSelfTest()
{
  SetMotorStates("XXXXNNNN"); // NEUTRAL ALL SLIDERS
  SetMotorStates("NNNNXXXX"); // NEUTRAL ALL GRIPPERS
  SetMotorStates("EEEEXXXX"); // EXTEND ALL GRIPPERS
  SetMotorStates("XXXXEEEE"); // EXTEND ALL SLIDERS
  SetMotorStates("XXXXNNNN"); // NEUTRAL ALL SLIDERS
  SetMotorStates("NNNNXXXX"); // NEUTRAL ALL GRIPPERS
}

void loop() {
  command = comms.DecodeIncomingMessage();
  SetMotorStates(command);
}
