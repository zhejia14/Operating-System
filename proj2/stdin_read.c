#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
	char* input=malloc(sizeof(char));
	long len=1,ret;
	asm volatile(
		"mov $0, %%rax\n"//read system call number 0
		"mov $0, %%rdi\n"//stdin
		"mov %1, %%rsi\n"//input
		"mov %2, %%rdx\n"//input size
		"syscall\n"
		"mov %%rax, %0"	
		: "=m"(ret)
		: "g"(input), "g"(len)
		:"rax", "rdi", "rsi", "rdx"
	);
	printf("讀入的字元為%s\n", input);
	free(input);
	return 0;
}
