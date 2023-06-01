#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Jerry Shi, jerryshi */
/* CS154, Autumn 2021 */
/* hw1 */

/* Question 1 */

int main() {
	int foo = 0x5;
	char endian = !((unsigned char*)&foo)[0];
	printf("hello world, from a %s-endian machine\n",
			endian ? "big" : "little");
	return endian;
}
