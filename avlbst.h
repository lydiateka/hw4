#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
    
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* AVLInsert(const std::pair<const Key, Value>& new_item);
    AVLNode<Key, Value>* AVLRemove(const Key& key);AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node); 
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node); 
    void rebalance(AVLNode<Key, Value>* node); 

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* newNode = static_cast<AVLNode<Key, Value>*>(this->AVLInsert(new_item)); 
    rebalance(newNode); 
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* removedNode = static_cast<AVLNode<Key, Value>*>(this->AVLRemove(key)); 
    rebalance(removedNode); 
}


template<class Key, class Value>
void AVLTree<Key,Value>::rebalance(AVLNode<Key, Value>* node)
{
    while(node != nullptr) { 
        //Calculate balance 
        int leftHeight = this->calculateHeight(node->getLeft()); 
        int rightHeight = this->calculateHeight(node->getRight());
        node->setBalance(leftHeight - rightHeight); 
        
        //Left-Left 
        if (node->getBalance() > 1 && node->getLeft() != nullptr &&node->getLeft()->getBalance() >= 0) { 
            node = rotateRight(node); 
        }
        //Left-Right 
        else if (node->getBalance() > 1 && node->getLeft() != nullptr && node->getLeft()->getBalance() < 0) { 
           node->setLeft(rotateLeft(node->getLeft()));
           node = rotateRight(node); 
        }

        //Right-Right 
        if (node->getBalance() < -1 && node->getRight() != nullptr && node->getRight()->getBalance() <= 0) { 
           node = rotateLeft(node);  
        }
        
        //Right-Left
        else if (node->getBalance() < -1 && node->getRight() != nullptr && node->getRight()->getBalance() > 0) { 
            node->setRight(rotateRight(node->getRight()));
            node = rotateLeft(node); 
        }

        node = node->getParent();
    }   

}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key,Value>::rotateLeft(AVLNode<Key, Value>* n1)
{
    AVLNode<Key, Value>* n2 = n1->getRight(); 
    AVLNode<Key, Value>* subtree = n2->getLeft(); 

    n2->setLeft(n1); 
    n1->setRight(subtree); 

    AVLNode<Key, Value>* parent = n1->getParent();
    n2->setParent(parent); 
    n1->setParent(n2); 
    if(parent != nullptr) { 
        if (parent->getLeft() == n1) { 
            parent->setLeft(n2);
        }
        else { 
            parent->setRight(n2);
        }
    }
    else { 
        this->root_ = n2; 
    }
    
    
    if (subtree != nullptr) { 
        subtree->setParent(n1); 
    }

    return n2; 
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key,Value>::rotateRight(AVLNode<Key, Value>* n1)
{
    AVLNode<Key, Value>* n2 = n1->getLeft(); 
    AVLNode<Key, Value>* subtree = n2->getRight(); 

    n2->setRight(n1); 
    n1->setLeft(subtree); 

   AVLNode<Key, Value>* parent = n1->getParent();
    n2->setParent(parent); 
    n1->setParent(n2); 
    if(parent != nullptr) { 
        if (parent->getLeft() == n1) { 
            parent->setLeft(n2);
        }
        else { 
            parent->setRight(n2);
        }
    }
    else { 
        this->root_ = n2; 
    }
    
    if (subtree != nullptr) { 
        subtree->setParent(n1); 
    }

    return n2; 
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::AVLInsert(const std::pair<const Key, Value>& new_item){
    if (this->root_ == nullptr) { 
        AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); 
        this->root_ = newNode; 
        return newNode; 
    }

    AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key, Value>*>(this->root_); 
    while (currNode != nullptr) { 
        if (new_item.first < currNode->getKey()) { 
            if (currNode->getLeft() == nullptr) { 
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, currNode);
                currNode->setLeft(newNode); 
                return newNode; 
            }
            else { 
                currNode = currNode->getLeft(); 
            }
            
        }
        else if (new_item.first > currNode->getKey()) { 
            if (currNode->getRight() == nullptr) { 
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, currNode);
                currNode->setRight(newNode); 
                return newNode; 
            }
            else { 
                currNode = currNode->getRight(); 
            } 
        }
        else { 
            currNode->setValue(new_item.second); 
            return currNode; 
        }
    }
    return currNode; 
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::AVLRemove(const Key& key)
{
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->root_);  

    // Walk through tree to find key 
    while (target != nullptr) { 
        if (key == target->getKey()) { 
            break; 
        }
        else if (key < target->getKey()) { 
            target = target->getLeft(); 
        }
        else if (key > target->getKey()) { 
            target = target->getRight(); 
        }   
    }

    // Key not found 
    if (target == nullptr) { 
        return nullptr; 
    }

    AVLNode<Key, Value>* parent = target->getParent();
    AVLNode<Key, Value>* child = nullptr; 
    
    // Case 1: 0 children 
    if (target->getLeft() == nullptr && target->getRight() == nullptr) { 
        if (parent == nullptr) { 
            this->root_ = nullptr; 
        }
        else if (parent->getLeft() == target) { 
            parent->setLeft(nullptr); 
        }
        else if (parent->getRight() == target) { 
            parent->setRight(nullptr); 
        }
        delete target; 
        return parent; 
    }

    // Case 2: 2 children
    if (target->getLeft() != nullptr && target->getRight() != nullptr) { 
        AVLNode<Key, Value>* predec = target->getLeft(); 
        while (predec->getRight() != nullptr) { 
            predec = predec->getRight(); 
        }
        nodeSwap(target, predec); 
        parent = target->getParent(); 

        if (target->getLeft() != nullptr) { 
            child = target->getLeft(); 
        }
        else if (target->getRight() != nullptr) {
            child = target->getRight();
        }
        
    }

    // Case 3: 1 child  
    else {
        if (target->getLeft() != nullptr && target->getRight() == nullptr) { 
            child = target->getLeft(); 
        }
        else if (target->getRight() != nullptr && target->getLeft() == nullptr) { 
        child = target->getRight(); 
        }
    }
    
    if (parent == nullptr) { 
        this->root_ = child; 
        if (child != nullptr) {
            child->setParent(nullptr); 
        }
    }
    else if (parent->getLeft() == target) { 
        parent->setLeft(child); 
        if (child != nullptr) { 
            child->setParent(parent);
        }
    }
    else if (parent->getRight() == target) { 
        parent->setRight(child); 
        if(child != nullptr) { 
            child->setParent(parent);
        }
    }

    delete target; 
    return parent; 
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
