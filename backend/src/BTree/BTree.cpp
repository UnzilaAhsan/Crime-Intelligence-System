#include "BTree.h"
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
BTreeNode<T>::BTreeNode(bool isLeaf) : leaf(isLeaf) {}

template <typename T>
BTree<T>::BTree(int degree) : root(nullptr), t(degree) {}

template <typename T>
void BTree<T>::insert(T key) {
    if (root == nullptr) {
        root = new BTreeNode<T>(true);
        root->keys.push_back(key);
    } else {
        if (root->keys.size() == 2*t - 1) {
            BTreeNode<T>* newRoot = new BTreeNode<T>(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0, root);
            
            int i = 0;
            if (newRoot->keys[0] < key) i++;
            insertNonFull(newRoot->children[i], key);
            
            root = newRoot;
        } else {
            insertNonFull(root, key);
        }
    }
}

template <typename T>
void BTree<T>::insertNonFull(BTreeNode<T>* node, T key) {
    int i = node->keys.size() - 1;
    
    if (node->leaf) {
        node->keys.push_back(T()); // Add empty element
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i+1] = node->keys[i];
            i--;
        }
        node->keys[i+1] = key;
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        
        if (node->children[i]->keys.size() == 2*t - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

template <typename T>
void BTree<T>::splitChild(BTreeNode<T>* parent, int i, BTreeNode<T>* child) {
    BTreeNode<T>* newChild = new BTreeNode<T>(child->leaf);
    
    // Move t-1 keys from child to newChild
    for (int j = 0; j < t-1; j++) {
        newChild->keys.push_back(child->keys[j+t]);
    }
    
    if (!child->leaf) {
        for (int j = 0; j < t; j++) {
            newChild->children.push_back(child->children[j+t]);
        }
    }
    
    // Resize child
    child->keys.resize(t-1);
    if (!child->leaf) {
        child->children.resize(t);
    }
    
    // Insert middle key to parent
    parent->keys.insert(parent->keys.begin() + i, child->keys[t-1]);
    parent->children.insert(parent->children.begin() + i + 1, newChild);
}

template <typename T>
bool BTree<T>::search(T key) {
    return (search(root, key) != nullptr);
}

template <typename T>
BTreeNode<T>* BTree<T>::search(BTreeNode<T>* node, T key) {
    if (node == nullptr) return nullptr;
    
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i])
        i++;
    
    if (i < node->keys.size() && key == node->keys[i])
        return node;
    
    if (node->leaf)
        return nullptr;
    
    return search(node->children[i], key);
}

// Explicit instantiation for string type
template class BTree<std::string>;
