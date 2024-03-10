/*
 *  \author
 *      Guilherme Ferreira Duarte - 107766
 */

#include "somm23.h"

namespace group 
{

// ================================================================================== //

    void feqTerm() 
    {
        soProbe(202, "%s()\n", __func__);

        while(feqHead != NULL) {
            FeqEventNode* temp = feqHead;
            feqHead = feqHead->next;
            delete temp;
        }
    }

// ================================================================================== //

} // end of namespace group

