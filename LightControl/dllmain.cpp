// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"


/**
 * Initialize and register the loggers.
 * 
 * \return void;
 */

VOID InitLoggers()
{
    try {
#ifdef _DEBUG
        //Create console logger
        auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#endif  //_DEBUG
        //Create a daily logger - a new file is created every day on 8:30am
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/lightcontrol.log", 8, 30);

#ifdef _DEBUG
        spdlog::sinks_init_list sinkLists = { console_sink, file_sink };
#else   //_DEBUG
        spdlog::sinks_init_list sinkLists = { file_sink };
#endif  //_DEBUG

        std::shared_ptr<spdlog::logger> pLoggers = std::make_shared<spdlog::logger>("lightcontrol", sinkLists);
        pLoggers->set_level(spdlog::level::trace);

        //set global pattern
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [tid %t] [%^%l%$] %v");
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);

        spdlog::initialize_logger(pLoggers);
    }
    catch (const spdlog::spdlog_ex&) {
    }
}


//define the call back function to roll the log files
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        InitLoggers();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        spdlog::shutdown();
        break;
    }
    return TRUE;
}

