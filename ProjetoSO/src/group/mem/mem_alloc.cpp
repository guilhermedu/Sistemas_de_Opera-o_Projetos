/*
 *  \author 
 *    Tiago Sousa FOnseca - 107266
 *    Fábio Micael Correia Alves - 108016
 *    Guilherme Chapelo Andrade  - 107696
 *    Guilherme Ferreira Duarte - 107766
 */

#include "somm23.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

uint32_t powerOfTwo(uint32_t num) {
    if (num == 0)
        return 1;

    uint32_t power = 1;
    while (power < num) {
        power *= 2;
    }
    return power;
}

 AddressSpaceMapping *memAlloc(uint32_t pid, AddressSpaceProfile *profile)
{
    soProbe(504, "%s(%u, %p)\n", __func__, pid, profile);

    require(pid > 0, "process ID must be non-zero");
    require(profile != NULL, "profile must be a valid pointer to an AddressSpaceProfile variable");

    // Calculate the total memory required for the address space profile
    uint32_t totalMemoryRequired = 0;
    uint32_t rounded2[profile->segmentCount];

    if (memParameters.policy == FirstFit){
        for (uint32_t i = 0; i < profile->segmentCount; ++i)
        {
            // Round up the segment size to the smallest multiple of the chunk size
            uint32_t roundedSize = ((profile->size[i] + memParameters.chunkSize - 1) / memParameters.chunkSize) * memParameters.chunkSize;
            totalMemoryRequired += roundedSize;
            rounded2[i]=roundedSize;
        }
    }else{
        for (uint32_t i = 0; i < profile->segmentCount; ++i)
        {
            // Round up the segment size to the smallest multiple of the chunk size
            uint32_t roundedSize = powerOfTwo((profile->size[i] + memParameters.chunkSize - 1) / memParameters.chunkSize) * memParameters.chunkSize;
            totalMemoryRequired += roundedSize;
            rounded2[i]=roundedSize;
        }
    }
    // Check if there is enough available memory
    if (totalMemoryRequired > (memParameters.totalSize - memParameters.kernelSize))
    {
        return IMPOSSIBLE_MAPPING;
    }
    else
    {
        /* The mapping to be filled and whose pointer should be returned */
        static AddressSpaceMapping theMapping = {0, {0}};

        // Flag to track if an allocation fails
        bool allocationFailed = false;
        theMapping.blockCount=0;

        for (uint32_t i = 0; i < profile->segmentCount; ++i)
        {
            Address segmentAddress;

            /* TODO POINT: Modify the following according to your allocation strategy */
            if (memParameters.policy == FirstFit)
            {
                segmentAddress = memFirstFitAlloc(pid, rounded2[i]);
            }
            else if (memParameters.policy == BuddySystem)
            {
                segmentAddress = memBuddySystemAlloc(pid, rounded2[i]);
            }
            else
            {
                // Handle an error for unsupported policy
                allocationFailed = true;
                break; // Exit the loop
            }

            if (segmentAddress == NULL_ADDRESS)
            {
                // Handle a failed allocation for this segment
                allocationFailed = true;
                break; // Exit the loop
            }

            /* Update the AddressSpaceMapping for this segment */
            theMapping.address[i] = segmentAddress;
            theMapping.blockCount++;
        }
        // Check if any allocation failed
        if (allocationFailed)
        {
            // Deallocate all previously allocated segments
            memFree(&theMapping);
            return NULL_ADDRESS; // Or throw an exception
        }

        return &theMapping;
    }
    }
}
 // end of namespace group