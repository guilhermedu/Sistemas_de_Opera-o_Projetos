/*
 *  \author Guilherme Chapelo Andrade
 */

#include "somm23.h"

namespace group 
{

// ================================================================================== //

    void pctTerm() 
    {
        soProbe(302, "%s()\n", __func__);

        /* TODO POINT: Replace next instruction with your code */
        // throw Exception(ENOSYS, __func__);
        while(pctHead != NULL){
            PctNode* temp = pctHead;
            pctHead = pctHead->next;
            delete temp;
        }
    }

// ================================================================================== //

} // end of namespace group

