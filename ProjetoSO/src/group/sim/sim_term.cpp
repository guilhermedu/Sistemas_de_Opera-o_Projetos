/*
 *  André e Guilherme Chapelo Andrade
 */

#include "somm23.h"
#include <cstring>
namespace group 
{

// ================================================================================== //

   void simTerm() 
{
    soProbe(102, "%s()\n", __func__);

    try {
        memTerm(); // Terminate the memory module
        swpTerm(); // Terminate the swap module
        pctTerm(); // Terminate the process control block module
        feqTerm(); // Terminate the free entry queue module
    } catch (...) {
        // Rethrow the exception as an Exception with the appropriate error number and function name
        throw Exception(errno, __func__);
    }

    // Reset the simulation's global variables
    stepCount = 0;
    simTime = 0;

    // Reset the forthcoming table
    forthcomingTable.count = 0;
    memset(forthcomingTable.process, 0, sizeof(ForthcomingProcess) * MAX_PROCESSES);
}

// ================================================================================== //

} // end of namespace group

