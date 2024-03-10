/*
 *  \author Guilherme Chapelo Andrade e João Paulo Rodrigues
 */

#include "somm23.h"
#include <cstdlib> // for rand() and srand()
#include <unistd.h> // for getpid()
#include <set> // for std::set

namespace group
{

// ================================================================================== //

    void simRandomFill(uint32_t n, uint32_t seed)
    {
        soProbe(105, "%s(%u, %u)\n", __func__, n, seed);

        require(n == 0 or n >= 2, "At least 2 processes are required");
        require(n <= MAX_PROCESSES, "More than MAX_PROCESSES processes not allowed");

        // If n is zero, randomly select the number of processes between 2 and MAX_PROCESSES
        if (n == 0) {
            n = 2 + rand() % (MAX_PROCESSES - 2 + 1);
        }

        // If seed is zero, set the seed of the random number generator to getpid()
        if (seed == 0) {
            seed = getpid();
        }

        srand(seed);

        // Fill the table with random processes
        std::set<uint32_t> usedPIDs;
        uint32_t arrivalTime = 0;
        for (uint32_t i = 0; i < n; i++) {
            ForthcomingProcess process;

            // Generate a unique PID
            do {
                process.pid = 1 + rand() % 65535;
            } while (usedPIDs.count(process.pid) > 0);
            usedPIDs.insert(process.pid);

            // Generate a random arrival time
            arrivalTime += rand() % 101;
            process.arrivalTime = arrivalTime;

            // Generate a random lifetime
            process.lifetime = 10 + rand() % 991;

            // Generate a random address space profile
            uint32_t numSegments = 1 + rand() % MAX_SEGMENTS;
            for (uint32_t j = 0; j < numSegments; j++) {
                uint32_t segmentSize = 0x100 + rand() % (0x800 - 0x100 + 1);
                process.addressSpace.size[j] = segmentSize; 
            }
            process.addressSpace.segmentCount = numSegments; 
            // Add the process to the table
            forthcomingTable.process[i] = process;

            feqInsert(ARRIVAL,arrivalTime, process.pid);
        }

        forthcomingTable.count = n;
    }

// ================================================================================== //

} // end of namespace group

