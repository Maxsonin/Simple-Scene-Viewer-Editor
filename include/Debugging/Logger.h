#ifndef LOGGER
#define LOGGER

#include "ConsoleDebug.h"

#include <fstream>
#include <mutex>
#include <iostream>

class Logger 
{
public:
    static void Initialize() 
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (!m_Instance) { m_Instance = new Logger(); }
    }

    static void WriteLog(const std::string& message) 
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (m_Instance) 
            m_Instance->m_FileStream << message << std::endl;
        else 
            std::cerr << ERROR("ERROR: Logger is NOT initialized.") << std::endl;
    }

private:
    Logger()
        : m_FileStream(m_FileName, std::ios::out | std::ios::app) // Write/Append
    {
        if (!m_FileStream.is_open()) 
            std::cerr << ERROR("ERROR: Failed to CREATE/OPEN file for writing Logs.") << std::endl;
    }

    ~Logger() { if (m_FileStream.is_open()) { m_FileStream.close(); } }

    const std::string m_FileName = "application_logs.txt";
    std::ofstream m_FileStream;
    static Logger* m_Instance;
    static std::mutex m_Mutex;
};

Logger* Logger::m_Instance = nullptr;
std::mutex Logger::m_Mutex;

#endif // !LOGGER

