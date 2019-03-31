#ifndef Rsolver_h
#define Rsolver_h

#include "Arduino.h"

#define MSG_SIZE        3
// Commands
#define BLINK_LED   0x01
#define SET_MOTORS  0x02

// Internal command
#define NO_PENDING  0xFE

class Rsolver
{
    public:
        Rsolver(int baud);

        void SetLed(int ledPin);
        String DecodeIncomingMessage(void);

    private:

        String DoActionOnIncomingMessage();

        void ConstructMotorCommand(int grippers, int sliders);
        char GetCharForMotorCommand(int command);
        void BlinkLed(int times, int delay);

        int m_baud;
        int m_ledPin;
        int m_msg[MSG_SIZE];
        String m_defaultNoActionCommand;
        String m_outputCommand;

};

#endif
