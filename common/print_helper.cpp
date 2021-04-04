#include "print_helper.h"
#define TO_STRING_DEFINE(type) String to_kv_string(const type& value) \
                                        { \
                                            std::stringstream out; \
                                            out << value; \
                                            return out.str(); \
                                        }

TO_STRING_DEFINE(char);
TO_STRING_DEFINE(u16);
TO_STRING_DEFINE(u32);
TO_STRING_DEFINE(int);
TO_STRING_DEFINE(u64);
TO_STRING_DEFINE(f64);
TO_STRING_DEFINE(String);
TO_STRING_DEFINE(long);
TO_STRING_DEFINE(unsigned long);

String to_kv_string(const u8& value)
{
    return to_kv_string(u16(value));
}

String to_kv_string(const char *str)
{
    return String(str);
}

String to_kv_string(bool value)
{
    if (value) {
        return String("true");
    } else {
        return String("false");
    }
}