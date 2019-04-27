#pragma once
#include <memory>
#include <string>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include "IRsolverSerial.h"

#define SERIAL_PORT_READ_BUF_SIZE	256

namespace Rsolver {

class RsolverSerial : public IRsolverSerial
{
public:
	explicit RsolverSerial(std::string portName, int baud);
	~RsolverSerial();

	virtual void Initialize() override;
	virtual void WriteToSerial(const unsigned char* msg, const int size) override;
	virtual std::string ReadLineFromSerial() override;

private:
	std::string									m_portName;
	int											m_baud;
	boost::asio::io_service						m_io;
	std::shared_ptr<boost::asio::serial_port>	m_serPort;
	std::string									m_delim;
	char										m_readRawBuf[SERIAL_PORT_READ_BUF_SIZE];
};

}