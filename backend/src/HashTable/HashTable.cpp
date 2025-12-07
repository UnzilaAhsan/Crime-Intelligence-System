#include "HashTable.h"
#include <iostream>

using namespace std;

template<typename K, typename V>
HashTable<K, V>::HashTable(int cap) : capacity(cap), size(0) {
    table.resize(capacity);
}

template<typename K, typename V>
void HashTable<K, V>::insert(K key, V value) {
    int index = hashFunction(key);
    
    // Check if key already exists
    for (auto& node : table[index]) {
        if (node.key == key) {
            node.value = value;
            return;
        }
    }
    
    // Insert new node
    table[index].emplace_back(key, value);
    size++;
}

template<typename K, typename V>
bool HashTable<K, V>::remove(K key) {
    int index = hashFunction(key);
    
    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->key == key) {
            table[index].erase(it);
            size--;
            return true;
        }
    }
    return false;
}

template<typename K, typename V>
V* HashTable<K, V>::get(K key) {
    int index = hashFunction(key);
    
    for (auto& node : table[index]) {
        if (node.key == key) {
            return &node.value;
        }
    }
    return nullptr;
}

template<typename K, typename V>
bool HashTable<K, V>::contains(K key) {
    return (get(key) != nullptr);
}

template<typename K, typename V>
void HashTable<K, V>::display() {
    for (int i = 0; i < capacity; i++) {
        if (!table[i].empty()) {
            cout << "Bucket " << i << ": ";
            for (auto& node : table[i]) {
                cout << "[" << node.key << " -> " << node.value << "] ";
            }
            cout << endl;
        }
    }
}

// Explicit instantiations
template class HashTable<std::string, std::string>;
template class HashTable<std::string, int>;
