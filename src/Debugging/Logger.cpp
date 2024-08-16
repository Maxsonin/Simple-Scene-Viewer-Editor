#include "Debugging/Logger.h"

void Logger::Initialize() 
{
    std::lock_guard<std::mutex> guard(m_Mutex);
    if (!m_Instance) { m_Instance = new Logger(); }
}

void Logger::WriteLog(const std::string& message)
{
    std::lock_guard<std::mutex> guard(m_Mutex);
    if (m_Instance)
        m_Instance->m_FileStream << message << std::endl;
    else
        std::cerr << ERROR("ERROR: Logger is NOT initialized.") << std::endl;
}

Logger::Logger()
    : m_FileStream(m_FileName, std::ios::out)
{
    if (!m_FileStream.is_open())
        std::cerr << ERROR("ERROR: Failed to CREATE/OPEN file for writing Logs.") << std::endl;
}

Logger::~Logger()
{
    if (m_FileStream.is_open()) { m_FileStream.close(); }
}

Logger* Logger::m_Instance = nullptr;
std::mutex Logger::m_Mutex;
