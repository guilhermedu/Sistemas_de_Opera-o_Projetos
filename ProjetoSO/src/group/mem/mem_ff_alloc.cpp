/*
 *  \author 
 *      Guilherme Ferreira Duarte - 107766 
 *      Fábio Micael Correia Alves - 108016
 */

#include "somm23.h"

#include <stdint.h>

namespace group 
{


// ================================================================================== //

    Address memFirstFitAlloc(uint32_t pid, uint32_t size)
    {
        soProbe(505, "%s(%u, %#x)\n", __func__, pid, size);

        require(pid > 0, "a valid process ID must be greater than zero");
        require(size, "the size of a memory segment must be greater than zero");

        // Inicializar o endereço de retorno como NULL_ADDRESS
        Address allocatedAddress = NULL_ADDRESS;        
        // Percorrer a lista de blocos livres até encontrar o primeiro bloco grande o suficiente
        MemListNode *currentNode = memFreeHead;
        while (currentNode != NULL) {
            // Verificar se o bloco atual é grande o suficiente
            if (currentNode->block.size >= size) {
                // Alocação bem-sucedida
                allocatedAddress = currentNode->block.address;
                // Atualizar o tamanho do bloco livre se necessário
                if (currentNode->block.size > size) {
                    currentNode->block.size -= size;
                    currentNode->block.address += size;
                } else {
                    // Remover o bloco livre da lista
                    if (currentNode == memFreeHead) {
                        memFreeHead = currentNode->next;
                    } else {
                        MemListNode *previousNode = memFreeHead;
                        while (previousNode->next != currentNode) {
                            previousNode = previousNode->next;
                        }
                        previousNode->next = currentNode->next;
                    }
                    delete currentNode;
                }

                // Sair do loop
                break;
            }
            // Passar para o próximo bloco da lista
            currentNode = currentNode->next;
        }
        // Se allocatedAddress ainda for NULL_ADDRESS, a alocação falhou
        if (allocatedAddress == NULL_ADDRESS) {
            // Lançar uma exceção apropriada
            return NULL_ADDRESS;
        }

        // Adicionar o bloco alocado à lista de blocos ocupados
        MemListNode *occupiedBlock = new MemListNode();
        occupiedBlock->block.pid = pid;
        occupiedBlock->block.size = size;
        occupiedBlock->block.address = allocatedAddress;
        occupiedBlock->next = memOccupiedHead;
        memOccupiedHead = occupiedBlock;

        //ordenar a lista dos occupied blocks por ordem crescente dos seus endereços
        currentNode = memOccupiedHead;
        MemListNode *aux = NULL;

        while (currentNode != NULL) {
            MemListNode *nextNode = currentNode->next;

            while (nextNode != NULL && nextNode->block.address < currentNode->block.address) {
                if (aux != NULL) {
                    aux->next = nextNode;
                } else {
                    memOccupiedHead = nextNode;
                }

                currentNode->next = nextNode->next;
                nextNode->next = currentNode;

                aux = nextNode;
                nextNode = currentNode->next;
            }

            aux = currentNode;
            currentNode = currentNode->next;
        }


        //ordenar a lista dos free blocks por ordem crescente dos seus endereços
        currentNode = memFreeHead;
        aux = NULL;

        while (currentNode != NULL) {
            MemListNode *nextNode = currentNode->next;

            while (nextNode != NULL && nextNode->block.address < currentNode->block.address) {
                if (aux != NULL) {
                    aux->next = nextNode;
                } else {
                    memOccupiedHead = nextNode;
                }

                currentNode->next = nextNode->next;
                nextNode->next = currentNode;

                aux = nextNode;
                nextNode = currentNode->next;
            }

            aux = currentNode;
            currentNode = currentNode->next;
        }

        return allocatedAddress;
    }


// ================================================================================== //

} // end of namespace group
