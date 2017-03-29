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

    // for checking cth element
    void fixUp(size_t c) {
        T tmp = mData[c];
        while (c > 0) {
            size_t p  = c / 2;
            if (mLess(tmp, mData[p])) break;
            mData[c] = mData[p];
            c = p;
        }
        mData[c] = tmp;
    }

    void fixDown (size_t p) {
        T tmp = mData[p];
        size_t c;
        while((c = 2 * p + 1) < mSize){
            if (c+1 < mSize && mLess(mData[c], mData[c+1])) c++;
            if (mLess(mData[c], tmp)) break;
            mData[p] = mData[c];
            p = c;
        }
        mData[p] = tmp;
    }

    void expand (size_t capacity) {
        T *arr = new T[capacity]();
        for (size_t i = 0; i < mSize; i++) arr[i] = mData[i];
        delete[] mData;
        mData = arr;
        mCap = capacity;
    }

public:
    priority_queue (const Comp &c = Comp()) {
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
        return mData[0];
    }
    // MODIFIERS //
    void push(const T &e){
        if (mSize+1 > mCap) expand(mCap * 2);
        mData[mSize] = e;
        mSize++;
        fixUp(mSize - 1);
    }

    void pop() {
        mData[0] = mData[mSize-1];
        mSize--;
        fixDown(0);
    }
};

#endif //PQUEUE_H
