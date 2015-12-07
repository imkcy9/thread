//
//  KcyPipe.hpp
//  thread
//
//  Created by imkcy on 15/12/5.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef KcyPipe_hpp
#define KcyPipe_hpp

#include "KcyQueue.hpp"


namespace kcy
{

    
    template <typename T, int N> class ypipe_base_t
    {
    public:
        virtual ~ypipe_base_t () {}
        virtual void write (const T &value_, bool incomplete_) = 0;
        virtual bool unwrite (T *value_) = 0;
        virtual bool flush () = 0;
        virtual bool check_read () = 0;
        virtual bool read (T *value_) = 0;
        virtual bool probe (bool (*fn)(T &)) = 0;
    };
}

namespace kcy {
    template<typename T,int N>
    class KcyPipe : public ypipe_base_t<T, N>
    {
    public:
        inline KcyPipe()
        {
            queue.push();
            w = r = f = &queue.back();
            c.set(&queue.back());
        }
        inline virtual ~KcyPipe()
        {

        }
        
        inline void write (const T &value_, bool incomplete_)
        {
            //  Place the value to the queue, add new terminator element.
            queue.back () = value_;
            queue.push ();
            
            //  Move the "flush up to here" poiter.
            if (!incomplete_)
                f = &queue.back ();
        }
        
        
        //  Pop an incomplete item from the pipe. Returns true is such
        //  item exists, false otherwise.
        inline bool unwrite (T *value_)
        {
            if (f == &queue.back ())
                return false;
            queue.unpush ();
            *value_ = queue.back ();
            return true;
        }
        
        //  Flush all the completed items into the pipe. Returns false if
        //  the reader thread is sleeping. In that case, caller is obliged to
        //  wake the reader up before using the pipe again.
        inline bool flush ()
        {
            //  If there are no un-flushed items, do nothing.
            if (w == f)
                return true;
            
            //  Try to set 'c' to 'f'.
            if (c.cas (w, f) != w) {
                
                //  Compare-and-swap was unseccessful because 'c' is NULL.
                //  This means that the reader is asleep. Therefore we don't
                //  care about thread-safeness and update c in non-atomic
                //  manner. We'll return false to let the caller know
                //  that reader is sleeping.
                c.set (f);
                w = f;
                return false;
            }
            
            //  Reader is alive. Nothing special to do now. Just move
            //  the 'first un-flushed item' pointer to 'f'.
            w = f;
            return true;
        }
        
        //  Check whether item is available for reading.
        inline bool check_read ()
        {
            //  Was the value prefetched already? If so, return.
            if (&queue.front () != r && r)
                return true;
            
            //  There's no prefetched value, so let us prefetch more values.
            //  Prefetching is to simply retrieve the
            //  pointer from c in atomic fashion. If there are no
            //  items to prefetch, set c to NULL (using compare-and-swap).
            r = c.cas (&queue.front (), NULL);
            
            //  If there are no elements prefetched, exit.
            //  During pipe's lifetime r should never be NULL, however,
            //  it can happen during pipe shutdown when items
            //  are being deallocated.
            if (&queue.front () == r || !r)
                return false;
            
            //  There was at least one value prefetched.
            return true;
        }
        
        //  Reads an item from the pipe. Returns false if there is no value.
        //  available.
        inline bool read (T *value_)
        {
            //  Try to prefetch a value.
            if (!check_read ())
                return false;
            
            //  There was at least one value prefetched.
            //  Return it to the caller.
            *value_ = queue.front ();
            queue.pop ();
            return true;
        }
        
        //  Applies the function fn to the first elemenent in the pipe
        //  and returns the value returned by the fn.
        //  The pipe mustn't be empty or the function crashes.
        inline bool probe (bool (*fn)(T &))
        {
            bool rc = check_read ();
            assert (rc);
            
            return (*fn) (queue.front ());
        }
        
    protected:
        KcyQueue<T, N> queue;
        T* w;
        T* r;
        T* f;
        
        KcyAutomic_ptr<T> c;
        
        KcyPipe(const KcyPipe&);
        const KcyPipe& operator=(const KcyPipe&);
    };
}
#endif /* KcyPipe_hpp */
