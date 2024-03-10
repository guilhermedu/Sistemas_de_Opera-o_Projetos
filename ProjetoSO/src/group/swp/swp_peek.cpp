/*
 *  André Miragaia 108412
    Fábio Alves 108016
 */

#include "somm23.h"

namespace group
{

// ================================================================================== //

    SwappedProcess *swpPeek(uint32_t idx)
    {
        soProbe(405, "%s(%u)\n", __func__, idx);

        /* TODO POINT: Replace next instruction with your code */
        //throw Exception(ENOSYS, __func__);

        SwpNode *current = swpHead;
        uint32_t index_atual = 0;
        while (index_atual != idx){
            current = current->next;
            index_atual++;
        }
        if (current == NULL){
            return NULL;
        }
        else{
            return &(current->process);
        }
 
    }

// ================================================================================== //

} // end of namespace group

