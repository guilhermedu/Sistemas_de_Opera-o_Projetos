/*
 *  \author
 *      Tiago Sousa Fonseca - 107266
 */

#include "somm23.h"

#include <stdint.h>
#include <../include/feq.h>

namespace group 
{

// ================================================================================== //

    void feqInit()
    {
        soProbe(201, "%s()\n", __func__);


        /*
         *  The module's internal data structure, defined in file feq.cpp, should be initialized properly.
         *  The following must be considered:
         *      It can be assumed that the input file is syntactically and semantically correct
         */


        // Initialize the FEQ head
        feqHead = NULL;
    }

// ================================================================================== //

} // end of namespace group

