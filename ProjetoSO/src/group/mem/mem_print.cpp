/*
 *  \author 
 *      Guilherme Ferreira Duarte - 107766
 *     Fábio Micael Correia Alves - 108016
 */

#include "somm23.h"

#include <stdio.h>
#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void printBuddySystem(MemTreeNode* node, FILE *fout, bool isOccupied) {
    if (node == NULL) {
        return;
    }

    if ((node->state == OCCUPIED && isOccupied)) {
        fprintf(fout,"|%8u |%#12x |%11u |\n", node->block.pid, node->block.address, node->block.size);
    }

    if ((node->state == FREE && !isOccupied)) {
        fprintf(fout,"|   ---   |%#12x |%11u |\n", node->block.address, node->block.size);
    }

    printBuddySystem(node->left, fout, isOccupied);
    printBuddySystem(node->right, fout, isOccupied);
}

    void memPrint(FILE *fout)
    {
        soProbe(503, "%s(\"%p\")\n", __func__, fout);

        require(fout != NULL and fileno(fout) != -1, "fout must be a valid file stream");

        if(memParameters.policy==FirstFit){
            fprintf(fout,"+====================================+\n");
            fprintf(fout,"|   FirstFit memory occupied blocks  |\n");
            fprintf(fout,"+---------+-------------+------------+\n");
            fprintf(fout,"|   PID   |   address   |    size    |\n");
            fprintf(fout,"+---------+-------------+------------+\n");
            //percorrer a lista de blocos ocupados
            MemListNode *occupiedNode = memOccupiedHead;
            
            while(occupiedNode != NULL){
                fprintf(fout,"|%8u |%#12x |%11u |\n", occupiedNode->block.pid, occupiedNode->block.address, occupiedNode->block.size);
                occupiedNode = occupiedNode->next;
            }
            fprintf(fout,"+====================================+\n");
            fprintf(fout,"\n");


            fprintf(fout,"+====================================+\n");
            fprintf(fout,"|     FirstFit memory free blocks    |\n");
            fprintf(fout,"+---------+-------------+------------+\n");
            fprintf(fout,"|   PID   |   address   |    size    |\n");
            fprintf(fout,"+---------+-------------+------------+\n");
            //percorrer a lista de blocos livres
            MemListNode *freeNode = memFreeHead;
            while(freeNode != NULL){
                fprintf(fout,"|   ---   |%#12x |%11u |\n",freeNode->block.address, freeNode->block.size);
                freeNode = freeNode->next;
            }
            fprintf(fout,"+====================================+\n");
            fprintf(fout,"\n");
        }
        else{
        fprintf(fout,"+====================================+\n");
        fprintf(fout,"| BuddySystem memory occupied blocks |\n");
        fprintf(fout,"+---------+-------------+------------+\n");
        fprintf(fout,"|   PID   |   address   |    size    |\n");
        fprintf(fout,"+---------+-------------+------------+\n");

        // Traverse the Buddy System tree and print occupied blocks
        printBuddySystem(memTreeRoot, fout, true);

        fprintf(fout,"+====================================+\n");
        fprintf(fout,"\n");

        fprintf(fout,"+====================================+\n");
        fprintf(fout,"|   BuddySystem memory free blocks   |\n");
        fprintf(fout,"+---------+-------------+------------+\n");
        fprintf(fout,"|   PID   |   address   |    size    |\n");
        fprintf(fout,"+---------+-------------+------------+\n");

        // Traverse the Buddy System tree and print free blocks
        printBuddySystem(memTreeRoot, fout, false);

        fprintf(fout,"+====================================+\n");
        fprintf(fout,"\n");
    }
    }

// ================================================================================== //

} // end of namespace group

