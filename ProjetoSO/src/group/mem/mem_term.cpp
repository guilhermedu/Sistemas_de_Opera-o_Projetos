/*
 *  \author ...
 *   Guilherme Ferreira Duarte - 107766
 *  Fábio Micael Correia Alves - 108016
 *  Guilherme Chapelo Andrade  - 107696
 */
 
 

#include "somm23.h"

namespace group 
{

// ================================================================================== //
    void deleteTree(MemTreeNode* node) 
    {
        if (node == NULL) return;

        // first delete both subtrees
        deleteTree(node->left);
        deleteTree(node->right);

        // then delete the node
        delete node;
    }
    void memTerm() 
    {
        soProbe(502, "%s()\n", __func__);
        // De-initialize the memory management data structures, depending on the policy
        if (memParameters.policy == FirstFit)
        {
            // De-initialize the list of free blocks
            delete memFreeHead;
            memFreeHead = NULL;
            delete memOccupiedHead;
            memOccupiedHead = NULL;
        }
        else // memParameters.policy == BuddySystem
        {
            deleteTree(memTreeRoot);
            memTreeRoot = NULL;
        }

        // Reset the global memory management parameters
        memParameters.totalSize = 0;
        memParameters.kernelSize = 0;
        memParameters.chunkSize = 0;
        // memParameters.policy is left as it is
    }


// ================================================================================== //

} // end of namespace group