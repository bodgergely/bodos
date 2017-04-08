/*
 * random.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */
#include <kernel/utils/random.h>

unsigned rand()
{
	return rdtsc() % RAND_MAX;
}



