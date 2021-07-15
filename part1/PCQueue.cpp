#include "PCQueue.hpp"

template <class T> // Add definition in PCQueue.hpp if you need this constructor
PCQueue<T>::PCQueue(): pcq_queue(),pcq_lock(),pcq_cond(),queue_size(0),push_waiting(0) {
    pthread_mutex_attr_t = pcq_lock_attr;
    pthread_mutexattr_init(&pcq_lock_attr);
    pthread_mutexattr_settype(&pcq_lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&pcq_lock,&pcq_lock_attr);
    pthread_mutexattr_destroy(&pcq_lock_attr);
    pthread_cond_init(&pcq_cond);
}

template <class T>
T PCQueue<T>::pop(){
    pthread_mutex_lock(&pcq_lock);
    while(push_waiting>0 && queue_size<=0) {
        pthread_cond_wait(&pcq_cond,&pcq_lock);
    }
    T temp = pcq_queue.pop();
    queue_size--;
    pthread_mutex_unlock(&pcq_lock);
    return temp;
}

template <class T>
void PCQueue<T>::push(const T& item){
    push_waiting++;
    pthread_mutex_lock(&pcq_lock);
    pcq_queue.push(item);
    queue_size++;
    push_waiting--;
    pthread_cond_signal(&pcq_cond);
    pthread_mutex_unlock(&pcq_lock);
}