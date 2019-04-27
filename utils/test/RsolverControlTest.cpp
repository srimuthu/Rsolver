#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "RsolverSerial.h"
#include "RsolverControl.h"

namespace Rsolver
{
namespace Test
{

using namespace testing;

static const int g_baud = 9600;
#ifdef _WIN32
static const std::string g_portName = "COM3";
#else
static const std::string g_portName = "ToBeImplemented";
#endif

class RsolverControlTest : public testing::Test
{
public:
	void SetUp() override
	{
		int baud = g_baud;
		m_rsolverSerial = std::make_unique<RsolverSerial>(g_portName, baud);
		m_rsolverControl = std::make_unique<RsolverControl>(std::move(m_rsolverSerial));
	}

	std::unique_ptr<RsolverSerial>	m_rsolverSerial;
	std::unique_ptr<RsolverControl>	m_rsolverControl;
};

TEST_F(RsolverControlTest, TestControlCreation)
{
	m_rsolverControl->InitializeRobot();
}


}
}
