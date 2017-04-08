/*
 * rdtsc.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */

#include "rdtsc.h"

unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

