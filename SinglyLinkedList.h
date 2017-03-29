#include <iosfwd>
#ifndef SLL_H
#define SLL_H

template <typename T>
class Node {
protected:
    T     data;
    Node* next;
};

template <typename T>
class SinglyLinkedList {
protected:
    Node*   mHead;
    size_t  mSize;
public:
    SinglyLinkedList() : mHead(NULL), mSize(0) {}

    ~SinglyLinkedList() {

    }
    // CAPACITY FUNCTIONS //
    bool empty() const { return mHead == NULL; }
    const T& front() const { return mHead->data; }
    const size_t size() const { return mSize; }

    // MODIFIERS //
    void push_front(const T &e) {
        Node* n = new Node;
        n->data = e;
        n->next = mHead;
        mHead = n;
        mSize++;
    }
    void pop_front() {
        Node* o = mHead;
        mHead = o->next;
        delete o;
        mSize--;
    }
};


#endif // SLL_H
