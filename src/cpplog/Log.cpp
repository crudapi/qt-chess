#include "Log.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <map>
#include <cassert>

Log Log::m_instance;

Log & Log::instance(void)
{
	return m_instance;
}
Log::~Log()
{
	m_fout.close();
}

void Log::log(const std::string& content, const std::string & file, int line, Log::LOG_LEVEL level, const std::string& func)
{
    if (level >= m_level)
	{
        writeLogToTarget(getCurrentTime(), level, func, content, file, line);
	}
}

void Log::writeLogToTarget(const std::string& time_clock, Log::LOG_LEVEL level, const std::string& func, const std::string & log, const std::string& file, int line)
{
	if (m_target == Log::LOG_TARGET::NO_LOG)
	{
		return;
	}

	std::map<int, std::string> logPartsMap;
    assert(m_formatOrder.size() == static_cast<size_t>(LOG_FORMAT_PART::SIZE));
    if (m_formatOrder.at(static_cast<size_t>(Log::LOG_FORMAT_PART::LEVEL)) != NO_USE)
	{
		std::string logLevelPartString;
		switch (level)
		{
		case Log::LOG_LEVEL::DEBUG_LEVEL:
			logLevelPartString = "DEBUG ";
			break;
		case Log::LOG_LEVEL::INFOR:
			logLevelPartString = "INFOR ";
			break;
		case Log::LOG_LEVEL::ERROR_LEVEL:
            logLevelPartString = "ERROR ";
			break;
		case Log::LOG_LEVEL::FATAL:
			logLevelPartString = "FATAL ";
			break;
		default:
			break;
		}
        logPartsMap[m_formatOrder.at(static_cast<size_t>(Log::LOG_FORMAT_PART::LEVEL))] = logLevelPartString;
	}
    auto order = m_formatOrder.at(static_cast<size_t>(Log::LOG_FORMAT_PART::FUNC));
    if (order != NO_USE)
    {
        logPartsMap[order] = func + "() ";
    }
	order = m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::DATETIME)];
	if (order != NO_USE)
	{
		logPartsMap[order] = time_clock + " ";
	}
	
	order = m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::LOG)];
	if (order != NO_USE)
	{
		logPartsMap[order] = log + " ";
	}
	
	order = m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::SOURCEFILE)];
	if (order != NO_USE)
	{
		logPartsMap[order] = file + " ";
	}
	
	order = m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::LINE)];
	if (order != NO_USE)
	{
		logPartsMap[order] = ":" +std::to_string(line) ;
	}

	std::string logRecord;
	for (auto& item : logPartsMap)
	{
		logRecord += item.second;
	}
	
	switch (m_target)
	{
	case Log::LOG_TARGET::CONSOLE:
    {
        std::lock_guard<std::mutex> lock(Log::instance().getMutex());
        std::cout << logRecord << std::endl;
    	break;
    }
    case Log::LOG_TARGET::FILE:
    {
        std::lock_guard<std::mutex> lock(Log::instance().getMutex());
        m_fout << logRecord << std::endl;
        break; 
    }
	case Log::LOG_TARGET::ALL:
    {
        std::lock_guard<std::mutex> lock(Log::instance().getMutex());
        std::cout << logRecord << std::endl;
        m_fout << logRecord << std::endl;
        break; 
    }
	default:
		break;
	}
}

std::string Log::getCurrentTime(void) const
{
	using std::chrono::system_clock;
    auto now = system_clock::now();
    auto high = std::chrono::steady_clock::now();
	std::time_t tt = system_clock::to_time_t(now);
	struct std::tm  valtm;
#ifdef _WIN32
	localtime_s(&valtm, &tt);
#else
    localtime_r(&tt, &valtm);
#endif
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	std::stringstream ss;
	ss << std::put_time(&valtm, "%Y-%m-%d %H:%M:%S:")
        <<std::setw(6)<<std::setfill('0')<<(high.time_since_epoch().count()/1000%1000000);
	return ss.str();
}

bool Log::init(const std::string & logFile, Log::LOG_LEVEL level, Log::LOG_TARGET target, const std::string& format)
{
    if (m_inited)
    {
        return true;
    }
	m_inited = true;
	m_level = level;
	m_target = target;
	initFormatOrder(format);
    if (target != Log::LOG_TARGET::CONSOLE)//all and file
    {
        m_fout.open(logFile, std::ofstream::app);
        if (m_fout.is_open())
        {
            m_fout << "Log inited OK . .............................................." << std::endl;
        }
    }
	return m_inited;
}

void Log::initFormatOrder(const std::string & format)
{
	std::stringstream ss(format);
	std::string line;
	int order = 0;
    NO_USE = -1;//always this value.
	m_formatOrder.resize(static_cast<size_t>(Log::LOG_FORMAT_PART::SIZE)
		, NO_USE);//NO_USE means this part not used when output log
	while (std::getline(ss, line, '_'))//"level_datetime_log_source_line"
	{
		if (line == "level")
		{
			m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::LEVEL)] = ++order;
		}
		else if (line == "datetime")
		{
			m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::DATETIME)] = ++order;
		}
		else if (line == "log")
		{
			m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::LOG)] = ++order;
		}
		else if (line == "source")
		{
			m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::SOURCEFILE)] = ++order;
		}
		else if (line == "line")
		{
			m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::LINE)] = ++order;
		}
        else if (line == "func")
        {
            m_formatOrder[static_cast<size_t>(Log::LOG_FORMAT_PART::FUNC)] = ++order;
        }
		else
		{
			throw std::runtime_error("not valid log format!" + format);
		}
	}
}

void init_log_cpp(const std::string & logFile, Log::LOG_LEVEL level, Log::LOG_TARGET target, const std::string& format)
{
    std::cout << "init_log_cpp entered." << std::endl;
	if (!Log::instance().init(logFile, level, target, format))
	{
		throw std::runtime_error("Log::instance().init failed!");
	}
}
