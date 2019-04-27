#pragma once
#include "RsolverTypes.h"

namespace Rsolver
{
class IRsolverControl
{
public:
	virtual ~IRsolverControl() = default;

	virtual std::vector<RobotCommand> GetRobotCommandsFromSolution(const std::string cubeSolution) = 0;
	virtual void ExecuteRobotCommands(std::vector<RobotCommand> robotCommands) = 0;
	virtual bool SendSerialCommand(RobotCommand command) = 0;
};
}

