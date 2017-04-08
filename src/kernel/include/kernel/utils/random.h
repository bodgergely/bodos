/*
 * random.h
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_UTILS_RANDOM_H_
#define SRC_KERNEL_INCLUDE_KERNEL_UTILS_RANDOM_H_

/*
 * standard C lib signature (but return unsigned instead of int)
 */

#include <kernel/utils/rdtsc.h>

#define RAND_MAX 65536

unsigned rand();



#endif /* SRC_KERNEL_INCLUDE_KERNEL_UTILS_RANDOM_H_ */
