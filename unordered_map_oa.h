/** Unordered Map implemented via hashtables with open addressing. */
#ifndef UNORDERED_MAP_OA_H
#define UNORDERED_MAP_OA_H

template<typename KeyT,
         typename MappedT,
         typename HasherT = std::hash<KeyT>,
         typename EqualT = std::equal_to<KeyT> >
class unordered_map{
protected:
    typedef std::pair<KeyT, MappedT> ValueT;
    typedef typename std::vector<ValueT>::iterator ValueIterator;

    size_t mSize;
    HasherT mHasher;
    EqualT mEqual;
    float mMaxLoadFactor;
    size_t mUsed;       //Number of non-empty buckets

    class BucketT {
    private:
        unsigned char status; // 0 = empty, 1 = deleted, 2 = data;
    public:
        ValueT  value;

        BucketT() : status(0), value(ValueT()) {}
        bool available() {return status < 2;}
        bool empty() {return status == 0;}
        bool has_data() {return status == 2;}
        void mark_empty() {status = 0;}
        void mark_deleted() {status = 1;}
        void mark_data() {status = 2;}
    };
    class hashtable_iterator{
    protected:
        BucketIterator mCurrentBucketIter;
        BucketIterator mEndBucketIter;

        void validate_iterators() {
            while(mCurrentBucketIter != mEndBucketIter && !mCurrentBucketIter->has_data()){
                mCurrentBucketIter++;
            }
        }
    public:

        /** Constructor */
        hashtable_iterator(BucketIterator bucket, BucketIterator endBucket) :
            mCurrentBucketIter(bucket), mEndBucketIter(endBucket)
        {
            validate_iterators();
        }

        hashtable_iterator &operator++() { // ++it
            mCurrentBucketIter++;
            validate_iterators();
            return (*this);
        }

        hashtable_iterator operator++(int) { // it++
            hashtable_iterator tmp(*this);
            operator++();
            return tmp;
        }

        ValueT &operator*() {
            return mCurrentBucketIter->value;
        }

        ValueT *operator->() {
            return &(mCurrentBucketIter->value);
        }

        bool operator!= (const hashtable_iterator &other) {
            return (mCurrentBucketIter != other.mCurrentBucketIter);
        }

        bool operator== (const hashtable_iterator &other) {
            return (mCurrentBucketIter == other.mCurrentBucketIter);
        }
    };

    typedef typename std::vector<BucketT>::iterator BucketIterator;
    vector<BucketT> mBuckets;

public:
    /** Default Constructor */
    unordered_map() :
        mBuckets( std::vector<BucketT>(11) ),
        mSize(0),
        mHasher( HasherT() ),
        mEqual( EqualT() ),
        mMaxLoadFactor(1.0)
    { }

    /** Copy Constructor */
    unordered_map(const unordered_map<KeyT, MappedT, HasherT, EqualT> &other) :
        mBuckets(other.mBuckets),
        mSize(other.mSize),
        mHasher(other.mHasher),
        mEqual(other.mEqual),
        mMaxLoadFactor(other.mMaxLoadFactor)
    { }

    /** Move Assignment Operator */
    unordered_map<KeyT, MappedT, HasherT, EqualT> &operator=(unordered_map<KeyT, MappedT, HasherT, EqualT> other) {
        using std::swap;
        swap(this->mBuckets, other.mBuckets);
        swap(this->mSize, other.mSize);
        swap(this->mHasher, other.mHasher);
        swap(this->mEqual, other.mEqual);
        swap(this->mMaxLoadFactor, other.mMaxLoadFactor);
        return *this;
    }

    ~unordered_map() {
        clear();
    }
    // Hasher //
    size_t hash_to_bucket(const KeyT &key) {
        return mHasher(key) % mBuckets.size();
    }

    /** If unable to find key, will return an empty bucket.*/
    size_t find_position(const KeyT& key) {
        size_t initialPosition = hash_to_bucket(key);
        size_t pos = initialPosition;
        while(!mBuckets[pos].empty() && !mEqual(mBuckets[pos].value.first, key)){
            pos = (pos + 1) % mBuckets.size();
        }
        return pos;
    }

    BucketIterator check_and_insert_to_position(const ValueT &val, size_t &pos){
        if (load_factor() > max_load_factor()) {
            rehash(2 * bucket_count());
            pos = find_position(val.first);
        }
        mSize++;
        mBuckets[pos].value = val;
        if(mBuckets[pos].empty()) mUsed++;
        mBuckets[pos].mark_data();
        return mBuckets.begin() + pos;
    }

    std::pair<iterator, bool> insert(const ValueT& val) {
        std::pair<iterator, bool> result;
        size_t pos = find_position(val.first);
        if (mBuckets[pos].available()) {
            BucketIterator it = check_and_insert_to_position(val, pos);
            result.first = iterator(it, mBuckets.end());
            result.second = true;
        } else { // key exists -> return existing position
            result.first = iterator(mBuckets.begin() + pos, mBuckets.end());
            result.second = false;
        }
        return result;
    }

    size_t erase(const KeyT &key) {
        size_t pos = find_position(key);
        if (mBuckets[pos].has_data()) {
            mBuckets[pos].mark_deleted();
            mSize--;
            return 1;
        } else {
            return 0;
        }
    }

    void clear() {
        for (BucketIterator it = mBuckets.begin(); it != mBuckets.end(); it++) {
            it->mark_empty();
        }
        mSize = 0;
        mUsed = 0;
    }

    float load_factor() {
        return (float) mUsed/mBuckets.size();
    }

    float get_max_load_factor() {
        return mMaxLoadFactor;
    }

    void set_max_load_factor(float z) {
        mMaxLoadFactor = z;
        rehash(bucket_count());
    }

    MappedT &operator[] (const KeyT& key) {
        size_t pos = find_position(key);
        if (mBuckets[pos].available()) {
            check_and_insert_to_position(make_pair(key, MappedT()), pos);
        }
        return mBuckets[pos].value.second;
    }

    void rehash(size_t m) {
        if (load_factor() <= max_load_factor() && m <= mBuckets.size()) return;
        m = std::max(m, (size_t)(0.5+mSize/mMaxLoadFactor));
        m = *std::lower_bound(PRIMES, PRIMES+N_PRIMES, m);
        std::vector<ValueT> tmp;
        for (BucketIterator it = mBuckets.begin(); it != mBuckets.end(); it++) {
            for (ValueIterator it2 = it->begin(); it2 != it->end(); it2++) {
                tmp.push_back(*it2);
            }
        }
        this->clear();
        mBuckets.resize(m);
        for (ValueIterator it = tmp.begin(); it != tmp.end(); it++) {
            this->insert(val);
        }
    }

    typedef hashtable_iterator iterator;
    iterator begin() {
        return iterator(mBuckets.begin(), mBuckets.end());
    }
};
#endif // UNORDERED_MAP_OA_H
