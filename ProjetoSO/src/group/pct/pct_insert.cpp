/*
 *  \author Guilherme Chapelo Andrade, João Paulo Rodrigues
 */

#include "somm23.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void pctInsert(uint32_t pid, uint32_t time, uint32_t lifetime, AddressSpaceProfile *profile)
    {
        soProbe(304, "%s(%d, %u, %u, %p)\n", __func__, pid, time, lifetime, profile);

        require(pid > 0, "a valid process ID must be greater than zero");
        require(time >= 0, "time must be >= 0");
        require(lifetime > 0, "lifetime must be > 0");
        require(profile != NULL, "profile must be a valid pointer to a AddressSpaceProfile");

        /* TODO POINT: Replace next instruction with your code */
        // Check for duplicate pid
        PctNode *current = pctHead;
        while (current != NULL) {
            if (current->pcb.pid == pid) {
                throw Exception(EINVAL, "An entry with the given pid already exists");
            }
            current = current->next;
        }

        // Create a new node
        PctNode *newNode = new PctNode;
        require(newNode != NULL, "unable to allocate memory for a new node");

        newNode->pcb.pid = pid;
        newNode->pcb.state = NEW;
        newNode->pcb.arrivalTime = time;
        newNode->pcb.lifetime = lifetime;
        newNode->pcb.activationTime = NO_TIME;
        newNode->pcb.finishTime = NO_TIME;
        newNode->pcb.memProfile = *profile; 
        newNode->pcb.memMapping.blockCount = 0;
        for (int i = 0; i < MAX_BLOCKS; i++) {
            newNode->pcb.memMapping.address[i] = 0;
        }

        // Insert the node into the linked list in ascending order of pid
        if (pctHead == NULL || pctHead->pcb.pid > pid) {
            // If list is empty or new node should be first
            newNode->next = pctHead;
            pctHead = newNode;
        } else {
            // Find the correct position in the list
            current = pctHead;
            while (current->next != NULL && current->next->pcb.pid < pid) {
                current = current->next;
            }
            // Insert the new node
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

// ================================================================================== //

// end of namespace group

