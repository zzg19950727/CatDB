#ifndef TYPE_H
#define TYPE_H
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <memory>
#include <chrono>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <cstring>
#include <cassert>
#define DECLARE(class_name)	\
class class_name;\
typedef shared_ptr<class_name> class_name##_s;

#define DISALLOW_COPY_AND_ASSIGN(class_name) \
class_name(const class_name&)=delete;\
class_name&operator=(const class_name&)=delete;

typedef unsigned char u8;
typedef unsigned short	u16;
typedef unsigned int u32;
typedef double	f64;
typedef unsigned long long u64;
typedef std::string String;
typedef std::chrono::system_clock Clock;

template<typename T>
using shared_ptr = std::shared_ptr<T>;
template<typename T1, typename T2>
using HashMap = std::map<T1, T2>;
template<typename T1, typename T2>
using UnorderedHashMap = std::unordered_map<T1, T2>;
template<typename T>
using HashSet = std::unordered_set<T>;
template<typename T>
using List = std::list<T>;
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using Hash = std::hash<T>;
template<typename T>
using Queue = std::queue<T>;
template<typename T1, typename T2>
using Pair = std::pair<T1, T2>;
#endif	//TYPE_H