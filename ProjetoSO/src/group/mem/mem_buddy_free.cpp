/*
 *  \author
 *     Tiago Sousa Fonseca - 107266
 *     Fábio Micael Correia Alves - 108016
 *     Guilherme Chapelo Andrade  - 107696
 */

#include "somm23.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

// Helper function to recursively free nodes and merge buddies
void freeRecursive(MemTreeNode* node, Address address) 
{
    if (node == NULL) {
        return;
    }

    if (node->block.address == address && node->state == OCCUPIED) {
        // Mark the node as free
        node->state = FREE;
        node->block.pid = 0; // Reset PID
        return;
    }

    if (node->state == SPLITTED) {
        // Recursively traverse the tree to find the node to free
        freeRecursive(node->left, address);
        freeRecursive(node->right, address);
        if (node->left->state == FREE && node->right->state == FREE) {
            // Merge buddies
            node->state = FREE;
            node->block.pid = 0; // Reset PID
            node->block.size = node->left->block.size + node->right->block.size;
            node->left = NULL;
            node->right = NULL;
        }
    }

    

}


void memBuddySystemFree(Address address)
{
    soProbe(509, "%s(%u)\n", __func__, address);

    /*
        *  Free a previously allocated (buddy system) block of memory.
        *  
        *  If the block to be freed is contiguous to an empty block, merging must take place. 
        *  
        *  The following must be considered:
        *  
        *      In case of an error, an appropriate exception must be thrown.
        *      All exceptions must be of the type defined in this project (Exception).
        */

    // Check if the address is valid
    require(address >= memParameters.kernelSize, "address must be bigger than the one use by OS");
    require(address < memParameters.totalSize, "address must be smaller than the total memory size");
    require((address % memParameters.chunkSize) == 0, "address must be a multiple of the chunk size");

    // Start from the root to free the corresponding node
    MemTreeNode* node = memTreeRoot;
    freeRecursive(node, address);
}

// ================================================================================== //

} // end of namespace group

