#include "Rsolver.h"

Rsolver::Rsolver(int baud)
: m_neutralPositions{238, 250, 218, 230, 430, 430, 430, 470}
, m_extendedPositions{445, 465, 425, 445, 210, 206, 236, 245}
, m_state(State::Startup)
{
    m_baud = baud;
    m_msg[0] = NO_PENDING;
    m_defaultNoActionCommand = "XXXXXXXX";
    m_outputCommand = m_defaultNoActionCommand;
    m_defaultNeutralAllCommand = "NNNNNNNN";
    m_cmdCount = 0;
}

void Rsolver::SetLed(int ledPin)
{
    m_ledPin = ledPin;
    pinMode(m_ledPin, OUTPUT);
}

bool Rsolver::SetState(State state)
{
    if(m_state != State::Error)
    {
        m_state = state;
        return true;
    }
    return false;
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

int Rsolver::GetNeutralPosition(int motor)
{
    return m_neutralPositions[motor];
}

int Rsolver::GetExtendedPosition(int motor)
{
    return m_extendedPositions[motor];
}

void Rsolver::SetNeutralPosition(int motor, int position)
{
    m_neutralPositions[motor] = position;
}

void Rsolver::SetExtendedPosition(int motor, int position)
{
    m_extendedPositions[motor] = position;
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
        case ENTER_CALIBRATION_MODE:
            m_state = State::Calibrating;
            m_msg[0] = NO_PENDING;
            m_outputCommand = m_defaultNeutralAllCommand;
            return m_defaultNeutralAllCommand;
        case CALIBRATION_MODE:
            m_msg[0] = NO_PENDING;
            if(m_state == State::Calibrating)
            {
                ConstructCalibrationCommand(m_msg[1], m_msg[2]);
                return m_outputCommand;
            }
            return m_defaultNoActionCommand;
        case EXIT_CALIBRATION_MODE:
            m_state = State::Operational;
            m_msg[0] = NO_PENDING;
            m_outputCommand = m_defaultNeutralAllCommand;
            SendCurrentPositionValues();
            return m_defaultNeutralAllCommand;
        default:
            return m_defaultNoActionCommand;
    }
}

void Rsolver::SendCurrentPositionValues()
{
    for(int i=0; i<NUMMOTORS; i++)
    {
        Serial.println(m_neutralPositions[i], DEC);
        Serial.println(m_extendedPositions[i], DEC);
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

void Rsolver::ConstructCalibrationCommand(int byte1, int byte2)
{
    int motor = (byte1 & 0xF0) >> 4;
    Position position = (byte1 & 0x0F)? Position::Extend : Position::Neutral;
    Direction direction = (byte2 & 0x80) ? Direction::Decrement : Direction::Increment;
    int value = byte2 & 0x7F;
    if(direction == Direction::Decrement)
    {
        value = value * -1;
    }
    if(position == Position::Extend)
    {
        m_extendedPositions[motor] += value;
        m_outputCommand[motor] = 'E';
    }
    else
    {
        m_neutralPositions[motor] += value;
        m_outputCommand[motor] = 'N';
    }
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
