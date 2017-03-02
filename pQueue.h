#include <iosfwd>
#ifndef PQUEUE_H
#define PQUEUE_H

template <typename T, typename Comp = std::less<T>>
class priority_queue {
protected:
    T* mData;
    size_t mCap;
    size_t mSize;
    Comp mLess;

public:
    priority_queue(const Comp &c = Comp()) {
        mCap = 1;
        mData = new T[mCap]();
        mSize = 0;
        mLess = c;
    }
    priority_queue(const priority_queue<T,Comp> &a) {
        mCap = a.mCap;
        mData = new T[mCap]();
        for (size_t i = 0; i < a.mCap; i++) {
            mData[i] = a.mData[i];
        }
        mSize = a.mSize;
        mLess = a.mLess;
    }

    ~priority_queue(){
        delete[] mData;
    }

    // CAPACITY FUNCTIONS //
    bool empty() const { return mSize == 0; }
    size_t size() const { return mSize; }
    // ACCESS //
    const T& top() const {

    }
    // MODIFIERS //
    void push(const T &e){

    }

    void pop() {

    }

};

#endif //PQUEUE_H
