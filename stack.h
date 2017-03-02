/** class  : Stack
  * author : Kris Kalavantavanich
  * date   : 24 Feb 2017
*/
#include<stdexcept>

#ifndef STACK_H
#define STACK_H

template<typename T>
class Stack {
    public:
        Stack() {
            int cap = 1;
            mData = new T[cap]();
            mCap = cap;
            mSize = 0;
        }
        Stack(const Stack<T> &a) {
            mData = new T[a.mCap]();
            mCap = a.mCap;
            mSize = a.mSize;
            for (size_t i = 0; i < a.mSize; i++){
                mData[i] = a.mData[i];
            }
        }
        ~Stack() {
            delete[] mData;
        }

        // Capacity functions
        bool empty() const {
            return mSize == 0;
        }
        size_t size() const {
            return mSize;
        }
        const T& top() {
            if (mSize == 0) {
                throw std::out_of_range("index out of range");
            }
            return mData[mSize-1];
        }
        // Modifier
        void push(const T& e) {
            ensureCapacity(mSize + 1);
            mData[mSize] = e;
            mSize++;
        }
        void pop() {
            if (mSize == 0) {
                throw std::out_of_range("index out of range");
            }
            mSize--;
        }

        friend std::ostream& operator<< (std::ostream& os, const Stack &s) {
            os << "[";
            if (mSize == 0) {
                os << "]";
                return os;
            }
            for (int i = 0; i < mSize - 1; i++) {
                os << mData[i] << ", ";
            }
            os << mData[mSize-1] << "]";
        }


    protected:
        T *mData;
        size_t mCap;
        size_t mSize;

        void expand(size_t capacity) {
            T *arr = new T[capacity]();
            for (size_t i = 0; i < mSize; i++) {
                arr[i] = mData[i];
            }
            delete[] mData;
            mData = arr;
            mCap = capacity;
        }

        void ensureCapacity(size_t capacity) {
            if (capacity > mCap) {
                size_t s = (capacity > 2 * mCap) ? capacity : 2 * mCap;
                expand(s);
            }
        }

};
#endif // STACK_H
