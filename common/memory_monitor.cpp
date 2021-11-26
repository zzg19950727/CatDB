#include "memory_monitor.h"
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>


using namespace CatDB::Share;

std::string handle_backtrace() 
{
  void *array[128];
  size_t size = backtrace(array, 128);
  char tmp[32] = {0};
  std::string ret;
  for (int i = 0; i < size; ++i) {
      sprintf(tmp, "%p ", array[i]);
      ret += std::string(tmp);
  }
  return ret;
}

std::string get_mode_name(size_t MODE_POS = 4)
{
	std::string mode = "SYSTEM";
    return mode;
    void *array[128];
    size_t size = backtrace(array, 128);
    if (size > MODE_POS) {
        char **symbols = backtrace_symbols(array+MODE_POS, 1);
        if (symbols != NULL) {
            mode = std::string(symbols[0]);
            int start_pos = 0;
            int end_pos = 0;
            for (int i = 0; i < mode.size(); ++i) {
                if (mode[i] == '_') {
                    if (start_pos > 0) {
                        if (end_pos > 0) {
                            start_pos = end_pos;
                            end_pos = i;
                        } else {
                            end_pos = i;
                        }
                    } else {
                        start_pos = i;
                    }
                }
            }
            while(start_pos < end_pos) {
                if (mode[start_pos] == '_' || 
                    (mode[start_pos] >= '0' && mode[start_pos] <= '9')) {
                    ++start_pos;
                } else {
                    break;
                }
            }
            while(start_pos < end_pos) {
                if (mode[end_pos] == '_' || 
                    (mode[end_pos] >= '0' && mode[end_pos] <= '9') ||
                    (mode[end_pos] >= 'A' && mode[end_pos] <= 'Z')) {
                    --end_pos;
                } else {
                    break;
                }
            }
            std::string copy = mode;
            mode = "";
            for (int i = start_pos; i <= end_pos; ++i) {
                if (copy[i] >= '0' and copy[i] <= '9') {
                    mode += "::";
                    if (copy[i+1] >= '0' and copy[i+1] <= '9') {
                        ++i;
                    }
                } else {
                    mode += copy[i];
                }
            }
            free(symbols);
        }
    }
	return mode;
}

MemoryMonitor::~MemoryMonitor()
{
    for (auto iter = mem_used.begin(); iter != mem_used.end(); ++iter) {
        delete iter->second;
        iter->second = NULL;
    }
}

MemoryMonitor &MemoryMonitor::make_memory_monitor()
{
    static MemoryMonitor monitor;
    return monitor;
}

void MemoryMonitor::use_memory(unsigned long long size)
{
    std::string mode = get_mode_name();
    std::lock_guard<std::mutex> lock(m_mutex);
    if (mem_used.find(mode) != mem_used.end()) {
        mem_used[mode]->use_count += size;
    } else {
        mem_used[mode] = new MemInfo(size);
    }
}

void MemoryMonitor::free_memory(unsigned long long size)
{
    std::string mode = get_mode_name();
    std::lock_guard<std::mutex> lock(m_mutex);
    if (mem_used.find(mode) != mem_used.end()) {
        mem_used[mode]->use_count -= size;
    }
}

void MemoryMonitor::trace_memory(void *ptr, bool release)
{
    std::string mode = get_mode_name(3);
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string addr = handle_backtrace();
    if (mode == trace_mode && mem_used.find(mode) != mem_used.end()) {
        MemInfo *info = mem_used[mode];
        if (release) {
            if (info->release_trace.find(ptr) != info->release_trace.end()) {
                info->release_trace[ptr].push_back(addr);
            } else {
                info->release_trace[ptr] = std::vector<std::string>();
                info->release_trace[ptr].push_back(addr);
            }
        } else {
            if (info->reference_trace.find(ptr) != info->reference_trace.end()) {
                info->reference_trace[ptr].push_back(addr);
            } else {
                info->reference_trace[ptr] = std::vector<std::string>();
                info->reference_trace[ptr].push_back(addr);
            }
        }
    }
}

void MemoryMonitor::clear_trace(void *ptr)
{
    std::string mode = get_mode_name();
    std::lock_guard<std::mutex> lock(m_mutex);
    if (mem_used.find(mode) != mem_used.end()) {
        MemInfo *info = mem_used[mode];
        info->release_trace.erase(ptr);
        info->reference_trace.erase(ptr);
    }
}