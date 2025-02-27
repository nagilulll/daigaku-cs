#ifndef hashmap_h
#define hashmap_h

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <functional>

template<typename ValueType>
class HashMap {
public:
    explicit HashMap(double maxLoadFactor = 0.75) : maxLoadFactor(maxLoadFactor), buckets(10), numItems(0) {}

    ~HashMap() = default;

    void insert(const std::string& key, const ValueType& value) {
        auto& bucket = buckets[getBucketIndex(key)];
        for (auto& pair : bucket) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        bucket.push_back({key, value});
        ++numItems;
        if (loadFactor() > maxLoadFactor) {
            resize();
        }
    }

    ValueType& operator[](const std::string& key) {
        auto& bucket = buckets[getBucketIndex(key)];
        for (auto& pair : bucket) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        bucket.emplace_back(key, ValueType());
        ++numItems;
        if (loadFactor() > maxLoadFactor) {
            resize();
        }
        return bucket.back().second;
    }

    const ValueType* find(const std::string& key) const {
        const auto& bucket = buckets[getBucketIndex(key)];
        for (const auto& pair : bucket) {
            if (pair.first == key) {
                return &pair.second;
            }
        }
        return nullptr;
    }

    int size() const {
        return numItems;
    }

private:
    std::vector<std::list<std::pair<std::string, ValueType>>> buckets;
    int numItems;
    double maxLoadFactor;

    size_t getBucketIndex(const std::string& key) const {
        std::hash<std::string> hasher;
        return hasher(key) % buckets.size();
    }

    double loadFactor() const {
        return static_cast<double>(numItems) / buckets.size();
    }

    void resize() {
        std::vector<std::list<std::pair<std::string, ValueType>>> newBuckets(buckets.size() * 2);
        for (auto& oldBucket : buckets) {
            for (auto& pair : oldBucket) {
                auto& newBucket = newBuckets[std::hash<std::string>()(pair.first) % newBuckets.size()];
                newBucket.push_back(std::move(pair));
            }
        }
        buckets.swap(newBuckets);
    }
};

#endif /* hashmap_h */
