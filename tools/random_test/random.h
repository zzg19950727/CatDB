#ifndef RANDOM_H
#define RANDOM_H
#include <vector>
#include <string>
using std::vector;
using std::string;

string random_list(vector<string> &list);
string random_number(int max_size = 100);
string random_string(int max_size = 20);
string random_time();


#endif  //RANDOM_H