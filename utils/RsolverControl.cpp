#include "RsolverControl.h"
#include "RsolverHelpers.h"

namespace Rsolver {

	Rsolver::RsolverControl::RsolverControl(std::unique_ptr<IRsolverSerial> serial)
		: m_serial(std::move(serial))
		, m_responseCounter(0)
		, m_waitForAck(true)
	{
		m_robotMoveCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::SetMotors));
		m_robotMoveCommand.arg1 = static_cast<unsigned char>(0x00);
		m_robotMoveCommand.arg2 = static_cast<unsigned char>(0x00);
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
	}

	std::vector<RobotCommand> RsolverControl::GetRobotCommandsFromSolution(const std::string cubeSolution)
	{
		std::vector<RobotCommand> commands;
		auto tokens = Helpers::SplitStringBySpace(cubeSolution);
		for (const auto &token : tokens)
		{
			auto moveInfo = GetMoveInfoFromStringToken(token);
			GenerateCommandsForMoveInfo(commands, moveInfo);
		}
		return commands;
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

	void RsolverControl::GenerateCommandsForMoveInfo(std::vector<RobotCommand>& commands, const MoveInfo & moveInfo)
	{
		auto startPos = GenerateStartPositionForCubeFaceMove(moveInfo);
		commands.insert(commands.end(), startPos.begin(), startPos.end());
		auto performMove = GenerateCommandsForCubeFaceMove(moveInfo);
		commands.insert(commands.end(), performMove.begin(), performMove.end());
		auto endPos = GenerateEndPositionForCubeFaceMove(moveInfo);
		commands.insert(commands.end(), endPos.begin(), endPos.end());
	}

	std::vector<RobotCommand> RsolverControl::GenerateStartPositionForCubeFaceMove(const MoveInfo & moveInfo)
	{
		std::vector<RobotCommand> startPosCommands;
		switch (moveInfo.face)
		{
		case CubeFaces::Up:
		case CubeFaces::Down:
			break;
		case CubeFaces::Right:
		case CubeFaces::Left:
			startPosCommands.emplace_back(Neutral({ Motors::TopSlide,  Motors::BottomSlide }));
			startPosCommands.emplace_back(Extend({ Motors::TopGrip, Motors::BottomGrip }));
			startPosCommands.emplace_back(Extend({ Motors::TopSlide, Motors::BottomSlide }));
			break;
		case CubeFaces::Front:
		case CubeFaces::Back:
			startPosCommands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			startPosCommands.emplace_back(Extend(Motors::BottomGrip));
			startPosCommands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			startPosCommands.emplace_back(Neutral(Motors::BottomSlide));
			startPosCommands.emplace_back(Neutral(Motors::BottomGrip));
			startPosCommands.emplace_back(Extend(Motors::BottomSlide));
			startPosCommands.emplace_back(Neutral({ Motors::TopSlide,  Motors::BottomSlide }));
			startPosCommands.emplace_back(Extend({ Motors::TopGrip, Motors::BottomGrip }));
			startPosCommands.emplace_back(Extend({ Motors::TopSlide, Motors::BottomSlide }));
			break;
		default:
			throw std::runtime_error("Invalid MoveInfo Face");
		}
		return startPosCommands;
	}

	std::vector<RobotCommand> RsolverControl::GenerateCommandsForCubeFaceMove(const MoveInfo & moveInfo)
	{
		std::vector<RobotCommand> commands;
		PerformCubeFaceRotations(commands, moveInfo);
		return commands;
	}

	std::vector<RobotCommand> RsolverControl::GenerateEndPositionForCubeFaceMove(const MoveInfo & moveInfo)
	{
		std::vector<RobotCommand> endPosCommands;
		switch (moveInfo.face)
		{
		case CubeFaces::Up:
		case CubeFaces::Down:
			break;
		case CubeFaces::Right:
		case CubeFaces::Left:
			endPosCommands.emplace_back(Neutral({ Motors::TopSlide,  Motors::BottomSlide }));
			endPosCommands.emplace_back(Neutral({ Motors::TopGrip, Motors::BottomGrip }));
			endPosCommands.emplace_back(Extend({ Motors::TopSlide, Motors::BottomSlide }));
			break;
		case CubeFaces::Front:
		case CubeFaces::Back:
			endPosCommands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			endPosCommands.emplace_back(Neutral(Motors::BottomGrip));
			endPosCommands.emplace_back(Neutral({ Motors::TopGrip, Motors::LeftGrip, Motors::RightGrip }));
			endPosCommands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			break;
		default:
			throw std::runtime_error("Invalid MoveInfo Face");
		}
		return endPosCommands;
	}

	void RsolverControl::PerformCubeFaceRotations(std::vector<RobotCommand>& commands, const MoveInfo & moveInfo)
	{
		for (auto i = 0; i < moveInfo.times; i++)
		{
			PerformOneCubeFaceRotation(commands, moveInfo.face);
		}
	}

	void RsolverControl::PerformOneCubeFaceRotation(std::vector<RobotCommand>& commands, CubeFaces cubeFace)
	{
		commands.emplace_back(Extend(Helpers::GetGripperForCubeFace(cubeFace)));
		commands.emplace_back(Neutral(Helpers::GetSliderForCubeFace(cubeFace)));
		commands.emplace_back(Neutral(Helpers::GetGripperForCubeFace(cubeFace)));
		commands.emplace_back(Extend(Helpers::GetSliderForCubeFace(cubeFace)));
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
		m_robotMoveCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::SetMotors));
		auto motorId = static_cast<int>(motor);
		if (motorId < 4)
		{
			m_robotMoveCommand.arg1 = (m_robotMoveCommand.arg1 | (3 << ((motorId * 2))));
		}
		else
		{
			m_robotMoveCommand.arg2 = (m_robotMoveCommand.arg2 | (3 << (((motorId - 4) * 2))));
		}
		return m_robotMoveCommand;
	}

	RobotCommand RsolverControl::Extend(std::vector<Motors> motors)
	{
		// Simulataneously extend all motos in the vector
		RobotCommand command;
		for (const auto &motor : motors)
		{
			command = Extend(motor);
		}
		return command;
	}

	RobotCommand RsolverControl::Neutral(Motors motor)
	{
		m_robotMoveCommand.command = static_cast<unsigned char>(Helpers::ConvertControlCommand(ControlCommands::SetMotors));
		auto motorId = static_cast<int>(motor);
		if (motorId < 4)
		{
			unsigned char mask = 0xFF ^ (3 << (motorId * 2));
			m_robotMoveCommand.arg1 = (m_robotMoveCommand.arg1 & (mask));
		}
		else
		{
			unsigned char mask = 0xFF ^ (3 << ((motorId - 4) * 2));
			m_robotMoveCommand.arg2 = (m_robotMoveCommand.arg2 & (mask));
		}
		return m_robotMoveCommand;
	}

	RobotCommand RsolverControl::Neutral(std::vector<Motors> motors)
	{
		// Simulataneously neutral all motos in the vector
		RobotCommand command;
		for (const auto &motor : motors)
		{
			command = Neutral(motor);
		}
		return command;
	}

	void RsolverControl::ConfigureWaitForAcknowledge(bool waitForAck)
	{
		m_waitForAck = waitForAck;
	}

	MoveInfo RsolverControl::GetMoveInfoFromStringToken(const std::string & moveString)
	{
		MoveInfo moveInfo;
		moveInfo.face = Helpers::GetCubeFaceForLetter(moveString.at(0));
		moveInfo.times = std::stoi(std::string(1, moveString.at(1)));
		return moveInfo;
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

	std::vector<RobotCommand> RsolverControl::GeneratePrepareForCapture(CubeFaces face)
	{
		std::vector<RobotCommand> commands;
		switch (face)
		{
		case CubeFaces::Up:
			commands.emplace_back(Neutral({Motors::BottomSlide}));
			commands.emplace_back(Extend({Motors::BottomGrip}));
			commands.emplace_back(Extend({Motors::BottomSlide}));
			commands.emplace_back(Neutral({Motors::TopSlide, Motors::RightSlide}));
			commands.emplace_back(Extend({Motors::RightGrip}));
			commands.emplace_back(Extend({Motors::RightSlide}));
			commands.emplace_back(Neutral({Motors::BottomSlide}));
			Neutral({ Motors::RightGrip });
			commands.emplace_back(Extend({Motors::LeftGrip}));
			commands.emplace_back(Extend({Motors::BottomSlide}));
			commands.emplace_back(Neutral({Motors::RightSlide}));
			commands.emplace_back(Extend({Motors::RightGrip}));
			commands.emplace_back(Extend({Motors::RightSlide}));
			commands.emplace_back(Neutral({Motors::BottomSlide}));
			return commands;
		case CubeFaces::Front:
			commands.emplace_back(Neutral({ Motors::LeftSlide, Motors::RightSlide }));
			return commands;
		case CubeFaces::Right:
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide }));
			return commands;
		case CubeFaces::Down:
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide }));
			commands.emplace_back(Extend({ Motors::LeftGrip }));
			commands.emplace_back(Extend({ Motors::LeftSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			Neutral({ Motors::LeftGrip });
			commands.emplace_back(Extend({ Motors::RightGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::LeftSlide }));
			commands.emplace_back(Extend({ Motors::LeftGrip }));
			commands.emplace_back(Extend({ Motors::LeftSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			return commands;
		case CubeFaces::Back:
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));

			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));

			commands.emplace_back(Extend({ Motors::TopSlide }));
			return commands;
		case CubeFaces::Left:
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));

			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));

			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));

			commands.emplace_back(Extend({ Motors::TopSlide }));
			return commands;

		default:
			throw std::runtime_error("Invalid Cube Face");

		}
	}

	std::vector<RobotCommand> RsolverControl::GenerateRecoverFromCapture(CubeFaces face)
	{
		std::vector<RobotCommand> commands;
		switch (face)
		{
		case CubeFaces::Up:
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::RightGrip }));
			commands.emplace_back(Extend({ Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			Extend({ Motors::RightGrip });
			commands.emplace_back(Neutral({ Motors::LeftGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::RightGrip }));
			commands.emplace_back(Extend({ Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide, Motors::TopSlide }));
			return commands;
		case CubeFaces::Front:
			commands.emplace_back(Extend({ Motors::LeftSlide, Motors::RightSlide }));
			return commands;
		case CubeFaces::Right:
			commands.emplace_back(Neutral({ Motors::TopSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			return commands;
		case CubeFaces::Down:
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::LeftSlide }));
			commands.emplace_back(Neutral({ Motors::LeftGrip }));
			commands.emplace_back(Extend({ Motors::LeftSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			Extend({ Motors::LeftGrip });
			commands.emplace_back(Neutral({ Motors::RightGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::LeftSlide }));
			commands.emplace_back(Neutral({ Motors::LeftGrip }));
			commands.emplace_back(Extend({ Motors::LeftSlide }));
			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide, Motors::TopSlide }));
			return commands;
		case CubeFaces::Back:
			commands.emplace_back(Extend({ Motors::LeftSlide, Motors::RightSlide }));

			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));

			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			return commands;
		case CubeFaces::Left:
			commands.emplace_back(Extend({ Motors::LeftSlide, Motors::RightSlide }));

			commands.emplace_back(Neutral({ Motors::BottomSlide }));
			commands.emplace_back(Extend({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::BottomSlide }));
			commands.emplace_back(Neutral({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			commands.emplace_back(Neutral({ Motors::BottomGrip }));
			commands.emplace_back(Extend({ Motors::TopSlide, Motors::LeftSlide, Motors::RightSlide }));
			return commands;

		default:
			throw std::runtime_error("Invalid Cube Face");

		}
	}

}
