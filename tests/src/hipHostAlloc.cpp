#include"test_common.h"

#define LEN 1024*1024
#define SIZE LEN*sizeof(float)

__global__ void Add(hipLaunchParm lp, float *Ad, float *Bd, float *Cd){
int tx = hipThreadIdx_x + hipBlockIdx_x * hipBlockDim_x;
Cd[tx] = Ad[tx] + Bd[tx];
}

int main(){
float *A, *B, *C;
float *Ad, *Bd, *Cd;

hipDeviceProp_t prop;
int device;
HIPCHECK(hipGetDevice(&device));
HIPCHECK(hipDeviceGetProperties(&prop, device));
if(prop.canMapHostMemory != 1){
std::cout<<"Exiting..."<<std::endl;
}
HIPCHECK(hipHostAlloc((void**)&A, SIZE, hipHostAllocWriteCombined | hipHostAllocMapped));
HIPCHECK(hipHostAlloc((void**)&B, SIZE, hipHostAllocWriteCombined | hipHostAllocMapped));
HIPCHECK(hipHostAlloc((void**)&C, SIZE, hipHostAllocMapped));

HIPCHECK(hipHostGetDevicePointer((void**)&Ad, A, 0));
HIPCHECK(hipHostGetDevicePointer((void**)&Bd, B, 0));
HIPCHECK(hipHostGetDevicePointer((void**)&Cd, C, 0));

for(int i=0;i<LEN;i++){
A[i] = 1.0f;
B[i] = 2.0f;
}

dim3 dimGrid(LEN/512,1,1);
dim3 dimBlock(512,1,1);

hipLaunchKernel(HIP_KERNEL_NAME(Add), dimGrid, dimBlock, 0, 0, Ad, Bd, Cd);

passed();

}
