#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "C:\Users\Sare\Documents\Visual Studio 2015\Projects\FinalProject\TemplateMatching\includes\bmp_util.h"
void check_omp();

int getMinimum(float * difference, int M, int N, int T_size) {
	omp_set_nested(1);
	float minimum = difference[0];
	int count = 0;
	int i, j;
#pragma omp parallel for 
	for (i = 0; i < N; i++) {
#pragma omp parallel for
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
	printf("\n%d count: \n", count);
	printf("\n%f min: \n", minimum);
	return count;
}

int getMatch(float *I, float *T, int Iw, int Ih, int Tw, int Th) {
	float *differences;
	float diff = 0;
	float temp;
	int counter = 0;
	omp_set_nested(1);
	//omp_lock_t writelock;
	size_t difference_size = sizeof(float) * (Iw - Tw + 1) * (Ih - Th + 1);
	differences = (float *)malloc(difference_size);
	int i, j, k, l;

	
#pragma omp parallel for private(i,j,k,temp,diff)
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
	counter = getMinimum(differences, (Iw - Tw + 1), (Ih - Th + 1), Th*Tw);
	return counter;

	/*for (int s = 0; s < 5; s++) {
	for (int n = 0; n < 5; n++) {
	printf("image: \t %f", I[s + n*Iw]);
	}
	printf("\n");
	}

	for (int s = 0; s < 5; s++) {
	for (int n = 0; n < 5; n++) {
	printf("template: \t %f", T[s + n*Tw]);
	}
	printf("\n");
	}
	*/
	/////////////////////////////////////////////////////////////////////////////////////
	/*
	for (int s = 0; s < Ih - Th; s++) {
	for (int n = 0; n < Iw - Tw; n++) {
	printf("\t %f", differences[n + s*(Iw - Tw)]);
	}
	printf("\n");
	}*/
}
////////////////////////////////////////////////////////////////////////////////
void flip(float *T, int Tw, int Th) {
	int i, j;
	omp_set_nested(1);
#pragma omp parallel for
	for (i = 0; i < Th; ++i) {
#pragma omp parallel for
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
	// rows== Th, cols == TW
	printf("hiiiiiiiii transpose\n\n");
	omp_set_nested(1);
#pragma omp parallel for
	for (i = 0; i < cols; ++i) {
#pragma omp parallel for
		for (j = 0; j < rows; ++j) {
			T_image[j + i*rows] = image[i + j*cols];
		}
	}
	/*for (int s = 0; s < cols; s++) {
	for (int n = 0; n < rows; n++) {
	printf("\t %f", T_image[n + s*rows]);
	}
	printf("\n");
	}*/
	flip(T_image, rows, cols);
	//printf("\n\n\n hiiiiiiiiiiiiiii dear rotate \n\n\n");
	//image = T_image;
#pragma omp parallel for
	for (s = 0; s < cols; s++) {
#pragma omp parallel for
		for (n = 0; n < rows; n++) {
			image[n + s*rows] = T_image[n + s*rows];
			//printf("\t %f", image[n + s*rows]);
		}
		//printf("\n");
	}

}

int main(int argc, char *argv[]) {
	omp_set_nested(1);
	/*int width = 5;
	int height = 5;
	int array[25] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
	for (int i = 0; i < height; ++i) {
	for (int j = 0; j < width / 2; ++j) {
	int k = width - 1 - j;
	short temp = array[i * width + j];
	array[i * width + j] = array[i * width + k];
	array[i * width + k] = temp;
	k--;
	}
	}

	for (int k = 0; k < width*height; k++) {
	printf("%d\t", array[k]);
	}
	*/
	///////////////////////////////////////////////////////////////////////////////
	/*int m, n, c, d, matrix[4], maximum;

	printf("Enter the number of rows and columns of matrix\n");
	scanf_s("%d%d", &m, &n);
	printf("Enter the elements of matrix\n");

	for (c = 0; c < m; c++)
	{
	for (d = 0; d < n; d++)
	{
	scanf_s("%d", &matrix[c*n +d]);
	}
	}

	for (c = 0; c < m; c++)
	{
	for (d = 0; d < n; d++)
	{
	printf("%d", matrix[c*n + d]);
	}
	}*/
	// Just an example here - you are free to modify them
	/////////////////////////////////////////////////////////////////
	double startTime, endTime;
	int I_width, I_height, T_width, T_height;
	float *I, *T;
	//int x1, y1, x2, y2;
	int match_count = 0;
	int argIter = 1;
	// set the file location of I, T, and Output


	printf("Format: templatmatching.exe image1.bmp template1.bmp image2.bmp template2.bmp ... \n");
		

	I = ReadBMP(argv[1], &I_width, &I_height);
	argIter++;
	T = ReadBMP(argv[2], &T_width, &T_height);


	if (I == 0 || T == 0) {
		exit(1);
	}

	while (I != 0 && T != 0) {
		if (I_width < T_width || I_height < T_height) {
			fprintf(stderr, "Error: The template is larger than the picture\n");
			exit(EXIT_FAILURE);
		}
		/*if (I_width < T_width || I_height < T_height) {
		fprintf(stderr, "Error: The template is larger than the picture\n");
		exit(EXIT_FAILURE);
		}
		*/

		startTime = omp_get_wtime();
		match_count = getMatch(I, T, I_width, I_height, T_width, T_height);


		//////////////////////////////////////////////////////////////////
		/*for (int s = 0; s < T_height; s++) {
		for (int n = 0; n < T_width; n++) {
		printf("\t %f", T[n + s*T_width]);
		}
		printf("\n");
		}*/

		//printf("Sareeeeeeeeeeeeee\n\n\n\n\n");
		rotate(T, T_width, T_height);

		match_count += getMatch(I, T, I_width, I_height, T_height, T_width);
		printf("\n number of matches: %d\n", match_count);
		endTime = omp_get_wtime() - startTime;
		printf("Time Elapsed %f Secs \n", endTime);
		/*
		for (int s = 0; s < T_height; s++) {
		for (int n = 0; n < T_width; n++) {
		printf("\t %f", T[n + s*T_width]);
		}
		printf("\n");
		}*/

		//printf("Result is put in: %s\n", argv[3]);
		argIter++;
		I = ReadBMP(argv[argIter], &I_width, &I_height);
		argIter++;
		T = ReadBMP(argv[argIter], &T_width, &T_height);
	}

	check_omp();
	free(I);
	free(T);

	return 0;
}


void check_omp() {
	// written by Ahmad Siavashi (siavashi@aut.ac.ir) April 26th, 2018
#ifdef _DEBUG
	printf("[!] Configuration: Debug.\n");
	printf("[#] Change to Release.\n");
#pragma message ("Change configuration to Release for a fast execution.")
#else
	printf("[-] Configuration: Release.\n");
#endif // _DEBUG
#ifdef _M_X64
	printf("[-] Platform: x64\n");
#elif _M_IX86
	printf("[-] Platform: x86\n");
#pragma message ("Change platform to x64 for more memory.")
#endif // _M_IX86
#ifdef _OPENMP
	printf("[!] OpenMP is on.\n");
	printf("[-] OpenMP version: %d\n", _OPENMP);
#else
	printf("[!] OpenMP is off.\n");
	printf("[#] Enable OpenMP.\n");
#endif // _OPENMP
	printf("[-] Maximum threads: %d\n", omp_get_max_threads());
	printf("[-] Nested Parallelism: %s\n", omp_get_nested() ? "On" : "Off");
#pragma message("Enable nested parallelism if you wish to have parallel region within parallel region.")
	printf("[#] Enable nested parallelism.\n");
}
