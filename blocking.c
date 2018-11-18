#include <stdio.h>
#include <math.h>
#include <stdint.h>
// Compile with: gcc -Wall blocking.c -o blocking

#define XSIZE 100
#define YSIZE 100

int main() {

	int x[XSIZE][YSIZE];
	int y[XSIZE][YSIZE];
	int z[XSIZE][YSIZE];

	int i, j, k;
  
	// Initialize Variables for Miss/Hit
	int CACHESIZE = 128; // in kB
	int miss = 0, hit = 0;
	uint32_t tagMask = 0xfffff000;
	uint32_t indexMask = 0x00000ff0;
	uint32_t arr[CACHESIZE];
	for(i = 0;i < CACHESIZE; ++i) arr[i] = 0;
	uint32_t addr;
	uint32_t index;
	uint32_t tag;

  	/* Initialize x matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {

			addr = (uint32_t)&x[i][j];
			index = (addr & indexMask)>>4;
			tag = (addr & tagMask)>>12;
			
			if(arr[index] == 0) {
				miss++;
				arr[index] = tag;
			}
			else if (tag == arr[index]){
				hit++;
			}
			else{
				miss++;
			}
			x[i][j] = 0;
		}
	}

  	/* Initialize y matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {

			addr = (uint32_t)&y[i][j];
			index = (addr & indexMask)>>4;
			tag = (addr & tagMask)>>12;

			if(arr[index] == 0) {
				miss++;
				arr[index] = tag;
			}
			else if (tag == arr[index]){
				hit++;
			}
			else{
				miss++;
			}

			y[i][j] = i + j + 2;
		}
	}

  	/* Initialize z matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {
			
			addr = (uint32_t)&y[i][j];
			index = (addr & indexMask)>>4;
			tag = (addr & tagMask)>>12;

			if(arr[index] == 0) {
				miss++;
				arr[index] = tag;
			}
			else if (tag == arr[index]){
				hit++;
			}
			else{
				miss++;
			}

			z[i][j] = i + j + 2;
		}
	}

	// Blocking Matrix Multiply
	int p, q, kk, jj;
	int sum = 0;
	int bSize = 26; // Blocking Factor

	// Account for bSize that AREN'T an Integer Multiple of Matrix Size
	int n = 100; // Make equal shortest size of matrix
	int variant = bSize * (n / bSize);

	for (kk = 0; kk < variant; kk += bSize){
		for (jj = 0; jj < variant; jj += bSize){

			for (i = 0; i < n; ++i){
				// Check Min
				p = (jj + bSize > YSIZE) ? YSIZE : (jj + bSize);

				for (j = jj; j < p; ++j){
					// Check Min
					q = (kk + bSize > YSIZE) ? YSIZE : (kk + bSize);

					// Perform Calculation
					sum = 0;
					for (k = kk; k < q; ++k){
						sum += y[i][k] * z[k][j];

						// Check Cache Hit/Miss
						addr = (uint32_t)&y[i][j];
						index = (addr & indexMask)>>4;
						tag = (addr & tagMask)>>12;
						if(arr[index] == 0) {
							miss++;
							arr[index] = tag;
						}
						else if (tag == arr[index]){
							hit++;
						}
						else{
							miss++;
						}

						addr = (uint32_t)&z[i][j];
						index = (addr & indexMask)>>4;
						tag = (addr & tagMask)>>12;
						if(arr[index] == 0) {
							miss++;
							arr[index] = tag;
						}
						else if (tag == arr[index]){
							hit++;
						}
						else{
							miss++;
						}

					}
					x[i][j] += sum;
					addr = (uint32_t)&x[i][j];
					index = (addr & indexMask)>>4;
					tag = (addr & tagMask)>>12;
					if(arr[index] == 0) {
						miss++;
						arr[index] = tag;
					}
					else if (tag == arr[index]){
						hit++;
					}
					else{
						miss++;
					}
				}
			}
		}
	}


	// Print Result
	printf("Result for: %dkB and %dx%d\n", CACHESIZE, XSIZE, YSIZE);
	printf("Hit: %d\n", hit);
	printf("Miss: %d\n", miss);

	double miss_d = (double) miss;
	double hit_d = (double) hit;
	printf("Miss: %lf\n", 100 * (miss_d/(hit_d + miss_d)));

	return 0;
}
