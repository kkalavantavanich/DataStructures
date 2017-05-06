/** Linear Singly LinkedList */
#ifndef LINKED_LIST_LS_H
#define LINKED_LIST_LS_H

#include<iosfwd>

template<typename T>
class linked_list{
    class node {
    public:
        T data;
        node *next;

        node() :
            data(T()),
            next(NULL)
            { }

        node(const T &data, node *next) :
            data(T(data)),
            next(next)
            { }
    }; // class node
protected:
    node *mFirst;
    size_t mSize;
public:
    /** Empty Constructor */
    linked_list():
        mFirst(NULL),
        mSize(0) {}

    /** Array Constructor */
    linked_list(T* others, size_t n) {
        for (int i = 0; i < n; i++) {
            mFirst = new node(*others, mFirst);
            others++;
        }
    }

    /** Copy Constructor*/
    linked_list(const linked_list &other):
        mFirst(NULL),
        mSize(other.mSize)
        {
        node *p = other.mFirst;
        while (p != NULL) {
            mFirst = new node(p->data, mFirst);
            p = p->next;
        }
        if (mFirst != NULL){
            node *q, *prev;
            p = mFirst;
            q = p->next;
            p->next = NULL;
            prev = p;
            p = q;
            for (int i = 1; i < mSize; i++) {
                q = p->next;
                p->next = prev;
                prev = p;
                p = q;
            }
            mFirst = prev;
        }
    }

    /** Destructor */
    ~linked_list(){
        clear();
    }

    /** Copy Assignment Operator*/
    linked_list &operator=(const linked_list &other) {
        if (this != &other) {
            mSize = other.mSize;
            node *p = other.mFirst, *r = NULL;
            while (p != NULL) {
                r = new node(p->data, r);
                p = p->next;
            }
            if (r != NULL){
                node *q, *prev;
                p = r;
                q = p->next;
                p->next = NULL;
                prev = p;
                p = q;
                for (int i = 1; i < mSize; i++) {
                    q = p->next;
                    p->next = prev;
                    prev = p;
                    p = q;
                }
                r = prev;
            }

            node *q;
            p = mFirst;
            while(p != NULL) {
                q = p->next;
                delete p;
                p = q;
            }
            mFirst = r;
        }
        return *this;
    }

    void clear() {
        node *q, *p = mFirst;
        while(p != NULL) {
            q = p->next;
            delete p;
            p = q;
        }
        mFirst = NULL;
        mSize = 0;
    }


    size_t size() { return mSize; }
    bool empty()  { return mSize == 0; }

    void push_front(const T& x) {
        mFirst = new node(x, mFirst);
        mSize++;
    }

    friend std::ostream &operator<< (std::ostream &os, const linked_list &ll) {
        os << "linked_list[";
        node *p = ll.mFirst;
        while(p != NULL) {
            os << p->data;
            p = p->next;
            if(p != NULL) os<< ",";
        }
        os << "]";
    }

    // assumes that linking chain is not broken and mSize is set correctly. //
    bool hasLoop() {
        node *t, *p;
        t = mFirst;
        for (int j = 0; j < mSize; j++) {
            p = t->next;
            for (int i = 1; i < mSize - j; i++) {
                if (p == t) return true;
                p = p->next;
            }
        }
    }

    void remove(const T& val) {
        if (mFirst == NULL) return;
        if (mFirst->data == val) {
            node *n = mFirst;
            mFirst = mFirst->next;
            mSize--;
            delete n;
            return;
        }
        node *p = mFirst;
        while (p->next != NULL){
            if (p->next->data == val){
                node *n = p->next;
                p->next = p->next->next;
                mSize--;
                delete n;
                return;
            }
            p = p->next;
        }
        return;
    }
};


#endif //LINKED_LIST_LS_H
