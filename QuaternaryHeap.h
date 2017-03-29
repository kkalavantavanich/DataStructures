/**
Author : Kris
Date : 29 March 2017

Guarantees : Parent is more than Children.
But Children are not sorted.

*/

#include <iosfwd>
#ifndef QUATERNARY_HEAP_H
#define QUATERNARY_HEAP_H

template <typename T, typename Comp = std::less<T> >
class quaternary_heap{
protected:
    T* mData;
    size_t mCap;
    size_t mSize;
    Comp mLess;

    void fixUp(size_t c) {
        T tmp = mData[c];
        while(c > 0) {
            size_t p = (c-1) / 4;
            if (mLess(tmp, mData[p])) break;
            mData[c] = mData[p];
            c = p;
        }
        mData[c] = tmp;
    }

    void fixDown (size_t p) {
        T tmp = mData[p];
        size_t c;
        size_t m;
        while((c = 4*p + 1) < mSize) {
            m = c;
            for (size_t i = 1; i <= 3; i++) {
                if (c+i >= mSize) break;
                else if (mLess(mData[m], mData[c+i]) m = c+i;
            }
            if (mLess(mData[m], tmp)) break;
            mData[p] = mData[m];
            p = m;
        }
    }

    void expand (size_t capacity) {
        T *arr = new T[capacity]();
        for (size_t i = 0; i < mSize; i++) arr[i] = mData[i];
        delete[] mData;
        mData = arr;
        mCap = capacity;
    }

public:
    quaternary_heap(const Comp &c = Comp()) {
        mCap = 1;
        mData = new T[mCap]();
        mSize = 0;
        mLess = c;
    }
    quaternary_heap(const quaternary_heap<T, Comp> &a) {
        mCap = a.mCap;
        mData = new T[mCap]();
        for (size_t i = 0; i < a.mCap; i++) {
            mData[i] = a.mData[i];
        }
        mSize = a.mSize;
        mLess = a.mLess;
    }
    ~quaternary_heap() {
        delete[] mData;
    }

    // CAPACITY FUNCTIONS //
    bool empty const { return mSize == 0; }
    size_t size() const { return mSize; }

    // ACCESS //
    const T& top() const {
        return mData[0];
    }

    // MODIFIERS //
    void push (const T &e) {
        if (mSize+1 > mCap) expand(mCap * 4);
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


#endif // QUATERNARY_HEAP_H
