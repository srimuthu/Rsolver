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

	virtual std::vector<RobotCommand> GetRobotCommandsFromSolution(const std::string cubeSolution) override;
	virtual void ExecuteRobotCommands(std::vector<RobotCommand> robotCommands) override;
	virtual bool SendSerialCommand(RobotCommand command) override;

	// Non-interface public methods (maybe move to another interface)
	RobotCommand Extend(Motors motor);
	RobotCommand Neutral(Motors motor);

private:

	std::unique_ptr<IRsolverSerial>		m_serial;
	RobotCommand						m_robotCommand;

};

}