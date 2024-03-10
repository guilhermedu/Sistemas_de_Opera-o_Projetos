/*
 *  \author João Paulo Rodrigues
 */

#include "somm23.h"
#include <time.h>

namespace group
{

// ================================================================================== //

    bool simStep()
    {
        soProbe(107, "%s()\n", __func__);

        /* TODO POINT: Replace next instruction with your code */

        if (feqIsEmpty()){
            return false;
        }
         // Fetch the next event from the FEQ
        FutureEvent event = feqPop();
        stepCount++;

        switch (event.type) {
            case ARRIVAL: {
                // Process an ARRIVAL event
                ForthcomingProcess *forthcomingProcess = simGetProcess(event.pid);
                if (forthcomingProcess == NULL) {
                    throw Exception(EINVAL, "Process not found");
                }

                // Insert the process into the PCT
                pctInsert(forthcomingProcess->pid, forthcomingProcess->arrivalTime, forthcomingProcess->lifetime, &forthcomingProcess->addressSpace);

                // Allocate memory for the process
                AddressSpaceMapping* mapping = memAlloc(forthcomingProcess->pid, &forthcomingProcess->addressSpace);

                if (mapping == IMPOSSIBLE_MAPPING){
                    pctUpdateState(forthcomingProcess->pid, DISCARDED);
                    simTime = forthcomingProcess->arrivalTime;
                } else if (mapping == NULL_ADDRESS){
                    swpAdd(forthcomingProcess->pid,&forthcomingProcess->addressSpace);
                    pctUpdateState(forthcomingProcess->pid, SWAPPED);
                    simTime = forthcomingProcess->arrivalTime;
                } else {
                    feqInsert(TERMINATE,forthcomingProcess->lifetime + forthcomingProcess->arrivalTime,forthcomingProcess->pid);
                    // código binário não passa mapping como argumento e deveria passar
                    pctUpdateState(forthcomingProcess->pid,ACTIVE,forthcomingProcess->arrivalTime, mapping);
                    simTime = forthcomingProcess->arrivalTime;
                }
                break;
            }
            case TERMINATE: {
                // Process a TERMINATE event
                AddressSpaceMapping* mapping = pctGetAddressSpaceMapping(event.pid);
                memFree(mapping);
                pctUpdateState(event.pid,FINISHED,event.time);
                simTime = event.time;

                uint32_t idx = 0;
                while(SwappedProcess* swapped = swpPeek(idx)){
                    AddressSpaceProfile* profile = pctGetAddressSpaceProfile(swapped->pid);
                    AddressSpaceMapping* mapping = memAlloc(swapped->pid, profile);
                    if (mapping == NULL_ADDRESS){
                        idx++;
                        continue;
                    } else {
                        uint32_t lifetime = pctGetLifetime(swapped->pid);
                        feqInsert(TERMINATE,lifetime+event.time,swapped->pid);
                        // código binário está a usar event.time onde deveria ser o arrival_time do
                        // swapped process no pctUpdateState
                        pctUpdateState(swapped->pid,ACTIVE,event.time,mapping);
                        swpRemove(idx);
                    }
                }
                break;
            }
        }
        return true;

    }

// ================================================================================== //

} // end of namespace group

