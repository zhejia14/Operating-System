#include <atomic.h>
#include "pthreadP.h"
#include <shlib-compat.h>
int __pthread_spin_lock(pthread_spinlock_t *lock) {
    int val=0;
    
    if(__glibc_likely(atomic_exchange_acquire(lock,1) == 0))/*假設spinlock一般都會成功，一開始就呼叫atomic_exchange_acquire函數，獲取lock舊的值，如果lock的值為0，回傳0*/
            return 0;/*表示獲得lock*/
    do{/*若lock的舊值為1，進入迴圈*//
        do{
            /* TODO Back-off. <--隨機等待一段時間，避免同時多個thread都要lock而使得硬體仲裁也沒有得到結果重複執行指令，直到只有一個人聽到lock*/
            atomic_spin_nop();/*在x86內相當於asm("pause") 主要目的是不希望cpu太熱*/
            val=atomic_load_relaxed(lock);/*載入 lock 到val*/

        }while(val!=0);/*一直測試val是否為0  這個迴圈較為輕量，花費的資源較少，減輕cpu負擔*/
        
    }while(!atomic_compare_exchange_weak_acquire(lock, &val, 1));
    /*比較 lock的值是否為0，其中多個thread內有一個成功執行此行，並且把lock的值設定為1，也就是再鎖上，獲得lock的thread就跳到return 0，
     剩下的其他thread繼續做spin   這個迴圈的判斷指令花費的資源較多，所以利用內部的迴圈減輕cpu負擔*/
    
    return 0;/*表示獲得lock*/
}
int __pthread_spin_unlock(pthread_spinlock_t *lock) {
    atomic_store_release(lock,0);/*將lock設定為0*/
    return 0;
}

