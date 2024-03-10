/*
 *  \author
 *      Tiago Sousa Fonseca - 107266
 *      Fábio Alves - 108016
 *      Guilherme Duarte - 107766
 */

#include "somm23.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void memInit(uint32_t mSize, uint32_t osSize, uint32_t cSize, AllocationPolicy policy) 
    {
        const char *pas = policy == FirstFit ? "FirstFit" : policy == BuddySystem ? "BuddySystem" : "Unkown";
        soProbe(501, "%s(%#x, %#x, %#x, %s)\n", __func__, mSize, osSize, cSize, pas);

        require(mSize > osSize, "memory must be bigger than the one use by OS");
        require((mSize % cSize) == 0, "memory size must be a multiple of chunck size");
        require((osSize % cSize) == 0, "memory size for OS must be a multiple of chunck size");
        require(policy == FirstFit or policy == BuddySystem, "policy must be FirstFit or BuddySystem");

        /*
         *  The following must be considered:
         *      In case of an error, an appropriate exception must be thrown.
         *      All exceptions must be of the type defined in this project (Exception).        
         */

        // Set the global memory management parameters
        memParameters.totalSize = mSize;
        memParameters.kernelSize = osSize;
        memParameters.chunkSize = cSize;
        memParameters.policy = policy;

        // Initialize the memory management data structures, depending on the policy
        if (policy == FirstFit)
        {
            // Initialize the list of free blocks
            memFreeHead = new MemListNode;
            memFreeHead->block.pid = 0;
            memFreeHead->block.address = osSize;
            memFreeHead->block.size = mSize - osSize;
            memFreeHead->next = NULL;
            memOccupiedHead = NULL;
        }
        else{
            memTreeRoot = new MemTreeNode;
            memTreeRoot->left=NULL;
            memTreeRoot->right=NULL;
            memTreeRoot->state=FREE;
            memTreeRoot->block.address=osSize;
            memTreeRoot->block.pid=0;
            uint32_t blocksize=(mSize-osSize)/cSize;
            uint32_t pow=1;
            while((pow*2)<blocksize){
                pow*=2;
            }
            memTreeRoot->block.size=pow*cSize;
            
        }
    }

// ================================================================================== //

} // end of namespace group

