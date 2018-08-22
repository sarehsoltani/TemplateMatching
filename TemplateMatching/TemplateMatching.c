//Required Libraries
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "bmp_util.h"

///////////////////////////////////////////////////////////////////////////////
// this function finds min of differences as one match 
int getMinimum(float * difference, int M, int N, int T_size) {
	float minimum = difference[0];
	int count = 0;
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			if (difference[i * M + j] < minimum) {
				minimum = difference[i * M + j];
				count = 0;
			}
			if (difference[i * M + j] == minimum && difference[i * M + j] <= (255 / T_size)) {
				count++;
			}
		}
	}
	//printf("%d\n", count);
	//printf("\n%f min: \n",min);
	return count;
}

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
//this function Flips templateTmage
void flip(float *T, int Tw, int Th) {
	int i, j;
	for (i = 0; i < Th; ++i) {
		for (j = 0; j < Tw / 2; ++j) {
			int k = Tw - 1 - j;
			float temp = T[i * Tw + j];
			T[i * Tw + j] = T[i * Tw + k];
			T[i * Tw + k] = temp;
			k--;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

void rotate(float* image, int cols, int rows) {
	int i, j, s, n;
	float* T_image = 0;
	int npixels_T = rows * cols;
	T_image = (float *)malloc(sizeof(float) * npixels_T);
	for (i = 0; i < cols; ++i) {
		for (j = 0; j < rows; ++j) {
			T_image[j + i*rows] = image[i + j*cols];
		}
	}

	flip(T_image, rows, cols);

	for (s = 0; s < cols; s++) {
		for (n = 0; n < rows; n++) {
			image[n + s*rows] = T_image[n + s*rows];
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

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
		// Find numbers of matches
		match_count = getMatch(I, T, I_width, I_height, T_width, T_height);
		// rotate  90 degree clockwise Template
		rotate(T, T_width, T_height);
		//Find numbers of matches between image and rotated template
		match_count += getMatch(I, T, I_width, I_height, T_height, T_width);
		//print number of matches
		printf("%d\n", match_count);
		//get more than one pic and template from user

	}
}