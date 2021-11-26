#ifndef PRINT_HELPER_H
#define PRINT_HELPER_H
#include <sstream>
#include "type.h"
#define VAR_NAME(var)   #var
#define K(arg) String(VAR_NAME(arg)) + ":" + ::to_kv_string(arg)
#define V(arg) ::to_kv_string(arg)
#define N(arg) String(VAR_NAME(arg))
#define KV(arg1, arg2) N(arg1) + ":" + V(arg2)
#define KV_STRING(...) String \
to_kv_string()const { return String("{") + ::to_kv_string(__VA_ARGS__) + String("}"); }
#define VIRTUAL_KV_STRING(...) virtual KV_STRING(__VA_ARGS__)
#define DECLARE_KV_STRING String to_kv_string()const;
#define DECLARE_KV_STRING_OVERRIDE String to_kv_string()const override;

#define DEFINE_KV_STRING(CLASS, ...) String \
CLASS::to_kv_string()const { return String("{") + ::to_kv_string(__VA_ARGS__) + String("}"); }

#define KV_STRING_OVERRIDE(...) String \
to_kv_string()const override { return String("{") + ::to_kv_string(__VA_ARGS__) + String("}"); }

String to_kv_string(const long long& value);
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
String to_kv_string(const shared_ptr<T>& value)
{
   if (value) {
        return value->to_kv_string();
    } else {
        return String("NULL");
    }
}

template<typename T>
String to_kv_string(const Vector<T>& value)
{
    String str = "[";
   for (u32 i = 0; i < value.size(); ++i) {
       str += ::to_kv_string(value[i]) + ",";
   }
   if (str.length() > 1) {
       str[str.length() - 1] = ']';
   } else {
       str += "]";
   }
   return str;
}

template<typename T, typename U>
String to_kv_string(const UnorderedHashMap<T, U>& value)
{
    String str = "[";
   for (auto iter = value.cbegin(); iter != value.cend(); ++iter) {
       str += "{ key:" + ::to_kv_string(iter->first) + ", value:" + ::to_kv_string(iter->second) + "},";
   }
   if (str.length() > 1) {
       str[str.length() - 1] = ']';
   } else {
       str += "]";
   }
   return str;
}

template<typename T, typename ...ARGS>
String to_kv_string(const T& value, ARGS... args)
{
    return ::to_kv_string(value) + "," + ::to_kv_string(args...);
}

#endif	//PRINT_HELPER_H
