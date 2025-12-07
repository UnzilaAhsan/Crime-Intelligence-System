#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <string>

template <typename T>
class BTreeNode {
public:
    bool leaf;
    std::vector<T> keys;
    std::vector<BTreeNode*> children;
    
    BTreeNode(bool leaf = true);
};

template <typename T>
class BTree {
private:
    BTreeNode<T>* root;
    int t;  // Minimum degree
    
    void insertNonFull(BTreeNode<T>* node, T key);
    void splitChild(BTreeNode<T>* parent, int i, BTreeNode<T>* child);
    BTreeNode<T>* search(BTreeNode<T>* node, T key);
    void traverse(BTreeNode<T>* node);
    
public:
    BTree(int degree);
    void insert(T key);
    bool search(T key);
    void display();
    
    // For criminal data
    void loadFromDatabase();
    void saveToDatabase();
};

#endif
