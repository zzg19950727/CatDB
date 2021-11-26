#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H
#include <string>
#include <mutex>
#include <vector>
#include <map>

namespace CatDB {
    namespace Share {
        class MemoryMonitor {
        public:
            ~MemoryMonitor();
            static MemoryMonitor &make_memory_monitor();
            void use_memory(unsigned long long size);
            void free_memory(unsigned long long size);
            void trace_memory(void *ptr, bool release);
            void clear_trace(void *ptr);
            void set_trace_mode(const std::string &mode) { trace_mode = mode; }
            std::string get_trace_mode() const { return trace_mode; }

            struct MemInfo {
                MemInfo(unsigned long long size)
                    :use_count(size)
                {}
                unsigned long long use_count;
                std::map<void*, std::vector<std::string>> reference_trace;
                std::map<void*, std::vector<std::string>> release_trace;
            };
            std::map<std::string, MemInfo*> mem_used;
        private:
            std::mutex m_mutex;
            std::string trace_mode;
        };
    }
}

#endif  //MEMORY_MONITOR_H