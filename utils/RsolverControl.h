#pragma once
#include <memory>
#include "IRsolverControl.h"
#include "IRsolverSerial.h"

namespace Rsolver {

class RsolverControl : public IRsolverControl
{
public:
	explicit RsolverControl(std::unique_ptr<IRsolverSerial> serial);
	~RsolverControl();

	virtual void InitializeRobot() override;
	virtual std::vector<RobotCommand> GetRobotCommandsFromSolution(const std::string cubeSolution) override;
	virtual void ExecuteRobotCommands(std::vector<RobotCommand> robotCommands) override;


	// Non-interface public methods (maybe move to another interface)
	RobotCommand Extend(Motors motor);
	RobotCommand Neutral(Motors motor);
	void ConfigureWaitForAcknowledge(bool waitForAck);

private:

	bool SendSerialCommand(RobotCommand command);
	std::vector<RobotCommand> GenerateLockCubeInPlaceCommands();

	std::unique_ptr<IRsolverSerial>		m_serial;
	RobotCommand						m_robotCommand;
	int									m_responseCounter;
	bool								m_waitForAck;

};

}