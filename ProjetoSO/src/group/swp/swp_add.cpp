/*
 *  \author ...
    André Miragaia 108412
    Fábio Alves 108016
 */

#include "somm23.h"

namespace group
{

// ================================================================================== //

    void swpAdd(uint32_t pid, AddressSpaceProfile *profile)
    {
        soProbe(404, "%s(%u, %p)\n", __func__, pid, profile);

        require(pid > 0, "a valid process ID must be greater than zero");
        require(profile != NULL, "profile must be a valid pointer to a AddressSpaceProfile");
        
        /* TODO POINT: Replace next instruction with your code */
        //throw Exception(ENOSYS, __func__);
        SwpNode *newNode = new SwpNode;
        // como dar handle do profile?
        newNode->process.pid = pid;
        newNode->process.profile = *profile;
        newNode->next = NULL;
        
        if(swpHead == NULL){
            swpHead = newNode;
            swpTail = newNode;
        }
        else{
            swpTail->next = newNode;
            swpTail = newNode;
        }
    }

// ================================================================================== //

} // end of namespace group

