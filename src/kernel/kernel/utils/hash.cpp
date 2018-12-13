#include <kernel/utils/hash.h>

size_t integer_hash(unsigned int key)
{
    // https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key 
    unsigned int& x = key; 
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x =  (x >> 16) ^ x;
    return x;
}    

template<>
size_t hash<int>::operator()(const int key){ return integer_hash((unsigned int)key); }

template<>
size_t hash<unsigned int>::operator()(const unsigned int key){ return integer_hash(key); }