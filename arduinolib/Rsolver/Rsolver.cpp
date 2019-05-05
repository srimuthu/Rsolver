#include "Rsolver.h"

Rsolver::Rsolver(int baud)
{
    m_baud = baud;
    m_msg[0] = NO_PENDING;
    m_defaultNoActionCommand = "XXXXXXXX";
    m_outputCommand = m_defaultNoActionCommand;
    m_cmdCount = 0;
}

void Rsolver::SetLed(int ledPin)
{
    m_ledPin = ledPin;
    pinMode(m_ledPin, OUTPUT);
}

String Rsolver::DecodeIncomingMessage()
{
    if(Serial.available()>=MSG_SIZE)
    {
        m_msg[0] = Serial.read();
        m_msg[1] = Serial.read();
        m_msg[2] = Serial.read();
    }
    return DoActionOnIncomingMessage();
}

String Rsolver::DoActionOnIncomingMessage()
{
    switch(m_msg[0])
    {
        case NO_PENDING:
            return m_defaultNoActionCommand;
        case BLINK_LED:
            BlinkLed(m_msg[1], m_msg[2]);
            m_msg[0] = NO_PENDING;
            AcknowledgeIncomingMessage();
            return m_defaultNoActionCommand;
        case SET_MOTORS:
            ConstructMotorCommand(m_msg[1], m_msg[2]);
            m_msg[0] = NO_PENDING;
            AcknowledgeIncomingMessage();
            return m_outputCommand;
        default:
            return m_defaultNoActionCommand;
    }
}

void Rsolver::AcknowledgeIncomingMessage()
{
    m_cmdCount++;
    Serial.println(m_cmdCount, DEC);
}

void Rsolver::ConstructMotorCommand(int grippers, int sliders)
{
    m_outputCommand[0] = GetCharForMotorCommand(grippers & 0x03);
    m_outputCommand[1] = GetCharForMotorCommand((grippers & 0x0C) >> 2);
    m_outputCommand[2] = GetCharForMotorCommand((grippers & 0x30) >> 4);
    m_outputCommand[3] = GetCharForMotorCommand((grippers & 0xC0) >> 6);

    m_outputCommand[4] = GetCharForMotorCommand(sliders & 0x03);
    m_outputCommand[5] = GetCharForMotorCommand((sliders & 0x0C) >> 2);
    m_outputCommand[6] = GetCharForMotorCommand((sliders & 0x30) >> 4);
    m_outputCommand[7] = GetCharForMotorCommand((sliders & 0xC0) >> 6);
}

char Rsolver::GetCharForMotorCommand(int command)
{
    switch(command)
    {
        case 0: return 'N';
        case 3: return 'E';
        default: return 'X';
    }
}

void Rsolver::BlinkLed(int numTimes, int timeDelay)
{
    for(int i=0;i<numTimes;i++)
    {
        digitalWrite(m_ledPin, HIGH);
        delay(timeDelay);
        digitalWrite(m_ledPin, LOW);
        delay(timeDelay);
    }
}
