/*!
 * Implement file for light conroller
 */


#include "lightcontrol.h"

/*
*  Global variant for read and write the serial port
*/

//global variants
serial::Serial g_serialPort;
std::shared_ptr<spdlog::logger> g_logger = NULL;

LIGHTCONTROL_API BOOL InitSerialPort(const std::string& port,
	uint32_t baudrate, 
	uint32_t timeout,
	bytesize_t bytesize, 
	parity_t parity, 
	stopbits_t stopbits, 
	flowcontrol_t flowcontrol)
{
	g_logger = spdlog::get("lightcontrol");
	if (g_serialPort.isOpen()) {
		g_serialPort.close();
	}

	Timeout t = Timeout::simpleTimeout(timeout);
	g_serialPort.setBaudrate(baudrate);
	g_serialPort.setTimeout(t);
	g_serialPort.setBytesize(bytesize);
	g_serialPort.setParity(parity);
	g_serialPort.setStopbits(stopbits);
	g_serialPort.setFlowcontrol(flowcontrol);
	g_serialPort.setPort(port);

	try {
		g_serialPort.open();
		if (g_serialPort.isOpen()) {
			g_logger->info("Open serial port successful! '{}'", port);
			return TRUE;
		}
		else {
			g_logger->error("Serial port opened failed! code: {}", GetLastError());
			return FALSE;
		}
	}
	catch (exception& e) {
		g_logger->error("Serial port opened failed! {}", e.what());
	}

	return FALSE;
}


LIGHTCONTROL_API BOOL SetBrightnessTo(unsigned int nVal, int nChannel, DEVICE_MODEL devModel)
{
	if (nVal < 0 || nVal > 255 || nChannel < 1) {
		return FALSE;
	}

	std::stringstream ss;
	char strVal[5] = { 0 };

	switch (devModel) {
		case DEVICE_MODEL::PD_4T:
			ss << "S";
			switch (nChannel) {
			case 1:
				ss << "A";
				break;
			case 2:
				ss << "B";
				break;
			case 3:
				ss << "C";
				break;
			case 4:
				ss << "D";
				break;
			}

			sprintf_s(strVal, 5, "%04d", nVal);
			ss << strVal;
			ss << "#";
			
			if (WriteSerialPort(ss.str())) {
				std::string strReceived;
				ReadSerialPort(strReceived, 256);

				if ((strReceived.c_str()[0] - 'A' + 1) == nChannel) {
					g_logger->info("Set light value OK! Send: {} Received: {}", ss.str(), strReceived);
					return TRUE;
				}
				else {
					g_logger->error("Set light value NG! Send: {} Received: {}", ss.str(), strReceived);
					return FALSE;
				}
			}
			else {
				//CLOG(ERROR, "lightcontrol") << "Send command string to light conroller failed!";
				g_logger->error("Send command failed!");
				return FALSE;
			}
		break;

		case DEVICE_MODEL::DCS20_4C015W_24PS:
			break;
		default:
			break;

	}
	
	return FALSE;
}


LIGHTCONTROL_API BOOL WriteSerialPort(std::string str)
{
	if (!g_serialPort.isOpen()) {
		return FALSE;
	}

	try {
		size_t nSize = g_serialPort.write(str);
		if (nSize == str.size()) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	catch (exception&e) {
		g_logger->error("Write serial port failed! {}", e.what());
	}

	return FALSE;
}


LIGHTCONTROL_API BOOL ReadSerialPort(std::string& strReceived, size_t bytesToRead)
{
	if (!g_serialPort.isOpen() || bytesToRead < 0) {
		return FALSE;
	}

	try {
		strReceived = g_serialPort.read(bytesToRead);
		return TRUE;
	}
	catch(exception &e){
		g_logger->error("Read serial port failed! {}", e.what());
	}

	return FALSE;
}


LIGHTCONTROL_API VOID CloseSerialPort()
{
	if (g_serialPort.isOpen()) {
		g_serialPort.close();
		g_logger->info("Serial port closed!");
	}
}
