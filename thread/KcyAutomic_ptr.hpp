//
//  KcyAutomic_ptr.hpp
//  thread
//
//  Created by imkcy on 15/12/5.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef KcyAutomic_ptr_hpp
#define KcyAutomic_ptr_hpp

#include "KcyLock.hpp"
//交换原子指针
namespace kcy
{
    template<typename T>
    class KcyAutomic_ptr
    {
    public:
        inline KcyAutomic_ptr():ptr(NULL)
        {
        }
        inline ~KcyAutomic_ptr()
        {
        }
        
        inline void set(T* ptr_)
        {
            ptr = ptr_;
        }
        
        inline T* exchange(T* val_) //交换指针
        {
            mutex.lock();
            T* old = (T*)ptr;
            ptr = val_;
            mutex.unlock();
            return old;
        }
        
        inline T* cas(T* comp_,T* val_) //比较交换指针
        {
            mutex.lock();
            T* old = (T*)ptr;
            if (ptr == comp_) {
                ptr = val_;
            }
            mutex.unlock();
            return old;
        }
        
    private:
        volatile T* ptr; //保存指针
        KcyLock mutex;
        KcyAutomic_ptr(const KcyAutomic_ptr&);
        const KcyAutomic_ptr& operator=(const KcyAutomic_ptr&);
    };
}

#endif /* KcyAutomic_ptr_hpp */
