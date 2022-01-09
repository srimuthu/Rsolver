#ifndef Rsolver_h
#define Rsolver_h

#include "Arduino.h"

#define MSG_SIZE        3

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

// Commands
#define BLINK_LED   0x01
#define SET_MOTORS  0x02
#define ENTER_CALIBRATION_MODE  0x03
#define CALIBRATION_MODE        0x04
#define EXIT_CALIBRATION_MODE   0x05

// Internal command
#define NO_PENDING  0xFE

enum class State
{
    Startup,
    Operational,
    Calibrating,
    Error
};

enum class Position
{
    Neutral,
    Extend
};

enum class Direction
{
    Increment,
    Decrement
};

class Rsolver
{
    public:
        Rsolver(int baud);

        void SetLed(int ledPin);
        String DecodeIncomingMessage(void);
        int GetNeutralPosition(int motor);
        int GetExtendedPosition(int motor);
        void SetNeutralPosition(int motor, int position);
        void SetExtendedPosition(int motor, int position);
        bool SetState(State state);

    private:

        String DoActionOnIncomingMessage();
        void AcknowledgeIncomingMessage(void);

        void ConstructMotorCommand(int grippers, int sliders);
        void ConstructCalibrationCommand(int byte1, int byte2);
        char GetCharForMotorCommand(int command);
        void BlinkLed(int times, int delay);

        void SendCurrentPositionValues();

        int m_baud;
        int m_ledPin;
        int m_msg[MSG_SIZE];
        int m_cmdCount;
        String m_defaultNoActionCommand;
        String m_defaultNeutralAllCommand;
        String m_outputCommand;
        int m_neutralPositions[NUMMOTORS];
        int m_extendedPositions[NUMMOTORS];
        State m_state;

};

#endif
