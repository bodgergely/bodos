
#ifndef BODOS_UNORDERED_MAP_H
#define BODOS_UNORDERED_MAP_H

#include <kernel/containers/queue.h>
#include <kernel/utils/hash.h>
#include <kernel/utils/utils.h>
#include <stdint.h>


namespace bodos
{

template<class Key>
class equal_to
{
public:
    bool operator()(const Key lhs, const Key rhs)
    {
        return lhs == rhs;
    }
};

template<class Key, class Val,
         class Hash = hash<Key>,
         class Pred = equal_to<Key>
         >
class unordered_map
{
    using Bucket = bodos::queue<pair<Key, Val>>;
    using hasher = Hash;
    using key_equal = Pred;
public:
    explicit unordered_map(size_t n = 8, const hasher& hf = hasher(),
                          const key_equal& eql = key_equal()) : hash_func(hf),
                                                                equal_func(eql)
    {
        buckets = allocate_buckets(n);
        bucket_count = n;
    }
    unordered_map(const unordered_map& ump);
    ~unordered_map() 
    { 
        if(buckets) 
        { 
            klog(INFO, "Will call delete[] on %d\n", buckets);
            delete[] buckets;
        } 
    }
    size_t  size() const { return item_count; }
    bool    insert(Key key, Val val)
    {
        Bucket& buck = find_bucket(key);
        if(buck.find([key](const pair<Key, Val>& elem)      // check if the key matches
                        { 
                            if(elem.first == key)
                                return true;
                            else 
                                return false; 
                        }))
        {
            return false;
        }
        if(buck.insert(make_pair(key, val))) { item_count++; return true; }
        else                 { return false; }
    }
    
    /**
     *  Find the item stored with the given key, return nullptr if not found 
     */
    Val*    find(Key key)
    {
        Bucket& buck = find_bucket(key);
        auto elem = buck.find([key](const pair<Key, Val>& elem)      // check if the key matches
                        { 
                            if(elem.first == key)
                                return true;
                            else 
                                return false; 
                        });
        if(!elem)
            return nullptr;
        return &elem->second;
        
    }
    
    /**
     * Erase the item with the given key
     */
    bool    erase(Key key)
    {
        Bucket& buck = find_bucket(key);
        auto elem = buck.find([key](const pair<Key, Val>& elem)      // check if the key matches
                        { 
                            if(elem.first == key)
                                return true;
                            else 
                                return false; 
                        });
        if(!elem) return false;
        auto el = *elem;
        if(buck.erase_item(el) > 0)
        {
            --item_count;
            return true;
        }
    } 

private:
    Bucket& find_bucket(Key key)
    {
        size_t hv = hash_func(key);
        return buckets[hv % bucket_count];
    }

    Bucket* allocate_buckets(int count) 
    { 
        klog(INFO, "Bucket array allocation starts\n");
        Bucket* b = new Bucket[count];
        if(!b) { klog(ERR, "unordered_map - Failed to allocate buckets"); HALT;}
        return b;
    }

private:
    Bucket*     buckets;
    int         bucket_count;
    hasher      hash_func;
    key_equal   equal_func;
    size_t      item_count = 0;
};


}

#endif
