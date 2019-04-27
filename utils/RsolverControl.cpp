#include "RsolverControl.h"
#include "RsolverHelpers.h"

namespace Rsolver {

	Rsolver::RsolverControl::RsolverControl(std::unique_ptr<IRsolverSerial> serial)
		: m_serial(std::move(serial))
	{
		m_robotCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::NoPending));
		m_robotCommand.arg1 = static_cast<unsigned char>(0xFF);
		m_robotCommand.arg2 = static_cast<unsigned char>(0xFF);

		m_serial->Initialize();
	}

	Rsolver::RsolverControl::~RsolverControl()
	{
	}

	std::vector<RobotCommand> RsolverControl::GetRobotCommandsFromSolution(const std::string cubeSolution)
	{
		auto tokens = Helpers::SplitStringBySpace(cubeSolution);
		return std::vector<RobotCommand>();
	}

	void Rsolver::RsolverControl::ExecuteRobotCommands(std::vector<RobotCommand> robotCommands)
	{
		for (const auto& cmd : robotCommands)
		{
			if (cmd.command == Helpers::ConvertControlCommand(ControlCommands::SetMotors))
			{
				SendSerialCommand(cmd);
			}
		}
	}

	bool RsolverControl::SendSerialCommand(RobotCommand cmd)
	{
		unsigned char message[] = { cmd.command, cmd.arg1, cmd.arg2 };
		m_serial->WriteToSerial(message, sizeof(message));
		return true;
	}

	RobotCommand RsolverControl::Extend(Motors motor)
	{
		m_robotCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::SetMotors));
		auto motorId = static_cast<int>(motor);
		if (motorId < 4)
		{
			m_robotCommand.arg1 = (0xFF ^ (1 << ((motorId * 2) + 1)));
		}
		else
		{
			m_robotCommand.arg2 = (0xFF ^ (1 << (((motorId - 4) * 2) + 1)));
		}
		return m_robotCommand;
	}

	RobotCommand RsolverControl::Neutral(Motors motor)
	{
		m_robotCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::SetMotors));
		auto motorId = static_cast<int>(motor);
		if (motorId < 4)
		{
			m_robotCommand.arg1 = (0xFF ^ (3 << (motorId * 2)));
		}
		else
		{
			m_robotCommand.arg2 = (0xFF ^ (3 << ((motorId - 4) * 2)));
		}
		return m_robotCommand;
	}

}
