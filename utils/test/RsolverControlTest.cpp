#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "IRsolverSerialMock.h"
#include "RsolverSerial.h"
#include "RsolverControl.h"

namespace Rsolver
{
namespace Test
{

using namespace testing;

class RsolverControlTest : public testing::Test
{
public:
	void SetUp() override
	{
		auto rsolverSerialMock = std::make_unique<StrictMock<RsolverSerialMock>>();
		m_rsolverSerialMock = rsolverSerialMock.get();
		m_rsolverControl = std::make_unique<RsolverControl>(std::move(rsolverSerialMock));
	}

	RsolverSerialMock*				m_rsolverSerialMock;
	std::unique_ptr<RsolverControl>	m_rsolverControl;
};

TEST_F(RsolverControlTest, InitializeRobotSuccess)
{
	EXPECT_CALL(*m_rsolverSerialMock, Initialize());
	EXPECT_CALL(*m_rsolverSerialMock, ReadLineFromSerial()).WillOnce(Return("Ready\r"));
	m_rsolverControl->InitializeRobot();
}

TEST_F(RsolverControlTest, InitializeRobotFailure)
{
	EXPECT_CALL(*m_rsolverSerialMock, Initialize());
	EXPECT_CALL(*m_rsolverSerialMock, ReadLineFromSerial()).WillOnce(Return("Blah\r"));
	EXPECT_THROW(m_rsolverControl->InitializeRobot(), std::runtime_error);
}

TEST_F(RsolverControlTest, MoveInfoExtractionTest)
{
	std::string testToken = "U3";
	auto moveInfo = m_rsolverControl->GetMoveInfoFromStringToken(testToken);
	EXPECT_EQ(moveInfo.face, CubeFaces::Up);
	EXPECT_EQ(moveInfo.times, 3);

	testToken = "R2";
	moveInfo = m_rsolverControl->GetMoveInfoFromStringToken(testToken);
	EXPECT_EQ(moveInfo.face, CubeFaces::Right);
	EXPECT_EQ(moveInfo.times, 2);

	testToken = "L2";
	moveInfo = m_rsolverControl->GetMoveInfoFromStringToken(testToken);
	EXPECT_EQ(moveInfo.face, CubeFaces::Left);
	EXPECT_EQ(moveInfo.times, 2);

	testToken = "D2";
	moveInfo = m_rsolverControl->GetMoveInfoFromStringToken(testToken);
	EXPECT_EQ(moveInfo.face, CubeFaces::Down);
	EXPECT_EQ(moveInfo.times, 2);

	testToken = "B2";
	moveInfo = m_rsolverControl->GetMoveInfoFromStringToken(testToken);
	EXPECT_EQ(moveInfo.face, CubeFaces::Back);
	EXPECT_EQ(moveInfo.times, 2);

	testToken = "F2";
	moveInfo = m_rsolverControl->GetMoveInfoFromStringToken(testToken);
	EXPECT_EQ(moveInfo.face, CubeFaces::Front);
	EXPECT_EQ(moveInfo.times, 2);
}

TEST_F(RsolverControlTest, SetProgressCallbackTest)
{
	int numCalls = 0;
	m_rsolverControl->SetProgressUpdateCallback([&](Progress progress) {
		numCalls++;
	});
	std::vector<RobotCommand> commands = m_rsolverControl->GenerateLockCubeInPlaceCommands();
	auto totalCommands = commands.size();
	{
		InSequence seq;
		for (auto i = 0; i < totalCommands; i++)
		{
			EXPECT_CALL(*m_rsolverSerialMock, WriteToSerial(_, _));
			EXPECT_CALL(*m_rsolverSerialMock, ReadLineFromSerial()).WillOnce(Return(std::to_string(i+1)));
		}
	}
	m_rsolverControl->ExecuteRobotCommands(commands);
	EXPECT_EQ(numCalls, totalCommands);
}

/*TEST_F(RsolverControlTest, GetMoveCommandsFromSolutionTest)
{
	const std::string solution = "U2 D3 B3 U1 R1 D2 L2 D2 L1 U3 L3 D2 B2 U1 R2 F2 U1 L2 U3 U2 R2 F2 R2 F2 U2 F2 U2 F2 U2";
	m_rsolverControl->InitializeRobot();
	auto commands = m_rsolverControl->GetRobotCommandsFromSolution(solution);
	m_rsolverControl->ExecuteRobotCommands(commands);
}*/

}
}
