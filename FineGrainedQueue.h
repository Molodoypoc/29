#pragma once
#include <mutex>

struct Node {
    int value;
    Node* next;
    std::mutex* node_mutex;

    Node(int val, Node* nxt = nullptr) : value(val), next(nxt), node_mutex(new std::mutex) {}
    ~Node() {
        delete node_mutex;
    }
};

template <typename T>
class FineGrainedQueue {
  private:
    Node* head;
    std::mutex* queue_mutex;

  public:
    FineGrainedQueue(int value);
    ~FineGrainedQueue();
    void insertIntoMiddle(int value, int pos);
    void sync_insertIntoMiddle (int value, int pos);
};