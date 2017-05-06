/** Unordered Map implemented via hashtable with separated chaining vector.*/
#ifndef UNORDERED_MAP_SCV_H
#define UNORDERED_MAP_SCV_H

#include<utility>
#include<vector>

template <typename KeyT,
         typename MappedT,
         typename HasherT = std::hash<KeyT>,
         typename EqualT = std::equal_to<KeyT> >
class unordered_map
{
protected:
    typedef std::pair<KeyT, MappedT> ValueT;
    typedef std::vector<ValueT> BucketT;
    typedef typename std::vector<ValueT>::iterator ValueIterator;
    typedef typename std::vector<BucketT>::iterator BucketIterator;

    const size_t N_PRIMES = 40;
    const size_t MAX_PRIME = 173;
    const unsigned long PRIMES[40] = {
        2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul,
        31ul, 37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul, 71ul,
        73ul, 79ul, 83ul, 89ul, 97ul, 101ul, 103ul, 107ul, 109ul, 113ul,
        127ul, 131ul, 137ul, 139ul, 149ul, 151ul, 157ul, 163ul, 167ul, 173ul,
    };

    std::vector<BucketT> mBuckets;
    size_t mSize;
    HasherT mHasher;
    EqualT mEqual;
    float mMaxLoadFactor;

    class hashtable_iterator {
    protected:
        ValueIterator mCurrentValueIter;
        BucketIterator mCurrentBucketIter;
        BucketIterator mEndBucketIter;

        void validate_iterators() {
            while (mCurrentBucketIter != mEndBucketIter &&
                   mCurrentValueIter == mCurrentBucketIter->end()) {
                mCurrentBucketIter++;
                if (mCurrentBucketIter == mEndBucketIter) break;
                mCurrentValueIter = mCurrentBucketIter->begin();
            }
        }
    public:
        typedef ValueT &reference;
        typedef ValueT *pointer;

        hashtable_iterator() { };

        hashtable_iterator(ValueIterator valueIter,
                           BucketIterator bucketIter,
                           BucketIterator endBucketIter) :
            mCurrentValueIter(valueIter),
            mCurrentBucketIter(bucketIter),
            mEndBucketIter(endBucketIter)
        {
            validate_iterators();
        }

        hashtable_iterator &operator++() { // ++it
            mCurrentValueIter++;
            validate_iterators();
            return (*this);
        }

        hashtable_iterator operator++(int) { // it++
            hashtable_iterator tmp(*this);
            operator++();
            return tmp;
        }

        reference operator*() {
            return *mCurrentValueIter;
        }

        pointer operator->() {
            return &(*mCurrentValueIter);
        }

        bool operator== (const hashtable_iterator &other) {
            return mCurrentValueIter == other.mCurrentValueIter;
        }

        bool operator!= (const hashtable_iterator &other) {
            return mCurrentValueIter != other.mCurrentValueIter;
        }
    }; // hashtable_iterator class

public:
    typedef hashtable_iterator iterator;

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

    // Capacity Functions //
    bool empty() {return mSize = 0; }
    size_t size() {return mSize; }
    size_t bucket_count() {return mBuckets.size(); }
    size_t max_bucket_count() {return MAX_PRIME; }
    size_t bucket_size(size_t n) {return mBuckets[n].size(); }
    float load_factor() { return (float) mSize / mBuckets.size(); }
    float max_load_factor() { return mMaxLoadFactor; }

    // Hasher //
    size_t hash_to_bucket(const KeyT& key) {
        return mHasher(key) % mBuckets.size();
    }

    // Access & Modifiers //
    ValueIterator find_in_bucket(BucketT &b, const KeyT &key) {
        for(ValueIterator it = b.begin(); it != b.end(); it++) {
            if (mEqual(it->first, key)) return it;
        }
        return b.end();
    }

    ValueIterator find(const KeyT &key) {
        size_t bucket_index = hash_to_bucket(key);
        return find_in_bucket(mBuckets[bucket_index], key);
    }

    MappedT &operator[] (const KeyT &key) {
        size_t bucket_index = hash_to_bucket(key);
        ValueIterator it = find_in_bucket(mBuckets[bucket_index], key);

        if (it == mBuckets[bucket_index].end()) {
            it = insert_to_bucket(make_pair(key, MappedT()), bucket_index);
        }
        return it->second;
    }

    ValueIterator insert_to_bucket(const ValueT& val, size_t &bucket_index) {
        if (load_factor() > max_load_factor()){
            rehash(2 * bucket_count());
            bucket_index = hash_to_bucket(val.first);
        }
        ++mSize;
        return mBuckets[bucket_index].insert(mBuckets[bucket_index].end(), val);
    }

    std::pair<iterator, bool> insert(const ValueT &val) {
        std::pair<iterator, bool> result;
        const KeyT& key = val.first;
        size_t bucket_index = hash_to_bucket(key);
        ValueIterator it = find_in_bucket(mBuckets[bucket_index], key);
        result.second = false;
        if (it == mBuckets[bucket_index].end()) {
            it = insert_to_bucket(val, bucket_index);
            // bucket_index may change
            result.second = true;
        }
        result.first = iterator(it, mBuckets.begin() + bucket_index, mBuckets.end());
        return result;
    }

    size_t erase(const KeyT &key) {
        size_t bucket_index = hash_to_bucket(key);
        ValueIterator it = find_in_bucket(mBuckets[bucket_index], key);
        if (it == mBuckets[bucket_index].end()) {
            return 0;
        } else {
            mBuckets[bucket_index].erase(it);
            mSize--;
            return 1;
        }
    }

    void clear() {
        for (BucketIterator it = mBuckets.begin(); it != mBuckets.end(); it++) {
            (*it).clear();
        }
        mSize = 0;
    }

    void rehash(size_t m) {
        if (m <= mBuckets.size() && load_factor() <= max_load_factor()) return;
        m = std::max(m, (size_t) (0.5 + mSize/mMaxLoadFactor));
        m = *std::lower_bound(PRIMES, PRIMES+N_PRIMES, m);
        std::vector<ValueT> tmp;
        for (BucketIterator it = mBuckets.begin(); it != mBuckets.end(); it++) {
            for (ValueIterator it2 = (*it).begin(); it2 != (*it).end(); it2++) {
                tmp.push_back(*it2);
            }
        }
        this->clear();
        mBuckets.resize(m);
        for (ValueIterator it = tmp.begin(); it != tmp.end(); it++) {
            this->insert(*it);
        }
    }

    iterator begin() {
        return iterator(mBuckets.begin()->begin(), mBuckets.begin(), mBuckets.end());
    }

    iterator end() {
        return iterator(mBuckets[mBuckets.size()-1].end(), mBuckets.end(), mBuckets.end());
    }

    const iterator cbegin() {
        return iterator(mBuckets.begin()->begin(), mBuckets.begin(), mBuckets.end());
    }

    const iterator cend() {
        return iterator(mBuckets[mBuckets.size()-1].end(), mBuckets.end(), mBuckets.end());
    }
};

#endif // UNORDERED_MAP_SCV_H
