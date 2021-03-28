#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cstdarg>
#include "type.h"
#include "log.h"
//#define LOG_SET		LOG_TRACE
static int LOG_SET = LOG_TRACE;
class LogStream
{
public:
	LogStream();
	~LogStream();
	std::ostream* switch_ostream(std::ostream* os);
	void set_log_file(const char* path);

	LogStream &operator<<(char);

	LogStream &operator<<(unsigned char);

	LogStream &operator<<(short);

	LogStream &operator<<(unsigned short);

	LogStream &operator<<(int);

	LogStream &operator<<(unsigned int);

	LogStream &operator<<(long);

	LogStream &operator<<(unsigned long);

	LogStream &operator<<(long long);

	LogStream &operator<<(unsigned long long);

	LogStream &operator<<(float);

	LogStream &operator<<(double);

	LogStream &operator<<(const char *);

	LogStream &operator<<(const std::string&);

	LogStream& operator()(int log_level, const std::string& module);

	bool print_info(const char* file, int line, const char* function);

private:
	std::ofstream out_file;
	std::ostream* os;
	std::string module;
	int log_level;
};

LogStream::LogStream()
	:os(&std::cout),
	log_level(LOG_ERR)
{

}

LogStream::~LogStream()
{
	if(out_file.is_open())
	{
		out_file.close();
	}	
}

std::ostream* LogStream::switch_ostream(std::ostream* os)
{
	std::ostream* ret = this->os;
	this->os = os;
	return ret;
}

void LogStream::set_log_file(const char* path)
{
	out_file.open(path);
	os = &out_file;
}

LogStream &LogStream::operator << (char value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (unsigned char value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (short value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (unsigned short value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (int value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (unsigned int value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (long value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (unsigned long value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (long long value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (unsigned long long value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator<<(float value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator<<(double value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream &LogStream::operator<<(const char *value)
{
	*os << std::string(value);
	os->flush();
	return *this;
}

LogStream &LogStream::operator << (const std::string& value)
{
	*os << value;
	os->flush();
	return *this;
}

LogStream & LogStream::operator()(int log_level, const std::string & module)
{
	this->log_level = log_level;
	this->module = module;
	return *this;
}

LogStream ostream;

String put_time(time_t t)
{
	char tmp[255];
	tm* time = localtime(&t);
	if (time == NULL)
	{
		return "";
	}
	sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d",
		time->tm_year + 1900,
		time->tm_mon + 1,
		time->tm_mday + 1,
		time->tm_hour,
		time->tm_min,
		time->tm_sec
	);
	return String(tmp);
}

bool LogStream::print_info(const char * file, int line, const char * function)
{
	if (log_level > LOG_SET || module=="Object" || module=="Page" || module=="TableSpace" || module == "IoService")
		return false;
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	*os << "[" << put_time(t) <<"] ";
	if (log_level == LOG_ERR)
		*os << "[ERR  ] [" << module << "] ";
	else if (log_level == LOG_WARN)
		*os << "[WARN ] [" << module << "] ";
	else if (log_level == LOG_INFO)
		*os << "[INFO ] [" << module << "] ";
	else if (log_level == LOG_TRACE)
		*os << "[TRACE] [" << module << "] ";
	*os << "[" << file << "] [" << line << "] [" << function << "(...)] ";
	return true;
}

unsigned char hex_tab[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

static int outc(char c)
{
	ostream << c;
	return 0;
}

static int outs(const char *s)                                 //输出字符串
{
	ostream << s;
	return 0;
}

static int out_int(int n, int base, char lead, int maxwidth)
{
	int m = 0;
	char buf[255], *s = buf + sizeof(buf);
	int count = 0, i = 0;	

	*--s = '\0';
	if (n < 0) {
		m = -n;
	}
	else {
		m = n;
	}

	do {
		*--s = hex_tab[m%base];
		count++;
	} while ((m /= base) != 0);
	if (n < 0)
		*--s = '-';

	return outs(s);
}

static int out_unsigned_int(unsigned int m, int base, char lead, int maxwidth)
{
	char buf[255], *s = buf + sizeof(buf);
	int count = 0, i = 0;

	*--s = '\0';

	do {
		*--s = hex_tab[m%base];
		count++;
	} while ((m /= base) != 0);

	return outs(s);
}

static int vprint(const char *fmt, va_list ap)
{
	char lead = ' ';
	int  maxwidth = 0;

	for (; *fmt != '\0'; fmt++){
		if (*fmt != '%') {
			outc(*fmt);
			continue;
		}

		fmt++;
		if (*fmt == '0') {
			lead = '0';
			fmt++;
		}

		while (*fmt >= '0' && *fmt <= '9') {
			maxwidth *= 10;
			maxwidth += (*fmt - '0');
			fmt++;
		}

		switch (*fmt) {
		case 'd': out_int(va_arg(ap, int), 10, lead, maxwidth); break;
		case 'f': out_int(va_arg(ap, double), 10, lead, maxwidth); break;
		case 'l': out_int(va_arg(ap, int), 10, lead, maxwidth); break;
		case 'o': out_unsigned_int(va_arg(ap, unsigned int), 8, lead, maxwidth); break;
		case 'u': out_unsigned_int(va_arg(ap, unsigned int), 10, lead, maxwidth); break;
		case 'x': out_unsigned_int(va_arg(ap, unsigned int), 16, lead, maxwidth); break;
		case 'c': outc(va_arg(ap, int)); break;
		case 's': outs(va_arg(ap, char *)); break;
		case 'p': outs(va_arg(ap, char *)); break;
		default:
			outc(*fmt);
			break;
		}
	}
	return 0;
}

bool CatDB::Common::log_set_info(const char* file, int line, const char* function)
{
	return ostream.print_info(file, line, function);
}

void CatDB::Common::log_set_level(int log_level, const char* module)
{
	ostream(log_level, module);
}

void CatDB::Common::log_print(const char *fmt, ...)
{
	va_list vap;
	va_start(vap, fmt);
	vprint(fmt, vap);
	va_end(vap);
	ostream << "\n";
}

std::ostream* CatDB::Common::switch_log_ostream(std::ostream* os)
{
	return ostream.switch_ostream(os);
}

void CatDB::Common::set_log_file(const char* file)
{
	ostream.set_log_file(file);
}

void CatDB::Common::set_debug_level(int level)
{
	LOG_SET = level;
}
