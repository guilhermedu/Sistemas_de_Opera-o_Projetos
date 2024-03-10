/*
 *  \author ...
    André Miragaia 108412
    Fábio Alves 108016
 */

#include "somm23.h"

namespace group
{

// ================================================================================== //

    void swpRemove(uint32_t idx)
    {
        soProbe(406, "%s(%u)\n", __func__, idx);

        /* TODO POINT: Replace next instruction with your code */
        // throw Exception(ENOSYS, __func__);
        uint32_t count = 0;
        SwpNode* current = swpHead;
        SwpNode* previous = NULL;

        if (current == NULL) {
            throw Exception(EINVAL, __func__);
        }

        while (current != NULL) {
            if (count == idx) {
                if (previous == NULL) {
                    swpHead = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;

                current = swpHead;
                while (current != NULL) {
                    current = current->next;
                }

                break;
            } else {
                count++;
                previous = current;
                current = current->next;
            }
        }
    }

// ================================================================================== //

} // end of namespace group

