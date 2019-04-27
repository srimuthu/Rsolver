#pragma once

namespace Rsolver
{
class IRsolverSerial
{
public:
	virtual ~IRsolverSerial() = default;

	virtual void Initialize() = 0;
	virtual void WriteToSerial(const unsigned char* msg, const int size) = 0;
	virtual std::string ReadLineFromSerial() = 0;
};
}

