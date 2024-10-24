#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <list>
#include <chrono>
#include <memory>
#include <string>

class CacheEntry {
public:
    std::string key;
    std::string value;
    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    
    CacheEntry(const std::string& k, const std::string& v)
        : key(k), value(v), timestamp(std::chrono::steady_clock::now()) {}
};

class Cache {
private:
    size_t capacity;
    std::chrono::seconds ttl;
    std::list<std::shared_ptr<CacheEntry>> cacheList;
    std::unordered_map<std::string, std::list<std::shared_ptr<CacheEntry>>::iterator> cacheMap;

    void evictExpiredEntries();
    void moveToFront(const std::string& key);

public:
    Cache(size_t cap, std::chrono::seconds ttl_duration);
    std::string get(const std::string& key);
    void put(const std::string& key, const std::string& value);
    void evict(const std::string& key);
};

#endif // CACHE_H
