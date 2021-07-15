#include "PCQueue.hpp"
template<class T>
    PCQueue<T>::PCQueue(): pcq_queue(),pcq_lock(),pcq_cond(),queue_size(0),push_sem(0) {
    pthread_mutexattr_t pcq_lock_attr;
    pthread_mutexattr_init(&pcq_lock_attr);
    pthread_mutexattr_settype(&pcq_lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&pcq_lock,&pcq_lock_attr);
    pthread_mutexattr_destroy(&pcq_lock_attr);
    pthread_cond_init(&pcq_cond,nullptr);
}

template <class T>
T PCQueue<T>::pop(){
    push_sem.down();
    pthread_mutex_lock(&pcq_lock);
    T temp = pcq_queue.front();
    pcq_queue.pop();
    pthread_mutex_unlock(&pcq_lock);
    return temp;
}

template <class T>
void PCQueue<T>::push(const T& item){
    pthread_mutex_lock(&pcq_lock);
    pcq_queue.push(item);
    pthread_mutex_unlock(&pcq_lock);
    push_sem.up();

}

template <class T>
void PCQueue<T>::push_all(vector<T>& pcq_helper_queue,int size){
    pthread_mutex_lock(&pcq_lock);
    for(int i=0;i<size;i++){
        pcq_queue.push(pcq_helper_queue[i]);
        //pcq_helper_queue.pop();
        push_sem.up();
    }
    pthread_mutex_unlock(&pcq_lock);
}

