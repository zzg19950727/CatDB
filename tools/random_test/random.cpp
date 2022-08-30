#include "random.h"

string random_list(vector<string> &list)
{
    return list[ std::rand() % list.size() ];
}

string random_number(int max_size)
{
    return std::to_string(std::rand() % max_size);
}

string random_string(int max_size)
{
    string str = "'";
    int n = (std::rand() % max_size) + 1;
    for (int i = 0; i < n; ++i) {
        int letter = std::rand()%26;
        int type = std::rand()%3;
        if (0 == type) {
            str += ('a' + letter);
        } else if (1 == type) {
            str += ('A' + letter);
        } else {
            str += "%";
        }
    }
    str += "'";
    return str;
}

string random_time()
{
    string time = "'";
    time += std::to_string(2000+std::rand()%20);
    time += "-" + std::to_string(std::rand()%12+1);
    time += "-" + std::to_string(std::rand()%28+1);
    time += " " + std::to_string(std::rand()%24);
    time += ":" + std::to_string(std::rand()%60);
    time += ":" + std::to_string(std::rand()%60);
    time += "'";
    return time;
}