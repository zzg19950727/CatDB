#include "config_read.h"
#include <fstream>
#include <regex>
using namespace CatDB::Server;

Config::Config(const char* path)
{
	read_file(path);
}

Config::~Config()
{
}

String Config::value(const String& key)const
{
	auto iter = m_keys.find(key);
	if(iter != m_keys.cend())
		return iter->second;
	else
		return "";
}

void Config::read_file(const char* path)
{
	std::ifstream file(path);
	if (!file.is_open())
		return ;

	std::regex regex("[\\s]*([a-zA-Z0-9_]+)[\\s]*[=][\\s]*([a-zA-Z0-9_\./:]+)[\\s]*");
	while (!file.eof())
	{
		String config;
		std::getline(file, config);
		if (config.empty())
			continue;

		std::smatch result;
		if (std::regex_search(config.cbegin(), config.cend(), result, regex))
		{
			if (result.size() != 3)
				break;
			String key = result.str(1);
			String value = result.str(2);
			m_keys[key] = value;
		}
	}
	file.close();
}
