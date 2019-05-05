# -------------------------------------------------------------------------------
# Name:        ArduinoComm.py
# Purpose:      Contains definitions for messaging conventions between
#               Pi and Arduino
#
# Author:      Sri Muthu Narayanan Balasubramanian
#
# Created:     17-05-2016
# Copyright:   (c) User 2016
# Licence:     <your licence>
# -------------------------------------------------------------------------------
import sys, time, os
import serial

msgLen = 3
serPort = "COM3"
cmdDict = {"NO_PENDING": 0xfe,
           "BLINK_LED": 0x01,
           "SET_MOTORS": 0x02}

motorDict = {"BGRIP": 0,
             "RGRIP": 1,
             "LGRIP": 2,
             "TGRIP": 3,
             "BSLIDE": 4,
             "RSLIDE": 5,
             "LSLIDE": 6,
             "TSLIDE": 7}

class ArduinoComm():
    def __init__(self, port=serPort, baud=9600):
        self.__InitSuccess = False
        try:
            self.__ser = serial.Serial(str(port),baud)
            print("Serial port opened successfully")
            self.__InitSuccess = True
            time.sleep(2)

        except:
            sys.exit("Error initializing serial port")

        self.__cmd = chr(cmdDict["NO_PENDING"])
        self.__msg = [self.__cmd,chr(0x00),chr(0x00)]

    def __del__(self):
        self.__ser.close()
        print("Serial port closed")

    def Terminate(self):
        self.__del__()


    def __TransmitMessage(self):

        for i in range(0,msgLen):
            self.__ser.write(self.__msg[i])
        print("Transmitting : ", self.__msg)

    def SendMessage(self, cmd, args):
        if cmd not in cmdDict:
            print("Invalid command provided - ignoring transmission")
            return False

        self.__cmd =chr(cmdDict[cmd])
        self.__msg[0] = self.__cmd
        for i in range(1,msgLen):
            self.__msg[i] = chr(args[i-1])

        self.__TransmitMessage()

    def ReceiveMessage(self):
        self.__incomingMsg = ""
        if self.__ser.inWaiting() > 0:
            self.__incomingMsg = self.__ser.read(self.__ser.inWaiting())
            return self.__incomingMsg
        else:
            return ""


class RobotControl(ArduinoComm):

    def __init__(self):
        self.arduinoComm = ArduinoComm("COM3", 9600)
        self.gripperCmd = 0x00
        self.sliderCmd = 0x00

    def __del__(self):
        self.arduinoComm.Terminate()

    def __extend(self, motor):
        if motor not in motorDict:
            print("Invalid motor")
            return False
        motorIdx = motorDict[motor]
        if(motorIdx < 4):
            self.gripperCmd = (self.gripperCmd | (3 << (motorIdx*2)))
        else:
            self.sliderCmd = (self.sliderCmd | (3 << ((motorIdx-4)*2)))
        self.TransmitCommand()

    def __neutral(self, motor):
        if motor not in motorDict:
            print("Invalid motor")
            return False
        motorIdx = motorDict[motor]
        if(motorIdx < 4):
            self.gripperCmd = self.gripperCmd & (0xFF ^ (3 << (motorIdx * 2)))
        else:
            self.sliderCmd = self.sliderCmd & (0xFF ^ (3 << ((motorIdx - 4) * 2)))
        self.TransmitCommand()

    def TransmitCommand(self):
        self.arduinoComm.SendMessage("SET_MOTORS", [self.gripperCmd, self.sliderCmd])

    def LockCubeInPlace(self):
        self.__neutral("BGRIP")
        self.__neutral("RGRIP")
        self.__neutral("LGRIP")
        self.__neutral("TGRIP")
        self.__extend("BSLIDE")
        self.__extend("RSLIDE")
        self.__extend("LSLIDE")
        self.__extend("TSLIDE")

    def __displayMotorChoices(self):
        print("0. Bgrip \t 1. Rgrip \t 2. Lgrip \t 3. Tgrip \
              \t 4. Bslide \t 5. Rslide \t 6. Lslide \t 7. Tslide")

    def __displayOperationChoices(self):
        print("1. Neutral \t 2. Extend \t 3. No Change \t 4. Exit")

    def __getMotorForNumber(self, number):
        for motor in motorDict:
            if(motorDict[motor] == number):
                return motor

    def __interactiveControl(self):
        self.__displayMotorChoices()
        motorChoice = int(input())
        self.__displayOperationChoices()
        operationChoice = int(input())
        if(motorChoice>7 or operationChoice>4): return -1
        if(operationChoice == 1):
            self.__neutral(self.__getMotorForNumber(motorChoice))
        elif(operationChoice == 2):
            self.__extend(self.__getMotorForNumber(motorChoice))
        elif(operationChoice == 3):
            return -1
        elif (operationChoice == 4):
            return 0
        return -1

    def DisplayUI(self):
        breakloop = -1
        while breakloop != 0:
            breakloop = self.__interactiveControl()


# testing purposes
if __name__ == '__main__':

    rcontrol = RobotControl()
    time.sleep(5)
    rcontrol.LockCubeInPlace()
    rcontrol.DisplayUI()
