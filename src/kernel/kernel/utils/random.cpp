/*
 * random.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */
#include <kernel/utils/random.h>



unsigned rand(unsigned long long limit)
{
	return rdtsc() % limit;
}


unsigned rand()
{
	rand(RAND_MAX);
}


