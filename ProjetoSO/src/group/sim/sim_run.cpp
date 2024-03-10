/*
 *  André e Guilherme Chapelo Andrade
 */

#include "somm23.h"

namespace group
{

// ================================================================================== //

    void simRun(uint32_t cnt)
    {
        soProbe(108, "%s(%u)\n", __func__, cnt);

        // se cnt == 0, corre até ao fim
        if (cnt == 0) {
            while (simStep()) {
                // continua a correr até não poder mais
            }
        } else {
            // corre cnt vezes
            for (uint32_t i = 0; i < cnt && simStep(); ++i) {
                // cada iteração é um passo
            }
        }
    }

// ================================================================================== //

} // end of namespace group

