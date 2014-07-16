#include <stdlib.h>
#include <stdio.h>

#include "TimeWatch.hpp"

#define DEBUG 1
#define MAT_SIZE 2000
#define BLOCK_SIZE 16
#define NUM_THREADS 8

int A[MAT_SIZE][MAT_SIZE];
int B[MAT_SIZE][MAT_SIZE];
int C[MAT_SIZE][MAT_SIZE];
int matrix_size;

TimeWatch time_watch;

void *thread(void *vargp);
__global__ void MatMulKernel(const int*, const int*, int*);
__global__ void MatMulKernelShare(int*, int*, int*);


void initMatrix();
void dotMultiplyMatrix();
void multiplyMatrix();
void printMatrix();

void printTime(char* s){
	printf("%s: %.4lf\n", s, time_watch.getElapsedTime());
}

int main(void){

	// printf("Input matrix size: ");
	// scanf("%d\n", &matrix_size);
	initMatrix();

	/*   Using single thread CPU
	*/

	time_watch.startCount();
	multiplyMatrix();
	printTime("Multiply using CPU");
	// printMatrix();

	/*	Using multiple threads CPU
	*/
	initMatrix();
	time_watch.startCount();	
	pthread_t tid[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++){
		int startLine = MAT_SIZE / NUM_THREADS * i;
		pthread_create(&tid[i], NULL, thread, (void *)startLine);
	}
	
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(tid[i], NULL);

	printTime("Multiply using Multiple CPU");
	// printMatrix();	

	/* Using GPU
	*/

	size_t size = MAT_SIZE * MAT_SIZE * sizeof(int);
	int* d_A, *d_B;
	int* d_C;

	/*	Using GPU without Shared Memory
	*/

	time_watch.startCount();

	// Allocate A
	cudaError_t err = cudaMalloc((void**)&d_A, size);
	printf("CUDA malloc A: %s\n", cudaGetErrorString(err));
	err = cudaMemcpy(d_A, *A, size, cudaMemcpyHostToDevice);
	printf("Copy A to device: %s\n", cudaGetErrorString(err));

	// Allocate B
	err = cudaMalloc((void**)&d_B, size);
	printf("CUDA malloc B: %s\n", cudaGetErrorString(err));
	err = cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	printf("Copy B to device: %s\n", cudaGetErrorString(err));	

	// Allocate C
	err = cudaMalloc((void**)&d_C, size);
	printf("CUDA malloc C: %s\n", cudaGetErrorString(err));

	// Invoke kernel
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid((MAT_SIZE + dimBlock.x - 1) / dimBlock.x, 
		(MAT_SIZE + dimBlock.y - 1) / dimBlock.y);
	
	MatMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);

	err = cudaThreadSynchronize();
	printf("Run kernel: %s\n", cudaGetErrorString(err));

	// Copy to host
	err = cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	printf("Copy C from device: %s\n", cudaGetErrorString(err));
	
	printTime("Multiply Using GPU without Shared Memory");

	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	// printMatrix();

	/*	Using GPU with Shared Memory
	*/

	time_watch.startCount();

	err = cudaMalloc((void**)&d_A, size);
	printf("CUDA malloc A: %s\n", cudaGetErrorString(err));
	err = cudaMemcpy(d_A, *A, size, cudaMemcpyHostToDevice);
	printf("Copy A to device: %s\n", cudaGetErrorString(err));

	// Allocate B
	err = cudaMalloc((void**)&d_B, size);
	printf("CUDA malloc B: %s\n", cudaGetErrorString(err));
	err = cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	printf("Copy B to device: %s\n", cudaGetErrorString(err));	

	// Allocate C
	err = cudaMalloc((void**)&d_C, size);
	printf("CUDA malloc C: %s\n", cudaGetErrorString(err));

	// Invoke kernel
	dim3 dimBlock_Shared(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid_Shared(MAT_SIZE / dimBlock.x, MAT_SIZE / dimBlock.y);
	
	MatMulKernelShare<<<dimGrid_Shared, dimBlock_Shared>>>(d_A, d_B, d_C);

	err = cudaThreadSynchronize();
	printf("Run kernel: %s\n", cudaGetErrorString(err));

	// Copy to host
	err = cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	printf("Copy C from device: %s\n", cudaGetErrorString(err));
	
	printTime("Multiply Using GPU with Shared Memory");

	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	// printMatrix();

	return 0;

}

void initMatrix(){
	for (int i = 0; i < MAT_SIZE; i++)
		for (int j = 0; j < MAT_SIZE; j++){
			A[i][j] = i;
			B[i][j] = j;
			C[i][j] = 0;
		}
}

void dotMultiplyMatrix(){
	for (int i = 0; i < MAT_SIZE; i++)
		for (int j = 0; j < MAT_SIZE; j++){
			C[i][j] = A[i][j] * B[i][j]; 
		}
}

void multiplyMatrix(){
	for (int i = 0; i < MAT_SIZE; i++)
		for (int j = 0; j < MAT_SIZE; j++)
			for (int k = 0; k < MAT_SIZE; k++)
				C[i][j] += A[i][k] * B[k][j];
}

void printMatrix(){
	for (int i = 0; i < MAT_SIZE; i++){
		for (int j = 0; j < MAT_SIZE; j++){
			printf("%6d ", C[i][j]);
		}
		printf("\n");
	}
}

void *thread(void *vargp){
	int startLine = ((int)vargp);
	for (int i = startLine; i < startLine + MAT_SIZE / NUM_THREADS; i++)
		for (int j = 0; j < MAT_SIZE; j++)
			for (int k = 0; k < MAT_SIZE; k++)
				C[i][j] += A[i][k] * B[k][j];

	return NULL;
}


__global__ void MatMulKernel(const int* A, const int* B, int* C){
	// Each thread computes one element of C
	// by accumulatig results into Cvalue
	int Cvalue = 0;
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	if (row >= MAT_SIZE || col >= MAT_SIZE)
		return;
	for (int i = 0; i < MAT_SIZE; ++i)
		Cvalue += A[row * MAT_SIZE + i] * B[i * MAT_SIZE + col];

	C[row * MAT_SIZE + col] = Cvalue;
}

__device__ int GetElement(const int* A, int row, int col){
	return A[row * MAT_SIZE + col];
}

__device__ void SetElement(int* A, int row, int col, int value){
	A[row * MAT_SIZE + col] = value;
}

__device__ int* GetSubMatrix(int* A, int row, int col){
	int* Asub;
	Asub = &A[MAT_SIZE * BLOCK_SIZE * row + BLOCK_SIZE * col];

	return Asub;
}

__global__ void MatMulKernelShare(int* A, int* B, int* C){
	int blockRow = blockIdx.y, blockCol = blockIdx.x;
	int* Csub = GetSubMatrix(C, blockRow, blockCol);

	float Cvalue = 0.0;
	int row = threadIdx.y, col = threadIdx.x;

	for (int m = 0; m < (MAT_SIZE / BLOCK_SIZE); ++m){
		int* Asub = GetSubMatrix(A, blockRow, m);
		int* Bsub = GetSubMatrix(B, blockCol, m);

		__shared__ int As[BLOCK_SIZE][BLOCK_SIZE];
		__shared__ int Bs[BLOCK_SIZE][BLOCK_SIZE];

		As[row][col] = GetElement(Asub, row, col);
		Bs[row][col] = GetElement(Bsub, row, col);

		__syncthreads();

		for (int e = 0; e < BLOCK_SIZE; ++e)
			Cvalue += As[row][e] * Bs[e][col];

		__syncthreads();
	}

	SetElement(Csub, row, col, Cvalue);

}