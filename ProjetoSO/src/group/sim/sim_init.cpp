/*
 *  André e Guilherme Chapelo Andrade
 */

#include "somm23.h"
#include <cstring>
namespace group
{

// ================================================================================== //

    /*
     * \brief Init the module's internal data structure
     */
    void simInit(uint32_t mSize, uint32_t osSize, uint32_t cSize, AllocationPolicy policy)
{
    const char *pas = policy == FirstFit ? "FirstFit" : policy == BuddySystem ? "BuddySystem" : "Unkown";
    soProbe(101, "%s(%#x, %#x, %#x, %s)\n", __func__, mSize, osSize, cSize, pas);

    // Reset global variables
    stepCount = 0;
    simTime = 0;

    // Initialize the table
    forthcomingTable.count = 0;
    memset(forthcomingTable.process, 0, sizeof(ForthcomingProcess) * MAX_PROCESSES);

    try {
        memInit(mSize, osSize, cSize, policy);
        swpInit();
        pctInit();
        feqInit();
    } catch (const Exception& e) {
        throw Exception(errno, __func__);
    }
}

// ================================================================================== //

} // end of namespace group

