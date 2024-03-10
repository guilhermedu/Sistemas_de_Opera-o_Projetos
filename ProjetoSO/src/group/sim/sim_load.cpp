/*
 *  André e Guilherme Chapelo Andrade
 */

#include "somm23.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>
#include <algorithm>
namespace group
{

// ================================================================================== //

    void simLoad(const char *fname)
    {
        soProbe(104, "%s(\"%s\")\n", __func__, fname);

        require(fname != NULL, "fname can not be a NULL pointer");
        require(forthcomingTable.count == 0, "Forthcoming table should be empty");

        std::ifstream file(fname);
        if (!file) {
            throw Exception(errno, "Failed to open file");
        }
        
        std::vector<ForthcomingProcess> tempProcesses;

        std::string line;
        uint32_t lastArrivalTime = 0;
        while (std::getline(file, line)) {
            // ignora os comentarios e linhas vazias
            if (line.empty() || line[0] == '%') continue;

            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;

            // separa a linha em tokens
            while (std::getline(iss, token, ';')) {
                tokens.push_back(token);
            }

            if (tokens.size() != 4) {
                throw Exception(EINVAL, "Invalid line format");
            }

            // analisa os tokens
            uint32_t pid = std::stoul(tokens[0]);
            uint32_t arrivalTime = std::stoul(tokens[1]);
            uint32_t lifetime = std::stoul(tokens[2]);

            // analisa o address space
            AddressSpaceProfile addressSpace;
            std::istringstream issProfile(tokens[3]);
            std::string segmentSize;
            addressSpace.segmentCount = 0;
            while (std::getline(issProfile, segmentSize, ',')) {
                addressSpace.size[addressSpace.segmentCount++] = std::stoul(segmentSize);
            }

            ForthcomingProcess process;
            process.pid = pid;
            process.arrivalTime = arrivalTime;
            process.lifetime = lifetime;
            process.addressSpace = addressSpace;
            tempProcesses.push_back(process);

            // validaçoes (provavelmente falta alguma coisa e a exceção não é esta)
            if (lifetime <= 0) {
                throw Exception(EINVAL, "Lifetime must be greater than zero");
            }
            if (forthcomingTable.count >= MAX_PROCESSES) {
                throw Exception(EINVAL, "Exceeded max number of processes");
            }


            // Adiciona o processo à tabela
            forthcomingTable.process[forthcomingTable.count].pid = pid;
            forthcomingTable.process[forthcomingTable.count].arrivalTime = arrivalTime;
            forthcomingTable.process[forthcomingTable.count].lifetime = lifetime;
            forthcomingTable.process[forthcomingTable.count].addressSpace = addressSpace;
            forthcomingTable.count++;

            // atualiza o lastArrivalTime
            lastArrivalTime = arrivalTime; 


            feqInsert(ARRIVAL,arrivalTime, pid);
            
        }

        std::sort(tempProcesses.begin(), tempProcesses.end(), 
            [](const ForthcomingProcess &a, const ForthcomingProcess &b) {
                return a.arrivalTime < b.arrivalTime;
            });

        for (size_t i = 0; i < tempProcesses.size(); i++) {
            forthcomingTable.process[i] = tempProcesses[i];
        }
        forthcomingTable.count = tempProcesses.size();
    }

// ================================================================================== //

} // end of namespace group

