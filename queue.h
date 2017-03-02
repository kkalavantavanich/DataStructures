/** class  : Queue
  * author : Kris Kalavantavanich
  * date   : 27 Feb 2017
*/
#include <stdexcept>
#include <iosfwd>
#ifndef QUEUE_H
#define QUEUE_H
template <typename T>
class queue {
protected:
    T* mData;
    size_t mCap;
    size_t mSize;
    size_t mFront;

    void expand(size_t capacity) {
        T *arr = new T[capacity]();
        for (size_t i = 0; i < mSize ; i++) {
            arr[i] = mData[(i + mFront) % mCap];
        }
        delete[] mData;
        mData = arr;
        mCap = capacity;
        mFront = 0;
    }

    void ensureCapacity(size_t capacity) {
        if (capacity > mCap) {
            size_t s = capacity > 2*mCap ? capacity : 2*mCap;
            expand(s);
        }
    }

public:
    queue(){
        mCap = 1;
        mSize = 0;
        mData = new T[mCap]();
        mFront = 0;
    }
    queue(const queue<T> &a) {
        mData = new T[a.mCap]();
        mCap = a.mCap;
        mSize = a.mSize;
        for (size_t i = 0; i < a.mSize; i++) {
            mData[i] = a.mData[i];
        }
        mFront = a.mFront;
    }
    ~queue(){
        delete[] mData;
    }
    // CAPACITY //
    bool empty() { return mSize == 0; }
    size_t size() { return mSize; }
    // ACCESS //
    const T& front() {
        if (size() == 0)
            throw std::out_of_range("index out of range");
        return mData[mFront];
    }
    const T& back() {
        if (size() == 0)
            throw std::out_of_range("index out of range");
        return mData[(mFront + mSize - 1) % mCap];
    }
    // MODIFIER //
    void push(const T &e) {
        ensureCapacity(mSize + 1);
        mData[(mFront + mSize) % mCap] = e;
        mSize++;
    }
    void pop() {
        if (size() == 0)
            throw std::out_of_range("index out of range");
        mFront = (mFront + 1) % mCap;
        mSize--;
    }

    friend std::ostream& operator<< (std::ostream &os, const queue &q){
        os << "queue[";
        for(size_t i = 0; i < q.mCap; i++) {
            os << q.mData[i] << ", ";
        }
        os << "front = " << q.mFront << "]";
        return os;
    }
};



#endif // QUEUE_H
