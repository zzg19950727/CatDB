#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H
#include <sstream>
#include "type.h"
#define VAR_NAME(var)   #var
#define K(arg) String(VAR_NAME(arg)) + ":" + ::to_kv_string(arg)
#define EXPAND_KV_STRING(...) __VA_ARGS__
#define SUBPROCESS_KV_STRING(X,...) X + "," + 
#define KV_STRING(...) String \
to_kv_string()const { return String("{") + ::to_kv_string(__VA_ARGS__) + String("}"); }

String to_kv_string(const char& value);
String to_kv_string(const u8& value);
String to_kv_string(const u16& value);
String to_kv_string(const u32& value);
String to_kv_string(const int& value);
String to_kv_string(const u64& value);
String to_kv_string(const f64& value);
String to_kv_string(const String& value);
String to_kv_string(const long& value);
String to_kv_string(const unsigned long& value);
String to_kv_string(const char* value);
String to_kv_string(bool value);

template<typename T>
String to_kv_string(const T& value)
{
    return value.to_kv_string();
}

template<typename T>
String to_kv_string(T* value)
{
    if (value) {
        return to_kv_string(*value);
    } else {
        return String("NULL");
    }
}

template<typename T>
String to_kv_string(const std::shared_ptr<T>& value)
{
   if (value) {
        return value->to_kv_string();
    } else {
        return String("NULL");
    }
}

template<typename T>
String to_kv_string(const std::vector<T>& value)
{
    String str = "[";
   for (u32 i = 0; i < value.size(); ++i) {
       str += ::to_kv_string(value[i]) + ",";
   }
   str[str.length() - 1] = ']';
   return str;
}

template<typename T, typename ...ARGS>
String to_kv_string(const T& value, ARGS... args)
{
    return ::to_kv_string(value) + "," + ::to_kv_string(args...);
}

#endif	//PRINT_HELPER_H
