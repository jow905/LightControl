// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "easylogging++.h"

//disable the default logger
//el::Loggers::reconfigureLogger("default", el::ConfigurationType::Enabled, "false");

INITIALIZE_EASYLOGGINGPP


//define the call back function to roll the log files

static unsigned int idx;

void rolloutHandler(const char* filename, std::size_t size)
{
    //current time
    struct tm stime;
    time_t now = time(0);
    localtime_s(&stime, &now);

    char tmp[32] = { NULL };
    strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", &stime);
    std::string date(tmp);

    char buf[MAX_PATH] = { 0 };
    GetCurrentDirectoryA(MAX_PATH, buf);

    CreateDirectoryA(".\\logs", NULL);
    std::stringstream ssSourceFileName;
    std::stringstream ssTargetFileName;
    ssSourceFileName << buf << "\\" << filename;
    ssTargetFileName << buf << "\\logs\\" << date << ".log";
    std::string strSourceFileName = ssSourceFileName.str();
    std::string strTargetFileName = ssTargetFileName.str();
    CopyFileA(strSourceFileName.c_str(), strTargetFileName.c_str(), TRUE);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        //configuration
        el::Configurations confs;
        confs.setToDefault();
        confs.setGlobally(el::ConfigurationType::Format, "[%datetime %level] %msg");
        confs.setGlobally(el::ConfigurationType::Enabled, "true");
        confs.setGlobally(el::ConfigurationType::Filename, "lightcontrol.log");
        confs.setGlobally(el::ConfigurationType::ToFile, "true");
#ifndef _DEBUG 
        confs.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
#endif
        confs.setGlobally(el::ConfigurationType::SubsecondPrecision, "6");
        confs.setGlobally(el::ConfigurationType::PerformanceTracking, "true");
        confs.setGlobally(el::ConfigurationType::MaxLogFileSize, "2097152");
        confs.setGlobally(el::ConfigurationType::LogFlushThreshold, "1");

        //config and register new logger named 'ligtcontrol'
        el::Loggers::reconfigureLogger("lightcontrol", confs);

        el::Helpers::installPreRollOutCallback(rolloutHandler);

        //CLOG(INFO, "lightcontrol") << "Logger configured!!";
    }

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

