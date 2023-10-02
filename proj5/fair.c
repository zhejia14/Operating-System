#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)
//C11 spinlock
static inline int my_spin_lock (atomic_int *lock) {
    int val=0;
    if (likely(atomic_exchange_explicit(lock, 1, memory_order_acq_rel) == 0))/*如果lock＝0 就return 0  acq_rel:指令順序不能上下移動*/
        return 0;/*表示獲得lock*/
    do {
        do {
            asm("pause");/*主要目的是不希望cpu太熱*/
        } while (*lock != 0);/*直接判斷lock是否等於0（假設cache memory實現memory coherence）*/
        val = 0;
    } while (!atomic_compare_exchange_weak_explicit(lock, &val, 1, memory_order_acq_rel, memory_order_relaxed));
    /*比較 lock的值是否為0，獲得lock的thread就跳到return 0*/
    return 0;/*表示獲得lock*/
}
static inline int my_spin_unlock(atomic_int *lock) {/*unlock*/
    atomic_store_explicit(lock, 0, memory_order_release);/*將lock設定為0 release:順序在上的指令不能往下移*/
    return 0;
}
//C11 spinlock
atomic_int a_lock;
atomic_long count_array[256];
int numCPU;

void sigHandler(int signo) {
    for (int i=0; i<numCPU; i++) {
        printf("%i, %ld\n", i, count_array[i]);/*顯示進入cs的次數*/
    }
    exit(0);
}

atomic_int in_cs=0;
atomic_int wait=1;

void thread(void *givenName) {/*建立thread*/
    int givenID = (intptr_t)givenName;/*使用inptr_t轉換型別避免警告訊息  Get thread*/
    srand((unsigned)time(NULL));/*隨機*/
    unsigned int rand_seq;
    cpu_set_t set; CPU_ZERO(&set); CPU_SET(givenID, &set);
    sched_setaffinity(gettid(), sizeof(set), &set);/*第a個thread就在第a個core*/
    while(atomic_load_explicit(&wait, memory_order_acquire));/*將thread產生完之後，再把wait設成0，所有的thread會幾乎同時往下跑*/
    while(1) {
        my_spin_lock(&a_lock);/*lock*/
        atomic_fetch_add(&in_cs, 1);/*進入cs的時候就＋1*/
        atomic_fetch_add_explicit(&count_array[givenID], 1, memory_order_relaxed);/*計算lock的次數，如果有進入就＋1*/
        if (in_cs != 1) {
            printf("violation: mutual exclusion\n");/*若是不等於1 表示錯誤結束程式*/
            exit(0);
        }
        atomic_fetch_add(&in_cs, -1);/*離開cs的時候就-1*/
        my_spin_unlock(&a_lock);/*unlock*/
        int delay_size = rand_r(&rand_seq)%73;/*random，去避免隱含的同步效應*/
        for (int i=0; i<delay_size; i++);
    }
}

int main(int argc, char **argv) {
    signal(SIGALRM, sigHandler);
    alarm(5);
    numCPU = sysconf( _SC_NPROCESSORS_ONLN );/*cpu 個數*/
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * numCPU);/*分配thread需要的空間*/

    for (long i=0; i< numCPU; i++)
        pthread_create(&tid[i],NULL,(void *) thread, (void*)i);/*建立thread*/
    atomic_store(&wait,0);

    for (int i=0; i< numCPU; i++)
        pthread_join(tid[i],NULL);/*thread 需要join*/
}
