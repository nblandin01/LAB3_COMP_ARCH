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

int main(void) {
	// Matrix Variables
	int x[XSIZE][YSIZE];
	int y[XSIZE][YSIZE];
	int z[XSIZE][YSIZE];
	int i, j, k, r;

	// Miss/Hit Variables
	int miss = 0, hit = 0;
	uint32_t indexShift = 4;
	uint32_t tagShift = 12;
	uint32_t indexMask = 0xFF;
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
			
			// Initialize Matrix Element
			x[i][j] = 0;
		}
	}

	/* Initialize y matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {
			
			// Initialize Matrix Element
			y[i][j] = i + j;
		}
	}
	/* Initialize z matrix */
	for(i=0; i<XSIZE; i++) {
		for(j=0; j<YSIZE; j++) {
			
			// Initialize Matrix Element
			z[i][j] = i + j;
		}
	}

	/* Do matrix multiply */
	for(i=0; i<XSIZE; i=i+1) {
		for(j=0; j<YSIZE; j=j+1) {
			r = 0;
			for(k=0; k<XSIZE; k=k+1) {
				
				// Check Hit/Miss - Y
				addr = (uintptr_t) &y[i][k];
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
				addr = (uintptr_t) &z[k][j];
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

				// Update Matrix Element
				r = r + y[i][k] * z[k][j];
			}
			x[i][j] = r;
		}
	}

	fprintf(stdout, "Hits: %i\n", hit);
	fprintf(stdout, "Miss: %i\n", miss);
	// double miss_d = (double) miss;
	// double hit_d = (double) hit;
	// printf("Miss Rate: %lf %%\n", 100 * (miss_d/(hit_d + miss_d)));

	// Addresses
	// printf("\n");
	// printf("Addresses of x's\n");
	// printf("x[0][0]: %p\n",&x[0][0]);
	// printf("x[0][1]: %p\n",&x[0][1]);
	// printf("x[0][2]: %p\n",&x[0][2]);
	// printf("x[0][3]: %p\n",&x[0][3]);
	// printf("x[0][98]: %p\n",&x[0][YSIZE-2]);
	// printf("x[0][99]: %p\n",&x[0][YSIZE-1]);
	// printf("x[1][0]: %p\n",&x[1][0]);
	// printf("x[1][1]: %p\n",&x[1][1]);
	// printf("x[99][99]: %p\n",&x[99][99]);
	// printf("\n");
	// printf("Addresses of y's\n");
	// printf("y[0][0]: %p\n",&y[0][0]);
	// printf("y[0][1]: %p\n",&y[0][1]);
	// printf("y[0][2]: %p\n",&y[0][2]);
	// printf("y[0][3]: %p\n",&y[0][3]);
	// printf("y[0][98]: %p\n",&y[0][YSIZE-2]);
	// printf("y[0][99]: %p\n",&y[0][YSIZE-1]);
	// printf("y[1][0]: %p\n",&y[1][0]);
	// printf("y[1][1]: %p\n",&y[1][1]);
	// printf("y[99][99]: %p\n",&y[99][99]);
	// printf("\n");
	// printf("Addresses of z's\n");
	// printf("z[0][0]: %p\n",&z[0][0]);
	// printf("z[0][1]: %p\n",&z[0][1]);
	// printf("z[0][2]: %p\n",&z[0][2]);
	// printf("z[0][3]: %p\n",&z[0][3]);
	// printf("z[0][98]: %p\n",&z[0][YSIZE-2]);
	// printf("z[0][99]: %p\n",&z[0][YSIZE-1]);
	// printf("z[1][0]: %p\n",&z[1][0]);
	// printf("z[1][1]: %p\n",&z[1][1]);
	// printf("z[99][99]: %p\n",&z[99][99]);
}
