#ifndef BODOS_HASH_H
#define BODOS_HASH_H

#include <stddef.h>

size_t integer_hash(unsigned int key);

template<class Key>
class hash
{
public:
    size_t operator()(const Key key);
};





#endif // !BODOS_HASH_H
