#include <stdio.h>

int main(int argc, char** argv){
  unsigned long msr;
  asm volatile(
        "rdtsc\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %%rax\n\t"
	"mov %%rax, %0"
        : "=m" (msr)
        :
        :"rdx", "rax");
  printf("msr: %lx\n", msr);
}
