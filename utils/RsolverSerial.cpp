#include "RsolverSerial.h"

namespace Rsolver {

	Rsolver::RsolverSerial::RsolverSerial(std::string portName, int baud)
		: m_portName(portName)
		, m_baud(baud)
		, m_delim("\r\n")
	{
	}

	Rsolver::RsolverSerial::~RsolverSerial()
	{
		if (m_serPort)
		{
			m_serPort->cancel();
			m_serPort->close();
			m_serPort.reset();
		}
		m_io.stop();
		m_io.reset();
	}

	void RsolverSerial::Initialize()
	{
		boost::system::error_code errorCode;
		if (m_serPort)
		{
			throw std::runtime_error("WARNING: Serial port already open!");
		}

		m_serPort = std::make_shared<boost::asio::serial_port>(m_io);
		m_serPort->open(m_portName, errorCode);
		if (errorCode)
		{
			throw std::runtime_error("Cannot open serial port: " + m_portName + " Error Message"
				+ errorCode.message().c_str());
		}

		m_serPort->set_option(boost::asio::serial_port_base::baud_rate(m_baud));
	}

	void RsolverSerial::WriteToSerial(const unsigned char * msg, const int size)
	{
		if (!m_serPort)
		{
			throw std::runtime_error("ERROR: Write called on Unintialized Serial port! Call Initialize() first!");
		}
		boost::system::error_code errorCode;
		if (size == 0)
		{
			return;
		}

		m_serPort->write_some(boost::asio::buffer(msg, size), errorCode);

		if (errorCode)
		{
			throw std::runtime_error("WriteToSerial Failed: " + m_portName + " Error Message"
				+ errorCode.message().c_str());
		}
	}

	std::string RsolverSerial::ReadLineFromSerial()
	{
		boost::asio::streambuf buf;
		boost::asio::read_until(*m_serPort, buf, m_delim);
		std::istream is(&buf);
		std::string line;
		std::getline(is, line);
		return line;
	}

}
