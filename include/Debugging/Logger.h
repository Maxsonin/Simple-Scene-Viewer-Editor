#ifndef LOGGER
#define LOGGER

#include "ConsoleDebug.h"

#include <fstream>
#include <mutex>
#include <iostream>

class Logger
{
public:
    static void Initialize();

    static void WriteLog(const std::string& message);

private:
    Logger();
    ~Logger();

    const std::string m_FileName = "application_logs.txt";
    std::ofstream m_FileStream;
    static Logger* m_Instance;
    static std::mutex m_Mutex;
};

#endif // !LOGGER
