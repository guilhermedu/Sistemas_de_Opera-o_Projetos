/*
 *  \author ...
    André Miragaia 108412
    Fábio Alves 108016
 */

#include "somm23.h"

namespace group
{

// ================================================================================== //

    void swpTerm()
    {
        soProbe(402, "%s()\n", __func__);

        /* TODO POINT: Replace next instruction with your code */
        SwpNode *current = swpHead;
        while(current != NULL){
            SwpNode *temp = current;
            current = current->next;
            delete temp;
        }

        swpHead = NULL;
        swpTail = NULL; 
        
    }

// ================================================================================== //

} // end of namespace group

