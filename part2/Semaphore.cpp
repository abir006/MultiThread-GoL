//
// Created by Abir on 23/05/2019.
//
#include "Semaphore.hpp"
Semaphore::Semaphore(){
    available_resource = 0;
    pthread_mutexattr_t m_lock_attr;
    pthread_mutexattr_init(&m_lock_attr);
    pthread_mutexattr_settype(&m_lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&m_lock,&m_lock_attr);
    pthread_mutexattr_destroy(&m_lock_attr);
    pthread_cond_init(&sem_cond,nullptr);
}
Semaphore::Semaphore(unsigned val){
    available_resource = val;
    pthread_mutexattr_t  m_lock_attr;
    pthread_mutexattr_init(&m_lock_attr);
    pthread_mutexattr_settype(&m_lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&m_lock,&m_lock_attr);
    pthread_mutexattr_destroy(&m_lock_attr);
    pthread_cond_init(&sem_cond,nullptr);
}
void Semaphore::up(){
    pthread_mutex_lock(&m_lock);
    available_resource++;
    pthread_cond_signal(&sem_cond);
    pthread_mutex_unlock(&m_lock);
}
void Semaphore::down(){
    pthread_mutex_lock(&m_lock);
    while(available_resource==0) {
        pthread_cond_wait(&sem_cond,&m_lock);
    }
    available_resource--;
    pthread_mutex_unlock(&m_lock);
}
