/*
 *  \author Guilherme Chapelo Andrade, João Paulo Rodrigues
 */

#include "somm23.h"

#include <stdio.h>
#include <stdint.h>
#include <algorithm>
namespace group 
{

// ================================================================================== //

   void simPrint(FILE *fout)
{
    soProbe(103, "%s(\"%p\")\n", __func__, fout);

    require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

    fprintf(fout, "+====================================================================+\n");
    fprintf(fout, "|                          forthcomingTable                          |\n");
    fprintf(fout, "+-------+---------+----------+---------------------------------------+\n");
    fprintf(fout, "|    Simulation step:    %3u |              Simulation time: %7u |\n", stepCount, simTime);
    fprintf(fout, "+-------+---------+----------+---------------------------------------+\n");
    fprintf(fout, "|  PID  | arrival | lifetime |         address space profile         |\n");
    fprintf(fout, "+-------+---------+----------+---------------------------------------+\n");

    /* std::sort(forthcomingTable.process, forthcomingTable.process + forthcomingTable.count, 
        [](const ForthcomingProcess &a, const ForthcomingProcess &b) {
            return a.arrivalTime < b.arrivalTime;
        });
 */
    for (uint32_t i = 0; i < forthcomingTable.count; i++) {
        ForthcomingProcess process = forthcomingTable.process[i];

        fprintf(fout, "| %5u | %7u | %8u |", process.pid, process.arrivalTime, process.lifetime);

         for(uint32_t i = 0; i < MAX_BLOCKS; i++){
            if(i >= process.addressSpace.segmentCount){
                if(i == MAX_BLOCKS-1){
                    fprintf(fout,"   ---   |\n");
                } else {
                    fprintf(fout,"   ---   :");
                }
            } else {
                if(i == MAX_BLOCKS-1){
                    fprintf(fout,"%8u |\n", process.addressSpace.size[i]);
                } else {
                    fprintf(fout,"%8u :", process.addressSpace.size[i]);
                }
            }
        }
    }
    fprintf(fout, "+====================================================================+\n");
    fprintf(fout, "\n");
}
// ================================================================================== //

} // end of namespace group
