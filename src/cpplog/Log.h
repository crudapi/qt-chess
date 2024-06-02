#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>

//作者 C++程序员Carea 全网同名 微信 make_it_simple888

class Log
{
public:
	static Log& instance(void);
	~Log();
	enum class LOG_LEVEL { DEBUG_LEVEL, INFOR, ERROR_LEVEL, FATAL };
	enum class LOG_TARGET{ CONSOLE, FILE, ALL, NO_LOG};
public:
	bool init(const std::string& logFile, Log::LOG_LEVEL level, Log::LOG_TARGET target, const std::string& format);
	void log(const std::string& content, const std::string& file, int line, Log::LOG_LEVEL level, const std::string& func);
    std::mutex& getMutex(void) { return m_mutex; }
public:
    int duration = 1;
private:
	void initFormatOrder(const std::string & format);
	void writeLogToTarget(const std::string& time_clock, Log::LOG_LEVEL level, const std::string& func, const std::string& log, const std::string& file, int line);
	std::string getCurrentTime(void) const;
    enum class LOG_FORMAT_PART {LEVEL = 0, DATETIME, LOG, SOURCEFILE, LINE , FUNC, SIZE};
private:
	std::ofstream m_fout;
	static Log m_instance;
	Log::LOG_LEVEL m_level;
	Log::LOG_TARGET m_target;
    std::vector<int> m_formatOrder;
    bool m_inited = false;
    std::mutex m_mutex;
    int NO_USE;
};

void /*SDK_API*/ init_log_cpp(const std::string& logFile, Log::LOG_LEVEL level, Log::LOG_TARGET target, const std::string& format);

#define debug_log_cpp(content) \
    {  \
      std::string content_str; std::ostringstream oss; oss<<content; content_str = oss.str(); \
      Log::instance().log(content_str, __FILE__, __LINE__, Log::LOG_LEVEL::DEBUG_LEVEL, __func__); \
    }
#define infor_log_cpp(content) \
    {  \
      std::string content_str; std::ostringstream oss; oss<<content; content_str = oss.str(); \
      Log::instance().log(content_str, __FILE__, __LINE__, Log::LOG_LEVEL::INFOR, __func__); \
    }
#define error_log_cpp(content)                                                                 \
    {  \
      std::string content_str; std::ostringstream oss; oss<<content; content_str = oss.str(); \
      Log::instance().log(content_str, __FILE__, __LINE__, Log::LOG_LEVEL::ERROR_LEVEL, __func__); \
    }
#define fatal_log_cpp(content) \
    { \
      std::string content_str; std::ostringstream oss; oss<<content; content_str = oss.str(); \
      Log::instance().log(content_str, __FILE__, __LINE__, Log::LOG_LEVEL::FATAL, __func__); \
    }

//use demo：


  //  int main()
  //  {
  //      init_log_cpp("main-log.txt", Log::LOG_LEVEL::DEBUG_LEVEL, Log::LOG_TARGET::FILE, "datetime_level_func_log_source_line");
		//init_log_cpp("log_sdk.txt", Log::LOG_LEVEL::DEBUG_LEVEL, Log::LOG_TARGET::FILE, "datetime_log");//output date time and log only

  //      int a = 1;
  //      debug_log_cpp("this is a message. a="<<a);

  //      return 0;
  //  }

