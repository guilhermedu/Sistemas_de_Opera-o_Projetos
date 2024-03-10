/*
 *  \author ...
 *      Guilherme Ferreira Duarte - 107766 
 *     Fábio Micael Correia Alves - 108016
 */
 

#include "somm23.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //
void memFirstFitFree(Address address) {
    soProbe(508, "%s(%u)\n", __func__, address);

    require(memOccupiedHead != NULL, "Occupied list should contain nodes");

    // Encontrar o bloco ocupado a ser liberado
    MemListNode* currentOccupied = memOccupiedHead;
    MemListNode* previousOccupied = NULL;

    while (currentOccupied != NULL && currentOccupied->block.address != address) {
        previousOccupied = currentOccupied;
        currentOccupied = currentOccupied->next;
    }

    // Se o bloco ocupado não foi encontrado, lançar uma exceção
    if (currentOccupied == NULL) {
        throw Exception(ENOENT, __func__);
    }

    // Remover o bloco ocupado da lista
    if (previousOccupied != NULL) {
        previousOccupied->next = currentOccupied->next;
    } else {
        memOccupiedHead = currentOccupied->next;
    }

    // Encontrar o bloco livre anterior
    MemListNode* previousFree = memFreeHead;
    MemListNode* previousFreePrevious = NULL;

    while (previousFree != NULL && previousFree->block.address < address) {
        previousFreePrevious = previousFree;
        previousFree = previousFree->next;
    }

    // Calcular o tamanho do bloco livre
    uint32_t freeBlockSize = currentOccupied->block.size;

    // Atualizar o bloco livre anterior ou criar um novo bloco livre
    if (previousFreePrevious != NULL && previousFreePrevious->block.address + previousFreePrevious->block.size == address) {
        // Fundir com bloco livre à esquerda
        previousFreePrevious->block.size += freeBlockSize;

        // Tentar fundir com bloco livre à direita
        if (previousFree != NULL && address + freeBlockSize == previousFree->block.address) {
            previousFreePrevious->block.size += previousFree->block.size;
            previousFreePrevious->next = previousFree->next;
            delete previousFree;
        }
    } else {
        // Criar um novo bloco livre
        MemListNode* newFreeBlock = new MemListNode();
        newFreeBlock->block.address = address;
        newFreeBlock->block.size = freeBlockSize;
        newFreeBlock->next = previousFree;

        // Tentar fundir com bloco livre à direita
        if (previousFree != NULL && address + freeBlockSize == previousFree->block.address) {
            newFreeBlock->block.size += previousFree->block.size;
            newFreeBlock->next = previousFree->next;
            delete previousFree;
        }

        if (previousFreePrevious != NULL) {
            previousFreePrevious->next = newFreeBlock;
        } else {
            memFreeHead = newFreeBlock;
        }
    }

    // Libera a memória do bloco ocupado
    if (currentOccupied != NULL) {
        delete currentOccupied;
    }
    
    }
}

