#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include <string>
#include <functional>

template<typename K, typename V>
class HashTable {
private:
    struct HashNode {
        K key;
        V value;
        HashNode(K k, V v) : key(k), value(v) {}
    };
    
    std::vector<std::list<HashNode>> table;
    int capacity;
    int size;
    
    int hashFunction(K key) {
        std::hash<K> hashFunc;
        return hashFunc(key) % capacity;
    }
    
public:
    HashTable(int cap = 100);
    
    void insert(K key, V value);
    bool remove(K key);
    V* get(K key);
    bool contains(K key);
    void display();
    int getSize() { return size; }
    
    // Criminal specific methods
    void loadFingerprints();
    void loadWeapons();
};

#endif
