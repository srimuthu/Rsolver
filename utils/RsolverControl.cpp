#include "RsolverControl.h"
#include "RsolverHelpers.h"

namespace Rsolver {

	Rsolver::RsolverControl::RsolverControl(std::unique_ptr<IRsolverSerial> serial)
		: m_serial(std::move(serial))
		, m_responseCounter(0)
		, m_waitForAck(true)
	{
		m_robotCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::NoPending));
		m_robotCommand.arg1 = static_cast<unsigned char>(0xFF);
		m_robotCommand.arg2 = static_cast<unsigned char>(0xFF);
	}

	Rsolver::RsolverControl::~RsolverControl()
	{
	}

	void RsolverControl::InitializeRobot()
	{
		m_serial->Initialize();
		auto reply = m_serial->ReadLineFromSerial();
		if (reply != "Ready\r")
		{
			throw std::runtime_error("Robot returned unexpected reply");
		}
		ExecuteRobotCommands(GenerateLockCubeInPlaceCommands());
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
			if (!SendSerialCommand(cmd))
			{
				throw std::runtime_error( "Command response mismatch! aborting");
			}
		}
	}

	bool RsolverControl::SendSerialCommand(RobotCommand cmd)
	{
		unsigned char message[] = { cmd.command, cmd.arg1, cmd.arg2 };
		m_serial->WriteToSerial(message, sizeof(message));
		auto response = std::stoi(m_serial->ReadLineFromSerial().c_str());
		if (m_waitForAck)
		{
			if (response != m_responseCounter + 1)
			{
				return false;
			}
			m_responseCounter++;
		}
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

	void RsolverControl::ConfigureWaitForAcknowledge(bool waitForAck)
	{
		m_waitForAck = waitForAck;
	}

	std::vector<RobotCommand> RsolverControl::GenerateLockCubeInPlaceCommands()
	{
		std::vector<RobotCommand> lockCube;
		lockCube.emplace_back(Neutral(Motors::BottomGrip));
		lockCube.emplace_back(Neutral(Motors::RightGrip));
		lockCube.emplace_back(Neutral(Motors::LeftGrip));
		lockCube.emplace_back(Neutral(Motors::TopGrip));
		lockCube.emplace_back(Extend(Motors::BottomSlide));
		lockCube.emplace_back(Extend(Motors::RightSlide));
		lockCube.emplace_back(Extend(Motors::LeftSlide));
		lockCube.emplace_back(Extend(Motors::TopSlide));
		return lockCube;
	}

}
