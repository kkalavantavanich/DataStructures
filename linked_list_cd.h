
template<typename T>
class linked_list{
private:

    template<typename U = T>
    class node{
    public:
        U data;
        node<U> *prev, *next;

        node<U>(const U& data = U()):data(data), prev(this), next(this)
        { }

        node<U>(const U& data, node<U>* prev, node<U>* next)
            :data(U(data)), prev(prev), next(next)
        { }
    }; // class node

    template<typename U = T>
    class list_iterator{
    private:
        node<U>* ptr_;
    public:
        /** Empty Constructor */
        list_iterator() : ptr_(NULL){ }

        /** Default Constructor */
        list_iterator(node<U> *ptr) : ptr_(ptr) { }

        /** Copy Constructor */
        list_iterator(const list_iterator<U> &other){
            ptr_ = other.ptr_;
        }

        // Override Operators //
        list_iterator<U> &operator++() {
            ptr_ = ptr_->next;
            return (*this);
        }

        list_iterator<U> &operator--() {
            ptr_ = ptr_->prev;
            return (*this);
        }

        list_iterator<U> operator++(int) {
            list_iterator tmp(*this);
            ptr_ = ptr_->next;
            return tmp;
        }

        list_iterator<U> operator--(int) {
            list_iterator tmp(*this);
            ptr_ = ptr_->prev;
            return tmp;
        }

        node<U>& operator*() const {
            return *ptr_;
        }

        node<U>* operator->() const {
            return ptr_;
        }

        bool &operator==(const list_iterator<U> &other) const {
            return ptr_ == other.ptr_;
        }

        bool &operator!=(const list_iterator<U> &other) const {
            return ptr_ != other.ptr_;
        }
    }; // class list_iterator

private:
    size_t mSize_;
    node<T>* mHeader_;
public:
    typedef list_iterator<T> iterator;
    typedef list_iterator<const T> const_iterator;

    /** Empty Constructor */
    linked_list(): mSize_(0), mHeader_(NULL) { }

    linked_list(const T& element): mSize_(1), mHeader_(new node<T>(element)) { }

    /** Copy Constructor */
    linked_list(const linked_list<T> &other) {

    }

    ~linked_list() {
        clear();
        delete mHeader_;
    }

    // CAPACITY FUNCTIONS //
    inline bool empty() const { return (mSize_ == 0); }
    inline size_t size() const { return mSize_; }

    // ACCESS FUNCTIONS ///
    inline const T front() const {return mHeader_->next;}
    inline const T back() const {return mHeader_->prev;}

    // ITERATORS //
    iterator begin() {return iterator(mHeader_->next);}
    iterator end() {return iterator(mHeader_);}
    const_iterator cbegin() const {
        node<const T>* tmp;
        tmp = (mHeader_->next);
        return const_iterator(tmp);
    }
    const_iterator cend() const {
        node<const T>* tmp;
        tmp = (mHeader_);
        return const_iterator(tmp);
    }

    // MODIFIERS //

    /// Insert element before position.
    iterator insert(iterator it, const T& element) {
        node<T> *n = new node<T>(element, it->prev, &*it);
        it->prev->next = n;
        it->prev = n;
        mSize_++;
        return it;
    }

    iterator erase(iterator it) {
        iterator tmp(it);
        it->prev->next = it->next;
        it->next->prev = it->prev;
        delete it.ptr_;
        mSize_--;
        return tmp;
    }

    void clear() {
        while(mSize_ > 0) {
            erase(begin());
        }
    }

    iterator push_front(const T& element) {
        insert(iterator(mHeader_->next), element);
    }

    iterator push_back(const T& element) {
        insert(iterator(mHeader_->prev), element);
    }

    iterator pop_front() {
        erase(iterator(mHeader_->next));
    }

    iterator pop_back() {
        erase(iterator(mHeader_->prev));
    }

    //OUTPUT
    friend std::ostream &operator<< (std::ostream &os, const linked_list &ll) {
        os << "linked_list[";
        for (const_iterator it = ll.cbegin(); it != ll.cend();) {
            os << it->data;
            if(++it != ll.cend()) os << " ";
        }
        os << "]";
        return os;
    }

};
