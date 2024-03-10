/*
 *  \author
    *      Guilherme Ferreira Duarte - 107766
    *      Tiago Fonseca - 107266
 */

#include "somm23.h"

#include <stdio.h>
#include <stdint.h>

namespace group 
{

// ================================================================================== //

    void feqPrint(FILE *fout)
    {
        soProbe(203, "%s(\"%p\")\n", __func__, fout);
        

        // Se o arquivo de saída não for válido, lança uma exceção ENOSYS
        if (!fout) {
            throw Exception(ENOSYS, __func__);
        }

        require(fout != NULL && fileno(fout) != -1, "fout must be a valid file stream");

        FeqEventNode* feq_node = feqHead; // Obtém o ponteiro para o início

        // Imprime o estado interno do módulo FEQ e imprimir informações sobre cada evento
        
        fprintf(fout, "+==============================+\n");
        fprintf(fout, "|      Future Event Queue      |\n");
        fprintf(fout, "+----------+-----------+-------+\n");
        fprintf(fout, "|   time   |   type    |  PID  |\n");
        fprintf(fout, "+----------+-----------+-------+\n");

        while (feq_node != NULL) {
            const char *tas =feq_node->event.type == ARRIVAL ? "ARRIVAL" : feq_node->event.type == TERMINATE ? "TERMINATE" : "UNKNOWN";
            fprintf(fout, "| %8d | %-9s | %5d |\n", feq_node->event.time,tas,feq_node->event.pid);
            feq_node = feq_node->next;
        }
        fprintf(fout, "+==============================+\n\n");
    }

// ================================================================================== //

} // end of namespace group

