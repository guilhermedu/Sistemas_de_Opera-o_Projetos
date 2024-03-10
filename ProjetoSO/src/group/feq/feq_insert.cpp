/*
 *  \author
 *      Tiago Sousa Fonseca - 107266
 */

#include "somm23.h"
#include <../include/feq.h>
#include <../include/tme.h>

namespace group 
{

// ================================================================================== //

    void feqInsert(FutureEventType type, uint32_t time, uint32_t pid) 
    {
        const char *tas = type == ARRIVAL ? "ARRIVAL" : type == TERMINATE ? "TERMINATE" : "UNKNOWN";
        soProbe(204, "%s(%s, %u, %u)\n", __func__, tas, time, pid);

        require(pid > 0, "process ID must be non-zero");
        

        /* 
         *  Insert logic:
         *      - the list's elements should be sorted in ascending order of event time;
         *      - for elements with the same time stamp, TERMINATE events should appear before ARRIVAL events;
         *      - for elements with the same time stamp and the same type, order of insertion should be preserved.
         *      - If an anomalous situation occurs, an appropriate exception must be thrown.
         *      - All exceptions must be of the type defined in this project (Exception). 
         */


        // Create a new node for the Future Event Queue (FEQ)
        FeqEventNode* feq_node = new FeqEventNode;
        feq_node->event.pid = pid;
        feq_node->event.time = time;
        feq_node->event.type = type;

        /* 
         *  If 
         *      FEQ is empty or 
         *      the feqHead event time is greater then the feq_node event time
         *  We insert at the head
         */
        if (feqHead == NULL || feqHead->event.time > time)
        {
            feq_node->next = feqHead;
            feqHead = feq_node;
        } 
        else 
        {
            // Traverse the FEQ to find the appropriate position for insertion
            FeqEventNode* feq_current_node = feqHead;
            FeqEventNode* feq_previous_node = NULL;

            /* 
             *  While 
             *      feq_current_node is not NULL and
             *          feq_current_node event time < time or
             *              feq_current_node event type is ARRIVAL and feq_node event type is ARRIVAL or
             *              feq_current_node event type is TERMINATE and feq_node event type is ARRIVAL
             *  We keep traversing the FEQ
             */ 
            while (feq_current_node != NULL && 
                    (feq_current_node->event.time < time ||
                        (feq_current_node->event.time == time && 
                            ((feq_current_node->event.type == type) || 
                                (feq_current_node->event.type == TERMINATE && type == ARRIVAL)))))
            {
                feq_previous_node = feq_current_node;
                feq_current_node = feq_current_node->next;
            }

            /*
             *  Insert the new node at the appropriate position in the FEQ
             *  Check if the feq_previous_node is not NULL
             */ 
            if (feq_previous_node != NULL)
            {
                /*
                 *  It it isn't NULL, it means that we are a intermediate or end node
                 *  Connect the last node to the feq_node
                 */ 
                feq_previous_node->next = feq_node;
            } 
            else
            {
                // If it is, it means that we are the feqHead (probably we have the lower timestamp)
                feqHead = feq_node;
            }

            // Link the feq_node next pointer to the feq_current_node 
            feq_node->next = feq_current_node;
        }
    }


// ================================================================================== //

} // end of namespace group

