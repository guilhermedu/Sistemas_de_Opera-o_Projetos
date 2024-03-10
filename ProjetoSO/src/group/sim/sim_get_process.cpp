/*
 *  André e Guilherme Chapelo Andrade
 */

#include "somm23.h"

namespace group
{

// ================================================================================== //

     ForthcomingProcess *simGetProcess(uint32_t pid)
    {
        soProbe(106, "%s(%u)\n", __func__, pid);

        require(pid > 0, "a valid process ID must be greater than zero");

        for (uint32_t i = 0; i < forthcomingTable.count; ++i) {
            if (forthcomingTable.process[i].pid == pid) {
                // processo encontrado
                return &forthcomingTable.process[i];
            }
        }

        // processo não encontrado
        throw Exception(EINVAL, "Process ID does not exist");
    }

// ================================================================================== //

} // end of namespace group

