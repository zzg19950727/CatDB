#include "config_read.h"
#include "config.h"
#include "log.h"

using namespace CatDB::Server;

ConfigService::ConfigService()
{
	
}

ConfigService::~ConfigService()
{
	
}

int ConfigService::init(const char* path)
{
	ConfigReader reader;
	reader.read_file(path, m_keys);
	return 0;
}

void ConfigService::init()
{
	add_value("query_timeout", "1000000");
    add_value("log_level", "ERROR");
    add_value("log_module", "ALL");
	add_value("sample_size", "40000");
}

std::string ConfigService::ip()const
{
	String ip = value("ip");
	if(ip.empty())
		return "127.0.0.1";
	else
		return ip;
}

int ConfigService::port()const
{
	String port = value("port");
	if(port.empty() || std::stoi(port)<=0 )
		return 1234;
	else
		return std::stoi(port);
}

int ConfigService::max_client_count()const
{
	String count = value("max_client_count");
	if(count.empty() || std::stoi(count)<=0)
		return 2048;
	else
		return std::stoi(count);
}

int ConfigService::cache_size()const
{
	String size = value("cache_size");
	if(size.empty() || std::stoi(size)<=0)
		return 1024000;
	else
		return std::stoi(size);
}

int ConfigService::thread_pool_size() const
{
	String thread_pool_size = value("thread_pool_size");
	if (thread_pool_size.empty() || std::stoi(thread_pool_size) <= 0)
		return 1234;
	else
		return std::stoi(thread_pool_size);
}

String ConfigService::data_dir()const
{
	String dir = value("data_dir");
	return dir;
}

String ConfigService::recycle_dir()const
{
	String dir = value("recycle_dir");
	return dir;
}

String ConfigService::log_file_path()const
{
	String dir = value("log_file");
	return dir;
}

int ConfigService::log_level()const
{
	String level_str = value("log_level");
	int level = LOG_LEVEL_ERR;
	if (level_str == "trace") {
		level = LOG_LEVEL_TRACE;
	} else if (level_str == "info") {
		level = LOG_LEVEL_INFO;
	} else if (level_str == "error") {
		level = LOG_LEVEL_ERR;
	}
	return level;
}

String ConfigService::log_module() const
{
	String module = value("log_module");
	return module;
}

long long ConfigService::query_timeout() const
{
	String query_timeout = value("query_timeout");
	if (query_timeout.empty() || 
		query_timeout.length() > 9 ||
		std::stoi(query_timeout) <= 0)
		return 10000;
	else
		return std::stoi(query_timeout);
}

String ConfigService::value(const String& key)const
{
	auto iter = m_keys.find(key);
	if(iter != m_keys.cend())
		return iter->second;
	else
		return "";
}

int ConfigService::int_value(const String& key)const
{
	auto iter = m_keys.find(key);
	if(iter != m_keys.cend())
		return std::stoi(iter->second);
	else
		return 1;
}

HashMap<String, String>& ConfigService::get_all_config()
{
	return m_keys;
}

void ConfigService::set_value(const String& key, const String& value)
{
	auto iter = m_keys.find(key);
	if(iter != m_keys.cend()) {
		iter->second = value;
	}
}

void ConfigService::add_value(const String& key, const String& value)
{
	m_keys[key] = value;
}

bool ConfigService::has_key(const String& key) const
{
	return m_keys.find(key) != m_keys.cend();
}

void ConfigService::search_key(const String& key, Vector<Pair<String,String>> &result)
{
	for (auto iter = m_keys.cbegin(); iter != m_keys.cend(); ++iter) {
		if (iter->first.find(key) != String::npos) {
			result.push_back(Pair<String,String>(iter->first, iter->second));
		}
	}
}