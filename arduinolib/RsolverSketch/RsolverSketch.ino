
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Rsolver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define BAUD  9600

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
  comms.SetState(State::Operational);
  Serial.println("Ready");
}

void Extend(int motor)
{
  pwm.setPWM(motor, 0, comms.GetExtendedPosition(motor));
}

void Neutral(int motor)
{
  pwm.setPWM(motor, 0, comms.GetNeutralPosition(motor));
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
  delay(2000);
}

void loop() {
  command = comms.DecodeIncomingMessage();
  SetMotorStates(command);
}
