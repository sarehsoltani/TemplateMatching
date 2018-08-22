//Required Libraries
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "bmp_util.h"

////////////////////////////////////////////////////////////////////////////////
int getMatch(float *I, float *T, int Iw, int Ih, int Tw, int Th) {
	float *differences;
	float diff = 0;
	float temp;
	int counter = 0;
	size_t difference_size = sizeof(float) * (Iw - Tw + 1) * (Ih - Th + 1);
	differences = (float *)malloc(difference_size);
	int i, j, k, l;
	//move template on Image and  calculate differences between each pixels
	for (i = 0; i <= Ih - Th; i++) {
		for (j = 0; j <= Iw - Tw; j++) {
			for (k = 0; k < Th; k++) {
				for (l = 0; l < Tw; l++) {
					temp = I[((l + j) + (k + i)*Iw)] - T[l + k*Tw];
					diff += fabsf(temp);
				}
			}
			differences[j + i * (Iw - Tw + 1)] = (diff / ((float)(Th*Tw)));
			diff = 0;
		}
	}
	//find min
	counter = getMinimum(differences, (Iw - Tw + 1), (Ih - Th + 1), Th*Tw);
	return counter;
}
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
	int I_width, I_height, T_width, T_height;
	float *I, *T;
	int match_count = 0;
	int argIter = 1;
	int argc_temp = argc;
	//get image and template from user
	printf("Format: templatmatching.exe image1.bmp template1.bmp image2.bmp template2.bmp ... \n");
	argc_temp--;
	I = ReadBMP(argv[1], &I_width, &I_height);
	argc_temp--;
	argIter++;
	T = ReadBMP(argv[2], &T_width, &T_height);
	argc_temp--;
	//check Image size & Template size and compare with each other
	if (I == 0 || T == 0) {
		exit(1);
	}
	while (I != 0 && T != 0) {
		if (I_width < T_width || I_height < T_height) {
			fprintf(stderr, "Error: The template is larger than the picture\n");
			exit(EXIT_FAILURE);
		}
	}
}