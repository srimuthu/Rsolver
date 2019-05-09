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
	virtual std::vector<RobotCommand> GenerateLockCubeInPlaceCommands() override;
	virtual std::vector<RobotCommand> GeneratePrepareForCapture(CubeFaces face) override;
	virtual std::vector<RobotCommand> GenerateRecoverFromCapture(CubeFaces face) override;
	virtual void SetProgressUpdateCallback(std::function<void(Progress)> cb) override;

	// Non-interface public methods (maybe move to another interface?)
	void ConfigureWaitForAcknowledge(bool waitForAck);
	MoveInfo GetMoveInfoFromStringToken(const std::string& moveString);

private:

	RobotCommand Extend(Motors motor);
	RobotCommand Extend(std::vector<Motors> motors);
	RobotCommand Neutral(Motors motor);
	RobotCommand Neutral(std::vector<Motors> motors);
	void GenerateCommandsForMoveInfo(std::vector<RobotCommand>& commands, const MoveInfo& moveInfo);
	std::vector<RobotCommand> GenerateStartPositionForCubeFaceMove(const MoveInfo& moveInfo);
	std::vector<RobotCommand> GenerateCommandsForCubeFaceMove(const MoveInfo& moveInfo);
	std::vector<RobotCommand> GenerateEndPositionForCubeFaceMove(const MoveInfo& moveInfo);

	void PerformCubeFaceRotations(std::vector<RobotCommand>& commands, const MoveInfo& moveInfo);
	void PerformOneCubeFaceRotation(std::vector<RobotCommand>& commands, CubeFaces cubeFace);
	bool SendSerialCommand(RobotCommand command);

	void TriggerProgressUpdateCb(int totalSteps, int currentStep);

	std::unique_ptr<IRsolverSerial>		m_serial;
	RobotCommand						m_robotMoveCommand;
	int									m_responseCounter;
	bool								m_waitForAck;
	std::function<void(Progress)>		m_progressCb;

};

}