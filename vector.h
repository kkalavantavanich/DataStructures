/** class  : Vector
  * author : Kris Kalavantavanich
  * date   : 24 Feb 2017
*/
#include<stdexcept>

#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector
{
    public:
        Vector(){
            int cap = 1;
            mData = new T[cap]();
            mCap = cap;
            mSize = 0;
        }
        Vector(size_t cap) {
            mData = new T[cap]();
            mCap = cap;
            mSize = cap;
        }
        Vector(const Vector<T>& a) {
            mData = new T[a.capacity()]();
            mCap  = a.capacity();
            mSize = a.size();
            for (size_t i = 0; i < a.size(); i++) {
                mData[i] = a[i];
            }
        }
        ~Vector(){
            clear();
            delete [] mData;
        }
    // capacity functions
        bool empty() const {return mSize == 0;}
        size_t size() const {return mSize;}
        size_t capacity() const {return mCap;}
        void resize(size_t n) {
            if (n > mCap) {
                T *arr = new T[n]();
                for (size_t i = 0; i < mSize; i++) {
                    arr[i] = mData[i];
                }
                delete[] mData;
                mData = arr;
                mCap = n;
            }
            T init = T();
            for (size_t i = mSize; i < n; i++) {
                mData[i] = init;
            }
        }
    // iterator
        typedef T* iterator;

        iterator begin() const {
            return mData;
        }
        iterator end() const {
            return mData + mSize;
        }
    // access
        T& at(int index) const {
            return (*this)[index];
        }
        T& operator[](int i) const{
            if (i < 0 || (size_t) i >= mSize) {
                throw std::out_of_range("index out of range");
            }
            return mData[i];
        }
    // modifiers
        void push_back(const T& e){
            insert(end(), e);
        }
        void pop_back() {
            mSize--;
        }
        void clear(){
            mSize = 0;
        }
        void erase(iterator it){
            while ((it+1) != end() ) {
                *it  = *(it+1);
                it++;
            }
            mSize--;
        }
        void insert(iterator it, const T &e){
            size_t pos = it - begin();
            ensureCapacity(mSize + 1);
            for (size_t i = mSize; i > pos; i--) {
                mData[i] = mData[i-1];
            }
            mData[pos] = e;
            mSize++;
        }
        friend std::ostream& operator<< (std::ostream &os, const Vector &v) {
            os << "[";
            iterator it = v.begin();
            if(it != v.end()){
                for (; it != v.end() - 1; it++) {
                    os << *it << ", ";
                }
                os << *it << "]";
            } else {
                os << "]";
            }
            return os;
        }

        Vector<T> operator+ (const Vector &other) const {
            Vector<T> out (*this);
            for (iterator it = other.begin(); it != other.end(); it++) {
                out.push_back(*it);
            }
            return out;
        }

        int indexOf (const T &e) const {
            int i = 0;
            for (iterator it = begin(); it !=  end(); it++, i++) {
                if (*it == e) return i;
            }
            return -1;
        }

        bool contains (const T &e) const {
            for (iterator it = begin(); it != end(); it++) {
                if (*it == e) return true;
            }
            return false;
        }

    protected:
        T *mData;
        size_t mCap;
        size_t mSize;

        void ensureCapacity(size_t capacity) {
            if (capacity > mCap) {
                size_t s = (capacity > 2*mCap) ? capacity : 2 * mCap;
                resize(s);
            }
        }
};
#endif // VECTOR_H
