/*
 *  Fábio Micael Correia Alves - 108016
 *  Guilherme Chapelo Andrade  - 107696
 *  Guilherme Ferreira Duarte - 107766
 */

#include "somm23.h"

#include <stdint.h>
#include <math.h>

namespace group 
{
    
// ================================================================================== //
// Função para encontrar o menor número inteiro maior ou igual a x que é uma potência de 2
MemTreeNode* allocateRecursive(uint32_t pid, uint32_t size, MemTreeNode* node) {
    // If the node is free and its size is equal to the requested size, allocate it
    if (node->state == FREE && node->block.size == size) {
        node->block.pid = pid;
        node->state = OCCUPIED;
        return node;
    }

    // If the node is free and its size is larger than the requested size, split it
    if (node->state == FREE && node->block.size > size) {
        uint32_t splited_block = node->block.size / 2;

        // Create left and right buddies
        MemTreeNode* leftBuddy = new MemTreeNode;
        MemTreeNode* rightBuddy = new MemTreeNode;

        // Initialize left buddy
        leftBuddy->block.size = splited_block;
        leftBuddy->block.address = node->block.address;
        leftBuddy->block.pid = 0; // initially free
        leftBuddy->state = FREE;
        leftBuddy->left = NULL;
        leftBuddy->right = NULL;

        // Initialize right buddy
        rightBuddy->block.size = splited_block;
        rightBuddy->block.address = node->block.address + splited_block;
        rightBuddy->block.pid = 0; // initially free
        rightBuddy->state = FREE;
        rightBuddy->left = NULL;
        rightBuddy->right = NULL;

        // Update current node
        node->left = leftBuddy;
        node->right = rightBuddy;
        node->state = SPLITTED;

        // Recursively allocate on one of the buddies
        return allocateRecursive(pid, size, leftBuddy);
    }

    // If the node is split, try to allocate on its children
    if (node->state == SPLITTED) {
        MemTreeNode* leftAddress = allocateRecursive(pid, size, node->left);
        if (leftAddress != NULL) {
            return leftAddress;
        }
        return allocateRecursive(pid, size, node->right);
    }
    return NULL_ADDRESS;
    
}

Address memBuddySystemAlloc(uint32_t pid, uint32_t size)
{
    soProbe(506, "%s(%u, %#x)\n", __func__, pid, size);

    require(pid > 0, "a valid process ID must be greater than zero");
    require(size > 0, "the size of a memory segment must be greater than zero");

    printf("ARGUMENTS: %u, %u\n",pid,size);
    // Função recursiva para alocar memória
    MemTreeNode* node = memTreeRoot;
    MemTreeNode* allocatedNode = allocateRecursive(pid, size, node);


    // Se a alocação falhar, retorna NULL_ADDRESS
    if (allocatedNode == NULL_ADDRESS) {
        return NULL_ADDRESS;
    }
    printf("alocated: %x\n",allocatedNode->block.address);

    return allocatedNode->block.address;

}

// ================================================================================== //
} // end of namespace group```