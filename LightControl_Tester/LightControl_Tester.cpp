/*!
/*!
 * This file just for testing the dll interface for light controller, and also brief the usage of lightcontrol.dll
 */


#include <iostream>
#include "../LightControl/include/lightcontrol.h"

#ifdef _DEBUG	//_DEBUG
	#pragma comment(lib, "LightControld.lib")
#else
	#pragma comment(lib, "LightControl.lib")
#endif


int main()
{
	//s1. initialize the serial port in your constructor
	BOOL bIsOpen = InitSerialPort("COM3", 19200);
	if (!bIsOpen) {
		std::cout << "Open serial port failed!" << std::endl;
		return -1;
	}

	//s2. set the brightness use: BOOL SetBrightnessTo(unsigned int nVal, int nChannel = 1, DEVICE_MODEL devModel = DEVICE_MODEL::PD_4T);
	//demo1: set the light value to 225 for channel 3
	if (SetBrightnessTo(225, 3)) {
		std::cout << "value:255, channel:3 result: success!" << std::endl;
	}
	else {
		std::cout << "value:255, channel:3 result: failed!" << std::endl;
	}

	Sleep(5000);   //sleep, just for funny!

	//demo2: set the channel value randomly
	int i = 255;
	srand(time(NULL));
	while (i--) {
		int nValue = rand() % 255 + 1;
		int nChannel = rand() % 4 + 1;
		if (!SetBrightnessTo(nValue, nChannel)) {
			std::cout << "value:255, channel:3 result: failed!" << std::endl;
		}
	}

	//s3. close serial port in you destructor
	CloseSerialPort();
	return 0;
}

