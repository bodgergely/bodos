#ifndef BODOS_UTILS_H
#define BODOS_UTILS_H

#define HALT while(1);

namespace bodos
{

template<class A, class B>
struct pair
{
    pair(A first_, B second_) : first(first_),
                                second(second_) {}

    A   first;
    B   second;
};

template<class A, class B>
bool operator==(const pair<A, B>& lhs, const pair<A, B>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second; 
}

template<class A, class B>
struct pair<A, B> 
make_pair(A first, B second)
{
    return pair<A, B>(first, second);
}

}

#endif // !BODOS_UTILS_H

