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
  	int bSize = 13; // Blocking Factor
  	int32_t indexShift = 4;
	uint32_t tagShift = 12;
	uint32_t indexMask = 0xFF;

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
	int a, b, c, d;

	for(k = 0; k < XSIZE; k += bSize) {
		for(j = 0; j < YSIZE; j += bSize) {
			for (i = 0; i < XSIZE; i += 2){
				for (jj = j; jj < min(j + bSize, XSIZE); jj += 2){
					a = b = c = d = 0;
					for (kk = k; kk < min(k + bSize, YSIZE); ++kk){

						a += y[i][kk] * z[kk][jj];
						b += y[i+1][kk] * z[kk][jj];
						c += y[i][kk] * z[kk][jj+1];
						d += y[i+1][kk] * z[kk][jj+1];

					}
					x[i][jj] += a;
					x[i+1][jj] += b;
					x[i][jj+1] += c;
					x[i+1][jj+1] += d;
				}
			}
		}
	}

	/* Initialize y matrix */
	// printf("Matrix Y\n");
	// for(i=0; i<XSIZE; i++) {
	// 	for(j=0; j<YSIZE; j++) {
	// 		printf("%d ", y[i][j]);
	// 	}
	// 	printf("\n");
	// }

 //  	/* Initialize z matrix */
 //  	printf("\nMatrix Z\n");
	// for(i=0; i<XSIZE; i++) {
	// 	for(j=0; j<YSIZE; j++) {		
	// 		printf("%d ", z[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// Print Matrix
	// printf("\nMatrix X\n");
	// for(i=0; i<XSIZE; i++) {
	// 	for(j=0; j<YSIZE; j++) {
	// 		printf("%d ", x[i][j]);
	// 	}
	// 	printf("\n");
	// }

	return 0;
}
