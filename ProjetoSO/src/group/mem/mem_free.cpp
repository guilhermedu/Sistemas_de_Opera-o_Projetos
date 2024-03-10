/*
 *  \author ...
 *      Guilherme Ferreira Duarte - 107766
 */

#include "somm23.h"

#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void memFree(AddressSpaceMapping *mapping)
    {
        soProbe(507, "%s(mapping: %p)\n", __func__, mapping);

        require(mapping != NULL, "mapping must be a valid pointer to a AddressSpaceMapping");

        for(uint32_t i = 0; i < mapping->blockCount; i++){
            //chamar a função de libertar memória para cada bloco
            if(memParameters.policy == FirstFit){
                memFirstFitFree(mapping->address[i]);
            }else if(memParameters.policy == BuddySystem){
                memBuddySystemFree(mapping->address[i]);
            }else{
                //lançar uma exceção
                throw Exception(ENOSYS, __func__);
            }
        }
    }

// ================================================================================== //

} // end of namespace group


