/*
 *  \author
 *      Tiago Sousa Fonseca - 107266
 */

#include "somm23.h"
#include <../include/feq.h>

namespace group 
{

// ================================================================================== //

    FutureEvent feqPop()
    {
        soProbe(205, "%s()\n", __func__);


        /*
         *  The following must be considered:
         *      The event must be removed from the queue.
         *      The EINVAL exception should be thrown, if no event exists.
         *      All exceptions must be of the type defined in this project (Exception).
         *  
         *  Returns:
         *     The first event in the FEQ.
         */


        // If FEQ is empty, throw an exception
        if (feqIsEmpty())
        {
            throw Exception(EINVAL, __func__);
        }

        // Get the first event in the FEQ
        FutureEvent feq_event = feqHead->event;

        // Remove the first event from the FEQ
        FeqEventNode* feq_node = feqHead;

        // Update the FEQ head
        feqHead = feqHead->next;

        // Delete the node
        delete feq_node;

        // Return the event
        return feq_event;
    }

// ================================================================================== //

    bool feqIsEmpty()
    {
        soProbe(206, "%s()\n", __func__);

        /*
         *  The following must be considered:
         *      All exceptions must be of the type defined in this project (Exception).
         *  
         *  Returns:
         *      true if queue is empty and false otherwise
         */

        return feqHead == NULL;
    }

// ================================================================================== //

} // end of namespace group

