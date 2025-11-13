#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

// Check if all leaf-root paths have the same depth 
bool checkDepth(Node* currNode, int currDepth, int& leafDepth) {
    // Base Case
    if (currNode == nullptr) { 
        return true; 
    }

    // Check if current node is a leaf 
    if (currNode->left == nullptr && currNode->right == nullptr) { 
        // If leaf depth is 0, this is the first leaf we have seen 
        if (leafDepth == 0) { 
            leafDepth = currDepth; 
        } 
        // Check if paths are different depths      
        else if (currDepth != leafDepth) { 
            return false; 
        }
        return true; 
    }

    // Recurse through left and right children 
    bool leftTree = checkDepth(currNode->left, currDepth + 1, leafDepth); 
    bool rightTree = checkDepth(currNode->right, currDepth + 1, leafDepth);

    return (leftTree && rightTree); 
}

bool equalPaths(Node * root)
{
    int leafDepth = 0; 
    return checkDepth(root, 0, leafDepth); 
    
    
}

