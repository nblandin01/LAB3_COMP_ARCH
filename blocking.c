#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define XSIZE 100
#define YSIZE 100
#define NUMBLOCKS 256   // for 16kB its 256; indexShift = 4; tagShift = 12; mask = 0xFF
						// for 128kB its 1024; indexShift = 5; tagShift = 15; mask = 0x3FF

// Block Struct
struct Block {
	uint32_t valid;
	uint32_t tag;
};

int min(int a, int b){
	return a < b ? a : b;
}

int main() {

	int x[XSIZE][YSIZE];
	int y[XSIZE][YSIZE];
	int z[XSIZE][YSIZE];

	int i, j, k;
  	
  	// CHANGE PARAMATERS
  	int bSize = 11; // Blocking Factor
  	int32_t indexShift = 4;
	uint32_t tagShift = 12;
	uint32_t indexMask = 0xFF;

	// Miss/Hit Variables
	int miss = 0, hit = 0;
	struct Block cache[NUMBLOCKS];
	for(i=0;i < NUMBLOCKS; ++i){
		cache[i].valid = 0;
	}
	uint32_t addr;
	uint32_t index;
	uint32_t tag;

  	/* Initialize x matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {
			x[i][j] = 0;
		}
	}

  	/* Initialize y matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {
			y[i][j] = i + j + 2;
		}
	}

  	/* Initialize z matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {		
			z[i][j] = i + j + 2;
		}
	}

	// Blocking Matrix Multiply
	int p, q, kk, jj;
	int sum = 0;

	for(k = 0; k < XSIZE; k += bSize) {
		for(j = 0; j < YSIZE; j += bSize) {
			for (i = 0; i < XSIZE; ++i){
				for (jj = j; jj < min(j + bSize, YSIZE); ++jj){
					for (kk = k; kk < min(k + bSize, XSIZE); ++kk){

						// Check Hit/Miss - Y
						addr = (uintptr_t) &y[i][kk];
						index = (addr >> indexShift) & indexMask;
						tag = addr >> tagShift;

						if (cache[index].valid == 1 && cache[index].tag == tag){
							hit++;
						}
						else{
							miss++;
							cache[index].tag = tag;
							cache[index].valid = 1;
						}

						// Check Hit/Miss - Z
						addr = (uintptr_t) &z[kk][jj];
						index = (addr >> indexShift) & indexMask;
						tag = addr >> tagShift;

						if (cache[index].valid == 1 && cache[index].tag == tag){
							hit++;
						}
						else{
							miss++;
							cache[index].tag = tag;
							cache[index].valid = 1;
						}

						x[i][jj] += y[i][kk] * z[kk][jj];
					}
				}
			}
		}
	}


	// Print Result
	printf("Hit: %d\n", hit);
	printf("Miss: %d\n", miss);

	double miss_d = (double) miss;
	double hit_d = (double) hit;
	printf("Miss: %lf\n", 100 * (miss_d/(hit_d + miss_d)));

	return 0;
}
