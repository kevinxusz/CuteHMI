#include "LibmodbusConnection.hpp"
#include "functions.hpp"

#include <QtDebug>

namespace cutehmi {
namespace modbus {

void LibmodbusConnection::setByteTimeout(Timeout timeout)
{
	 if (modbus_set_byte_timeout(context(), timeout.sec, timeout.usec) == -1)
		 qDebug() << "libmodbus error: " << modbus_strerror(errno);
}

LibmodbusConnection::Timeout LibmodbusConnection::byteTimeout() const
{
	Timeout timeout;
	// libmodbus seems to be not const-correct.
	if (modbus_get_byte_timeout(const_cast<modbus_t *>(context()), & timeout.sec, & timeout.usec) == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return timeout;
}

void LibmodbusConnection::setResponseTimeout(Timeout timeout)
{
	 if (modbus_set_response_timeout(context(), timeout.sec, timeout.usec) == -1)
		 qDebug() << "libmodbus error: " << modbus_strerror(errno);
}

LibmodbusConnection::Timeout LibmodbusConnection::responseTimeout() const
{
	Timeout timeout;
	// libmodbus seems to be not const-correct.
	if (modbus_get_response_timeout(const_cast<modbus_t *>(context()), & timeout.sec, & timeout.usec) == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return timeout;
}

bool LibmodbusConnection::connect()
{
	return modbus_connect(context()) == 0;
}

void LibmodbusConnection::disconnect()
{
	modbus_close(context());
}

int LibmodbusConnection::readIr(int addr, int num, uint16_t * dst)
{
	int result = modbus_read_input_registers(context(), addr, num, dst);
	if (result == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return result;
}

int LibmodbusConnection::readR(int addr, int num, uint16_t * dst)
{
	int result = modbus_read_registers(context(), addr, num, dst);
	if (result == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return result;
}

int LibmodbusConnection::writeR(int addr, uint16_t value)
{
	// For some reason libmodbus uses int as a value parameter, so we need to convert it back.
	int result = modbus_write_register(context(), addr, intFromUint16(value));
	if (result != 1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return result;
}

int LibmodbusConnection::readIb(int addr, int num, uint8_t * dst)
{
	int result = modbus_read_input_bits(context(), addr, num, dst);
	if (result == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return result;
}

int LibmodbusConnection::readB(int addr, int num, uint8_t * dst)
{
	int result = modbus_read_bits(context(), addr, num, dst);
	if (result == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return result;
}

int LibmodbusConnection::writeB(int addr, bool value)
{
	// "If the source type is bool, the value false is converted to zero and the value true is converted to one."
	//		-- §4.7/4 C++ Standard via StackOverflow.
	int result = modbus_write_bit(context(), addr, value);
	if (result == -1)
		qDebug() << "libmodbus error: " << modbus_strerror(errno);
	return result;
}

LibmodbusConnection::LibmodbusConnection(modbus_t * context):
	m_context(context)
{
}

const modbus_t * LibmodbusConnection::context() const
{
	return m_context;
}

modbus_t * LibmodbusConnection::context()
{
	return m_context;
}

void LibmodbusConnection::setContext(modbus_t * context)
{
	m_context = context;
}


}
}

//(c)MP: Copyright © 2016, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.