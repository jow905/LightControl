/*
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

#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using namespace serial;


//Remove the definitions when you want import the APIs
#define LIGHTCONTROL_API_EXPORT 

#ifdef LIGHTCONTROL_API_EXPORT
	#define LIGHTCONTROL_API extern "C" __declspec(dllexport)
#else
	#define LIGHTCONTROL_API extern "C" __declspec(dllimport)
#endif


//The light controller device model list
typedef enum _DEVICE_MODEL {
    PD_4T,
    DCS20_4C015W_24PS
} DEVICE_MODEL;

/**
 * Initialize the serial port and open it.
 *
 * \param port
 * \param baudrate
 * \param timeout
 * \param bytesize
 * \param parity
 * \param stopbits
 * \param flowcontrol
 * \return bool
 */
LIGHTCONTROL_API BOOL InitSerialPort(const std::string& port = "",
    uint32_t baudrate = 9600,
    uint32_t timeout = 50,
    bytesize_t bytesize = eightbits,
    parity_t parity = parity_none,
    stopbits_t stopbits = stopbits_one,
    flowcontrol_t flowcontrol = flowcontrol_none);

/**
 * Change the brightness value.
 *
 * \param nVal          -value to change
 * \param nChannel      -the channel
 * \param devModel      -the device model, default device is 'PD_4T'
 * \return bool         -true/false
 */
LIGHTCONTROL_API BOOL SetBrightnessTo(unsigned int nVal, int nChannel = 1, DEVICE_MODEL devModel = DEVICE_MODEL::PD_4T);

/**
 * Write string to serial port.
 *
 * \param str
 * \return bool
 */
LIGHTCONTROL_API BOOL WriteSerialPort(std::string str="");

/**
 * Read data from serial port.
 *
 * \param strReceived       -buffer to store the data from serial port
 * \param bytesToRead       -number of bytes ready to read
 * \return bool
 */
LIGHTCONTROL_API BOOL ReadSerialPort(std::string& strReceived, size_t bytesToRead = 256);

/**
 * Close the serial port.
 *
 * \return void
 */
LIGHTCONTROL_API VOID CloseSerialPort();

