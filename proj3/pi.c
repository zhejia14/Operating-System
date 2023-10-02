#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/syscall.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#define loopCount 100000000
char* exename;
long int numcpu=-1;
volatile double upper_buf[100], lower_buf[100];

void thread(void* giveName){
    long int id=(intptr_t)giveName;
    double left=(double)id/(double)numcpu;
    double right=(double)(id+1)/(double)numcpu;
    double x=0, y=0;
    double xdelta=(right-left)/loopCount;
    double MAX=0.0;
    
    for(int i=0;i<loopCount;i++){
        x=left+xdelta;
        y=sqrt(1.0-left*left);
        upper_buf[id]=upper_buf[id]+(xdelta*y);
        if(i==0)MAX=xdelta*y;
        left=left+xdelta;
    }
    lower_buf[id]=upper_buf[id]-MAX;
}





int main(int argc, char** argv){
    exename=argv[0];
    int check=atoi(argv[1]);
    numcpu =sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t* tid=(pthread_t*)malloc(sizeof(pthread_t)*numcpu);
    
    for(long int i=0;i<numcpu;i++)pthread_create(&tid[i], NULL, (void*)thread, (void*)i);
    for(long int i=0;i<numcpu;i++)pthread_join(tid[i], NULL);
    
    double up=0.0, low=0.0, pi=0.0;
    for(long int i=0;i<numcpu;i++){
        up=up+upper_buf[i];
        low=low+lower_buf[i];
    }
    printf("pi = %lf\n", ((up+low)/2)*4);
    return 0;
}
