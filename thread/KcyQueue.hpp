//
//  KcyQueue.hpp
//  thread
//
//  Created by imkcy on 15/12/5.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef KcyQueue_hpp
#define KcyQueue_hpp

#include "KcyLock.hpp"
#include "KcyAutomic_ptr.hpp"
#include <assert.h>
namespace kcy
{
    template<typename T, int N>
    class KcyQueue
    {
    public:
        inline KcyQueue()
        {
            begin_chunk = (chunk_t*)malloc(sizeof(chunk_t));
            assert(begin_chunk);
            begin_pos = 0;
            back_chunk = NULL;
            back_pos = 0;
            end_chunk = begin_chunk;
            end_pos = 0;
        }
        inline ~KcyQueue()
        {
            while (begin_chunk != end_chunk) {
                chunk_t* temp = begin_chunk;
                free(temp);
                begin_chunk = begin_chunk->next;
            }
            free(begin_chunk);
            free(spare_chunk.exchange(NULL));
        }
        inline T& front()
        {
            return begin_chunk->values[begin_pos];
        }
        inline T& back()
        {
            return back_chunk->values[back_pos];
        }
        inline void push()
        {
            back_chunk = end_chunk;
            back_pos = end_pos;
            if (++end_pos != N) {
                return;
            }
            
            chunk_t* sc = spare_chunk.exchange(NULL);
            if (sc) {
                end_chunk->next = sc;
                sc->prev = end_chunk;
            }else {
                end_chunk->next = (chunk_t*)malloc(sizeof(chunk_t));
                assert(end_chunk->next);
                end_chunk->next->prev = end_chunk;
            }
            end_chunk = end_chunk->next;
            end_pos = 0;
        }
        inline void unpush()
        {
            if(back_pos)
                --back_pos;
            else
            {
                back_pos = N - 1;
                back_chunk = back_chunk->prev;
            }
            
            if (end_pos)
            {
                --end_pos;
            } else
            {
                end_pos = N - 1;
                end_chunk = end_chunk->prev;
                free(end_chunk->next);
                end_chunk->next = NULL;
            }
        }
        
        inline void pop()
        {
            if (++begin_pos != N) {
                return;
            }
            chunk_t* temp = begin_chunk;
            begin_chunk = begin_chunk->next;
            begin_chunk->prev = NULL;
            begin_pos = 0;
            
            chunk_t* o = spare_chunk.exchange(temp);
            free(o);
        }
    private:
        struct chunk_t
        {
            T values[N];
            chunk_t* prev;
            chunk_t* next;
        };
        
        chunk_t* begin_chunk;
        int begin_pos;
        chunk_t* back_chunk;
        int back_pos;
        chunk_t* end_chunk;
        int end_pos;
        
        KcyAutomic_ptr<chunk_t> spare_chunk;
        
        KcyQueue(const KcyQueue&);
        const KcyQueue& operator=(const KcyQueue&);
    };
    
}

#endif /* KcyQueue_hpp */
