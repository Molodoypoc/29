#include "FineGrainedQueue.h"
#include <queue>

template <typename T>
FineGrainedQueue<T>::FineGrainedQueue(int value)
    : head(new Node(value)), queue_mutex(new std::mutex){}

template <typename T> FineGrainedQueue<T>::~FineGrainedQueue(){
    Node* cur = this->head;
    while(cur){
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    delete queue_mutex;
}

template <typename T> void FineGrainedQueue<T>::insertIntoMiddle(int value, int pos){
    Node *prev, *cur;
    queue_mutex->lock();
    prev = this->head;
    cur = this->head->next;
    prev->node_mutex->lock();
    if(cur)
        cur->node_mutex->lock();
    queue_mutex->unlock();
    int counter = 1;
    while(cur && counter < pos){
        Node* old_prev = prev;
        prev = cur;        
        old_prev->node_mutex->unlock();
        cur = prev->next;
        if(cur) {
            cur->node_mutex->lock();
        }
        counter++;
    }
    if(cur == nullptr){
        prev->next = new Node(value);
        prev->node_mutex->unlock();
    } else if(counter == pos){
        if(cur->next){
            cur->next->node_mutex->lock();
            prev->next = new Node(
                value, cur->next);
            prev->node_mutex->unlock();
            cur->next->node_mutex->unlock();
            cur->node_mutex->unlock();
            delete cur;
        } 
        else{
            cur->node_mutex->unlock();
            prev->node_mutex->unlock();
        }
    }
}

template <typename T> void FineGrainedQueue<T>::sync_insertIntoMiddle(int value, int pos){
    Node *prev, *cur;
    prev = this->head;
    cur = this->head->next;
    int counter = 1;
    while(cur && counter < pos){
        Node* old_prev = prev;
        prev = cur;
        cur = cur->next;
        counter++;
    }
    if(cur == nullptr){
        prev->next = new Node(value);
    } else if(counter == pos){
        if(cur->next){
            prev->next = new Node(
                value, cur->next);
            delete cur;
        }
    }
}