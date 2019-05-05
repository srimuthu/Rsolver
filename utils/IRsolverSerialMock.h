#pragma once

#include <string>
#include <gmock/gmock.h>
#include "IRsolverSerial.h"

namespace Rsolver
{
namespace Test
{
	class RsolverSerialMock : public IRsolverSerial
	{
	public:
		MOCK_METHOD0(Initialize, void(void));
		MOCK_METHOD2(WriteToSerial, void(const unsigned char* msg, const int size));
		MOCK_METHOD0(ReadLineFromSerial, std::string(void));
	};
}
}

