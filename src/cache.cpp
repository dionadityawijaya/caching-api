#include "cache.h"

Cache::Cache(size_t cap, std::chrono::seconds ttl_duration)
    : capacity(cap), ttl(ttl_duration) {}

void Cache::evictExpiredEntries() {
    auto now = std::chrono::steady_clock::now();
    while (!cacheList.empty()) {
        auto entry = cacheList.back();
        auto age = std::chrono::duration_cast<std::chrono::seconds>(now - entry->timestamp);
        if (age >= ttl) {
            cacheMap.erase(entry->key);
            cacheList.pop_back();
        } else {
            break;
        }
    }
}

void Cache::moveToFront(const std::string& key) {
    auto entryIt = cacheMap[key];
    cacheList.splice(cacheList.begin(), cacheList, entryIt);
    (*entryIt)->timestamp = std::chrono::steady_clock::now();
}

std::string Cache::get(const std::string& key) {
    evictExpiredEntries();
    if (cacheMap.find(key) == cacheMap.end()) {
        return ""; // Cache miss
    }
    moveToFront(key);
    return (*cacheMap[key])->value;
}

void Cache::put(const std::string& key, const std::string& value) {
    evictExpiredEntries();
    if (cacheMap.find(key) != cacheMap.end()) {
        moveToFront(key);
        (*cacheMap[key])->value = value;
    } else {
        if (cacheList.size() >= capacity) {
            auto last = cacheList.back();
            cacheMap.erase(last->key);
            cacheList.pop_back();
        }
        auto newEntry = std::make_shared<CacheEntry>(key, value);
        cacheList.push_front(newEntry);
        cacheMap[key] = cacheList.begin();
    }
}

void Cache::evict(const std::string& key) {
    if (cacheMap.find(key) != cacheMap.end()) {
        cacheList.erase(cacheMap[key]);
        cacheMap.erase(key);
    }
}
