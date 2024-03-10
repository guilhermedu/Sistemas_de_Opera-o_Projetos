/*
 *  \author Guilherme Chapelo Andrade, João Paulo Rodrigues
 */

#include "somm23.h"

#include <stdint.h>
#include <string.h>

namespace group 
{

// ================================================================================== //

    void pctUpdateState(uint32_t pid, ProcessState state, uint32_t time = NO_TIME, AddressSpaceMapping *mapping = NULL)
    {
        soProbe(309, "%s(%d, %u, %u)\n", __func__, pid, state, time);

        require(pid > 0, "a valid process ID must be greater than zero");

        /* TODO POINT: Replace next instruction with your code */
        PctNode* current = pctHead;
        while (current != NULL) {
            if (current->pcb.pid == pid) {
                current->pcb.state = state;
                if (state == ACTIVE) {
                    current->pcb.activationTime = time;
                    current->pcb.memMapping = *mapping;
                }
                if (state == FINISHED) {
                    current->pcb.finishTime = time;
                }
                if (state == SWAPPED || state == DISCARDED) {
                    current->pcb.memMapping.blockCount = 0;
                    for (int i = 0; i < MAX_BLOCKS; i++) {
                        current->pcb.memMapping.address[i] = 0;
                    }
                }
                return;
            }
            current = current->next;
        }
        throw Exception(EINVAL, __func__);
    }

// ================================================================================== //

} // end of namespace group

