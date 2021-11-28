/**
 * @file  shared_ptr.hpp
 * @brief shared_ptr is a minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * Copyright (c) 2013-2019 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http:
 */
#pragma once
#include <cstddef>   
#include "memory_monitor.h"
#define MEM_TRACE
namespace CatDB {
    namespace Share {
    class shared_ptr_count
    {
    public:
        shared_ptr_count() :
            pn(NULL)
        {
        }
        shared_ptr_count(const shared_ptr_count& count) :
            pn(count.pn)
        {
        }
        shared_ptr_count(shared_ptr_count&& count) :
            pn(count.pn)
        {
            count.pn = NULL;
        }
        bool is_valid() const
        {
            return pn != NULL;
        }
        
        void swap(shared_ptr_count& lhs) throw() 
        {
            std::swap(pn, lhs.pn);
        }
        
        long use_count(void) const throw() 
        {
            long count = 0;
            if (NULL != pn)
            {
                count = *pn;
            }
            return count;
        }
        
        template<class U>
        void acquire(U* p) 
        {
            if (NULL != p)
            {
                if (NULL == pn)
                {
                    try
                    {
                        pn = new long(1); 
                    }
                    catch (std::bad_alloc&)
                    {
                        delete p;
                        throw; 
                    }
                    #ifdef MEM_TRACE
                    MemoryMonitor::make_memory_monitor().use_memory(sizeof(U));
                    #endif  //MEM_TRACE
                }
                else
                {
                    ++(*pn);
                }
            }
        }
        
        template<class U>
        void release(U* p) throw() 
        {
            if (NULL != pn)
            {
                --(*pn);
                if (0 == *pn)
                {
                    if (NULL != p) {
                        delete p;
                    }
                    delete pn;
                    #ifdef MEM_TRACE
                    MemoryMonitor::make_memory_monitor().free_memory(sizeof(U));
                    MemoryMonitor::make_memory_monitor().clear_trace(p);
                    #endif  //MEM_TRACE
                }
                pn = NULL;
            }
        }

    public:
        long*   pn; 
    };
    
    template<class T>
    class shared_ptr
    {
    public:
        typedef T element_type;
        shared_ptr(void) throw() : 
            px(NULL),
            pn()
        {
        }
        
        shared_ptr(T* p) : 
            pn()
        {
            acquire(p);   
        }
        
        template <class U>
        shared_ptr(const shared_ptr<U>& ptr, T* p) :
            pn(ptr.pn)
        {
            acquire(p);   
        }
        
        template <class U>
        shared_ptr(const shared_ptr<U>& ptr) throw() : 
            pn(ptr.pn)
        {
            acquire(static_cast<typename shared_ptr<T>::element_type*>(ptr.px));   
        }

        template <class U>
        shared_ptr(shared_ptr<U>&& ptr) throw() : 
            pn(std::move(ptr.pn))
        {
            px = static_cast<typename shared_ptr<T>::element_type*>(ptr.px);  
            ptr.px = NULL;
        }
        
        shared_ptr(const shared_ptr& ptr) throw() :
            pn(ptr.pn)
        {
            acquire(ptr.px);   
        }

        shared_ptr(shared_ptr&& ptr) throw() :
            pn(std::move(ptr.pn))
        {
            px = ptr.px;  
            ptr.px = NULL;
        }
        
        shared_ptr& operator=(const shared_ptr& ptr) throw() 
        {
            shared_ptr tmp(ptr);
            swap(tmp);
            return *this;
        }
        
        ~shared_ptr(void) throw() 
        {
            release();
        }
        
        void reset(void) throw() 
        {
            release();
        }
        
        void reset(T* p) 
        {
            
            release();
            acquire(p); 
        }

        void swap(shared_ptr& lhs) throw() 
        {
            std::swap(px, lhs.px);
            pn.swap(lhs.pn);
        }

        
        operator bool() const throw() 
        {
            return px;
        }
        bool unique(void)  const throw() 
        {
            return (1 == pn.use_count());
        }
        long use_count(void)  const throw() 
        {
            return pn.use_count();
        }

        T& operator*()  const throw() 
        {
            
            return *px;
        }

        T* operator->() const throw() 
        {
            
            return px;
        }

        T* get(void)  const throw() 
        {
            
            return px;
        }

    private:
        void acquire(T* p) 
        {
            #ifdef MEM_TRACE
            MemoryMonitor::make_memory_monitor().trace_memory(p, false);
            #endif  //MEM_TRACE
            pn.acquire(p); 
            px = p; 
        }

        void release(void) throw() 
        {
            #ifdef MEM_TRACE
            if (px)
            MemoryMonitor::make_memory_monitor().trace_memory(px, true);
            #endif  //MEM_TRACE
            pn.release(px);
            px = NULL;
        }

    private:
        template<class U>
        friend class shared_ptr;

    private:
        T*                  px; 
        shared_ptr_count    pn; 
    };
    
    template<class T, class U> bool operator==(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() 
    {
        return (l.get() == r.get());
    }
    template<class T, class U> bool operator!=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
    {
        return (l.get() != r.get());
    }
    template<class T, class U> bool operator<=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
    {
        return (l.get() <= r.get());
    }
    template<class T, class U> bool operator<(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
    {
        return (l.get() < r.get());
    }
    template<class T, class U> bool operator>=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
    {
        return (l.get() >= r.get());
    }
    template<class T, class U> bool operator>(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
    {
        return (l.get() > r.get());
    }
    
    template<class T, class U>
    shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) // never throws
    {
        return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type*>(ptr.get()));
    }

    template<class T, class U>
    shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) // never throws
    {
        T* p = dynamic_cast<typename shared_ptr<T>::element_type*>(ptr.get());
        if (NULL != p)
        {
            return shared_ptr<T>(ptr, p);
        }
        else
        {
            return shared_ptr<T>();
        }
    }

    }
}
