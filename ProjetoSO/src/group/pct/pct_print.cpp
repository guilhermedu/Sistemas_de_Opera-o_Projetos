/*
 *  \author João Paulo Rodrigues
 */

#include "somm23.h"

#include <stdio.h>
#include <stdint.h>
#include <string>

namespace group 
{

// ================================================================================== //

    void pctPrint(FILE *fout)
    {
        soProbe(303, "%s(\"%p\")\n", __func__, fout);

        require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

        /* TODO POINT: Replace next instruction with your code */

        PctNode* current = pctHead;

        fprintf(fout,"+====================================================================================================================================================+\n");
        fprintf(fout,"|                                                                  Process Control Table                                                             |\n");
        fprintf(fout,"+-------+-----------+---------+----------+---------+---------+---------------------------------------+-----------------------------------------------+\n");
        fprintf(fout,"|  PID  |   state   | arrival | lifetime | active  | finish  |         address space profile         |             address space mapping             |\n");
        fprintf(fout,"+-------+-----------+---------+----------+---------+---------+---------------------------------------+-----------------------------------------------+\n");

        
        while (current != NULL) {
            

            fprintf(fout,"| %5d | %-9s | %7u | %8u | %7s | %7s |",
            current->pcb.pid,
            pctGetStateAsString(current->pcb.pid),
            current->pcb.arrivalTime,
            current->pcb.lifetime,
            current->pcb.activationTime == NO_TIME ? "  ---  " : std::to_string(current->pcb.activationTime).c_str(),
            current->pcb.finishTime == NO_TIME ? "  ---  " : std::to_string(current->pcb.finishTime).c_str());
            // current->pcb.memProfile
            for(uint32_t i = 0; i < current->pcb.memProfile.segmentCount;i++){
                if(current->pcb.memProfile.size[i] == 0){
                    if(i == MAX_BLOCKS-1){
                        fprintf(fout,"   ---   |");
                    } else {fprintf(fout,"   ---   :");}
                } else {
                    if(i== MAX_BLOCKS-1){
                        fprintf(fout,"%8u |", current->pcb.memProfile.size[i]);
                    } else {fprintf(fout,"%8u :", current->pcb.memProfile.size[i]);}
                }
            }
            for(uint32_t i = current->pcb.memProfile.segmentCount; i < MAX_BLOCKS; i++){
                if(i == MAX_BLOCKS - 1){
                    fprintf(fout, "   ---   |");
                } else {
                    fprintf(fout, "   ---   :");
                }
            }
            // current->pcb.memMapping
            for(uint32_t i = 0; i < current->pcb.memMapping.blockCount;i++){
                if(!current->pcb.memMapping.address[i]){
                    if(i == MAX_BLOCKS-1){
                        fprintf(fout,"    ---    |\n");
                    } else {fprintf(fout,"    ---    :");}
                } else {
                    if(i== MAX_BLOCKS-1){
                        fprintf(fout," 0x%07x |\n", current->pcb.memMapping.address[i]);
                    } else {fprintf(fout," 0x%07x :", current->pcb.memMapping.address[i]);}
                }
            }
            for(uint32_t i = current->pcb.memMapping.blockCount; i < MAX_BLOCKS; i++){
                if(i == MAX_BLOCKS - 1){
                    fprintf(fout, "    ---    |\n");
                } else {
                    fprintf(fout, "    ---    :");
                }
            }

            current = current->next;
        }
        fprintf(fout,"+====================================================================================================================================================+\n");
        fprintf(fout,"\n");
    }

// ================================================================================== //

} // end of namespace group

