1.在物理机下pthread_spin_lock比pthread_mutex_lock快一倍
(虚拟机里面测试结果是mutex比spin快,这个是有问题的,不能在虚拟机内测试)

2.最快的还是__sync_bool_compare_and_swap cas锁

3.如果是数据的累加应使用__sync_add_and_fetch

注意:

__atomic_store_n(&lock, 0); //gcc <4.7

__atomic_store_n(&lock, 0, __ATOMIC_SEQ_CST); // gcc >=4.7
adder0 sum:  40000000                          pthread_mutex_lock:6.330000  time:2
adder1 sum:  40000000          while __sync_bool_compare_and_swap:33.130000 time:8
adder2 sum:   4609605             if __sync_bool_compare_and_swap:3.580000  time:1
adder3 sum:  40000000                           pthread_spin_lock:8.970000  time:2
adder4 sum:   3655563                        pthread_spin_trylock:3.070000  time:1
adder5 sum:  40000000                        __sync_add_and_fetch:2.650000  time:0
adder6 sum:   4362557                    __sync_lock_test_and_set:3.210000  time:1
adder7 sum:  40000000      new while __sync_bool_compare_and_swap:0.000000  time:1
adder8 sum:  40000000   new old while __sync_bool_compare_and_swap:32.380000    time:8


addr7是原子操作(加了usleep(1)) 是最快的,不加usleep(1)和自旋锁一样

addr3是自旋锁   有点慢,lock是4个字节

addr0是mutex锁  但是锁是40多个字节

