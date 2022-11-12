/*!
 * Header file for light controller
 */
#pragma once
#include <string>
#include <iostream>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using namespace serial;

/*!
 * Remove the definitions when you want import the APIs
 */
#define LIGHTCONTROL_API_EXPORT 

#ifdef LIGHTCONTROL_API_EXPORT
	#define LIGHTCONTROL_API extern "C" __declspec(dllexport)
#else
	#define LIGHTCONTROL_API extern "C" __declspec(dllimport)
#endif

/*
 * The light controller device model list
 */
typedef enum _DEVICE_MODEL {
    PD_4T,
    DCS20_4C015W_24PS
} DEVICE_MODEL;

/*
 *  Open the serial port
 */
LIGHTCONTROL_API BOOL OpenSerialPort(const std::string& port = "",
    uint32_t baudrate = 9600,
    uint32_t timeout = 50,
    bytesize_t bytesize = eightbits,
    parity_t parity = parity_none,
    stopbits_t stopbits = stopbits_one,
    flowcontrol_t flowcontrol = flowcontrol_none);

LIGHTCONTROL_API BOOL SetBrightnessTo(unsigned int nVal, int nChannel = 1, DEVICE_MODEL devModel = DEVICE_MODEL::PD_4T);
LIGHTCONTROL_API BOOL WriteSerialPort(std::string str="");
LIGHTCONTROL_API BOOL ReadSerialPort(std::string& strReceived, size_t bytesToRead = 256);
LIGHTCONTROL_API VOID CloseSerialPort();

