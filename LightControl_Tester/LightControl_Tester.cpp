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
	//s1. open the serial port,you can put the codes in your constructor
	BOOL bIsOpen = OpenSerialPort("COM3", 19200);
	if (!bIsOpen) {
		return -1;
	}

	//s2. set the brightness use: BOOL SetBrightnessTo(unsigned int nVal, int nChannel = 1, DEVICE_MODEL devModel = DEVICE_MODEL::PD_4T);
	int i = 255;
	srand(time(NULL));
	while (i--) {
		int nChannel = rand() % 4 + 1;
		if (!SetBrightnessTo(i, nChannel)) {
			std::cout << "set brightness: " << i << ", Channel: " << nChannel << " NG!!" << std::endl;
		}
	}


	//s3. close the serial port, you can put this codes in you destructor
	CloseSerialPort();
	return 0;
}

