/*
 *  \author ...
 */

#include "somm23.h"

#include <stdio.h>
#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void swpPrint(FILE *fout)
    {
        soProbe(403, "%s(\"%p\")\n", __func__, fout);

        require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

        SwpNode *current = swpHead;
        fprintf(fout,"+===============================================+\n");
        fprintf(fout,"|             Swapped Process Queue             |\n");
        fprintf(fout,"+-------+---------------------------------------+\n");
        fprintf(fout,"|  PID  |         address space profile         |\n");
        fprintf(fout,"+-------+---------------------------------------+\n");
        while(current != NULL){
            fprintf(fout, "|   %3d |", current->process.pid);
            for(uint32_t i = 0; i < current->process.profile.segmentCount; i++){
                if(i == MAX_BLOCKS - 1){
                    fprintf(fout,"%8u |\n", current->process.profile.size[i]);
                }else{
                    fprintf(fout,"%8u :", current->process.profile.size[i]);
                }
            }

            for(uint32_t i = current->process.profile.segmentCount; i < MAX_BLOCKS; i++){
                if(i == MAX_BLOCKS - 1){
                    fprintf(fout, "   ---   |\n");
                } else {
                    fprintf(fout, "   ---   :");
                }
            }

            current = current->next;
        }
        fprintf(fout,"+===============================================+\n\n");
        /* TODO POINT: Replace next instruction with your code */
        //throw Exception(ENOSYS, __func__);
    }


// ================================================================================== //

} // end of namespace group

