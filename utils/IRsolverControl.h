#pragma once
#include "RsolverTypes.h"

namespace Rsolver
{
class IRsolverControl
{
public:
	virtual ~IRsolverControl() = default;

	virtual void InitializeRobot() = 0;
	virtual std::vector<RobotCommand> GetRobotCommandsFromSolution(const std::string cubeSolution) = 0;
	virtual void ExecuteRobotCommands(std::vector<RobotCommand> robotCommands) = 0;
};
}

