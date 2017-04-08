/*
 * RandomTest.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */

#include <kernel/tests/RandomTest.h>

void generateRandomNumbers(int count, int limit)
{
	for(int i=0;i<count;i++)
	{
		kprintf("%d\n", rand(limit));
	}
}

