#include "config_read.h"
#include <fstream>
using namespace CatDB::Server;

bool ConfigReader::parse_key_value(const String& str, String& key, String& value)
{
	u32 i = 0;
	//skip space
	while (i < str.size() && str[i] == ' ')
		++i;
	//parse key
	for (; i < str.size(); ++i) {
		if (str[i] == ' ' || str[i] == '=')
			break;
		else
			key += str[i];
	}
	//skip space
	while (i < str.size() && str[i] == ' ')
		++i;

	if (i >= str.size() || str[i] != '=') {
		return false;
	}
	else {
		++i;
	}
	//skip space
	while (i < str.size() && str[i] == ' ')
		++i;
	//parse value
	for (; i < str.size(); ++i) {
		if (str[i] == ' ')
			break;
		else
			value += str[i];
	}
	return true;
}

void ConfigReader::read_file(const char* path, HashMap<String, String> &keys)
{
	std::ifstream file(path);
	if (!file.is_open())
		return ;

	while (!file.eof())
	{
		String config;
		std::getline(file, config);
		if (config.empty())
			continue;
		String key;
		String value;
		if (parse_key_value(config, key, value))
		{
			keys[key] = value;
		}
	}
	file.close();
}
