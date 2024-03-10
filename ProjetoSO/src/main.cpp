/*
 *  
 *  ...
 *
 *  \author Artur Pereira - 2023
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <termios.h>

#include <string>
#include <iostream>
#include <map>

#include "somm23.h"


/* ******************************************** */
/* print help message */
static uint32_t chunkSize = 0x100;
static uint32_t memSize= 0x1000 * 0x100;
static uint32_t osSize= 0x100 * 0x100;
static void printUsage(const char *cmd_name)
{
    printf("Sinopsis: %s [OPTIONS] \n"
           "  OPTIONS:\n"
           "  -i infile     --- set input file (default: none)\n"
           "  -o outfile    --- set output file (default: stdout)\n"
           "  -f buddy      --- set buddy system as the allocation policy (default: first fit)\n"
           "  -c size       --- chunk size (default: %u or %#x)\n"
           "  -m size       --- total memory size, in bytes, (default: %u or %#x)\n"
           "  -k address    --- memory size, in bytes, used by (kernel) OS (default: %u or %#x)\n"
           "  -O outfile    --- set probing file (default: stdout)\n"
           "  -P num-num    --- set probing map to given ID range (default: 0-0)\n"
           "  -A num-num    --- add range of IDs to probing map\n"
           "  -R num-num    --- remove range of IDs from probing map\n"
           "  -b            --- set bin selection map to 100-599\n"
           "  -g            --- set bin selection map to 0-0 (default)\n"
           "  -a num-num    --- add range of IDs to bin selection map\n"
           "  -r num-num    --- remove range of IDs from bin selection map\n"
           "  -h            --- print this help\n", 
           cmd_name, chunkSize, chunkSize, memSize, memSize, osSize, osSize);
}

/* ******************************************** */
/*
 * pause simulation
 */
bool pauseSim(const char *prompt = "")
{
    static bool firstTime = true;
    static struct termios prev, cur;
    if (firstTime)
    {
        firstTime = false;
        tcgetattr(STDIN_FILENO, &prev);
        cur = prev;
        cur.c_lflag &= (~ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &cur);
    }

    printf("%s. Continue (Y/n)? ", prompt); fflush(stdout);
    while (true)
    {
        int res = getchar();
        if (res == '\n') break;
        printf("\n");
        if (res == 'n' or res == 'N') return false;
        if (res == 'y' or res == 'Y') break;
        printf("Bad option! Continue (Y/n)? "); fflush(stdout);
    }
    return true;
}

/* ******************************************** */
/* The main function */
int main(int argc, char *argv[])
{
    const char *progName = basename(argv[0]); 

    /* open probing system, all */
    FILE *fout = stdout;
    soProbeOpen(stdout, 0, 0);
    soProbeSetIDs(0, 0);

    /* default values for command line options */
    AllocationPolicy memPolicy = FirstFit;
    const char *infile = NULL;
    const char *outfile = NULL;

    /* process command line options */
    int opt;
    while ((opt = getopt(argc, argv, "i:o:f:k:m:c:O:P:A:R:bga:r:h")) != -1)
    {
        switch (opt)
        {
            case 'i':       // set input file
            {
                infile = optarg;
                break;
            }
            case 'o':       // set output file
            {
                outfile = optarg;
                if ((fout = fopen(outfile, "w")) == NULL)
                {
                    fprintf(stderr, "%s: Bad argument (%s): fail opening file.\n", progName, optarg);
                    return EXIT_FAILURE;
                }
                break;
            }
            case 'f':
            {
                if (optarg[0] == 'b') memPolicy = BuddySystem;
                break;
            }
            case 'c':          // set memory chunk size
            {
                int n = 0;
                sscanf(optarg, "%u%n", &chunkSize, &n);
                if ((size_t)n == strlen(optarg)) break;
                n = 0;
                sscanf(optarg, "%x%n", &chunkSize, &n);
                if ((size_t)n == strlen(optarg)) break;
                fprintf(stderr, "%s: Bad argument (%s) to '-c' option.\n", progName, optarg);
                return EXIT_FAILURE;
            }
            case 'm':          // set memory size
            {
                int n = 0;
                sscanf(optarg, "%u%n", &memSize, &n);
                if ((size_t)n == strlen(optarg)) break;
                n = 0;
                sscanf(optarg, "%x%n", &memSize, &n);
                if ((size_t)n == strlen(optarg)) break;
                fprintf(stderr, "%s: Bad argument (%s) to '-m' option.\n", progName, optarg);
                return EXIT_FAILURE;
            }
            case 'k':          // set memory size used by OS
            {
                int n = 0;
                sscanf(optarg, "%u%n", &osSize, &n);
                if ((size_t)n == strlen(optarg)) break;
                n = 0;
                sscanf(optarg, "%x%n", &osSize, &n);
                if ((size_t)n == strlen(optarg)) break;
                fprintf(stderr, "%s: Bad argument (%s) to '-k' option.\n", progName, optarg);
                return EXIT_FAILURE;
            }
            case 'O':          /* set probbing file */
            {
                soProbeFile(optarg);
                break;
            }
            case 'P':          /* set ID range to probing system */
            {
                uint32_t lower, upper;
                uint32_t cnt = 0;
                if ( (sscanf(optarg, "%d%*[,-]%d %n", &lower, &upper, &cnt) != 2) 
                        or (cnt != strlen(optarg)) )
                {
                    fprintf(stderr, "%s: Bad argument to '-p' option.\n", progName);
                    printUsage(progName);
                    return EXIT_FAILURE;
                }
                soProbeSetIDs(lower, upper);
                break;
            }
            case 'A':          /* add IDs to probe conf */
            {
                uint32_t lower, upper;
                uint32_t cnt = 0;
                if ( (sscanf(optarg, "%d%*[,-]%d %n", &lower, &upper, &cnt) != 2) 
                        or (cnt != strlen(optarg)) )
                {
                    fprintf(stderr, "%s: Bad argument to '-A' option.\n", basename(argv[0]));
                    printUsage(basename(argv[0]));
                    return EXIT_FAILURE;
                }
                soProbeAddIDs(lower, upper);
                break;
            }
            case 'R':          /* remove IDs from probe conf */
            {
                uint32_t lower, upper;
                uint32_t cnt = 0;
                if ( (sscanf(optarg, "%d-%d %n", &lower, &upper, &cnt) != 2) 
                        or (cnt != strlen(optarg)) )
                {
                    fprintf(stderr, "%s: Bad argument to '-R' option.\n", basename(argv[0]));
                    printUsage(basename(argv[0]));
                    return EXIT_FAILURE;
                }
                soProbeRemoveIDs(lower, upper);
                break;
            }
            case 'b':  // set binary mode TODO
            {
                soBinSetIDs(100, 599);
                break;
            }
            case 'g':  // set binary mode  TODO
            {
                soBinSetIDs(0, 0);
                break;
            }
            case 'a':          /* add IDs to probe conf */
            {
                uint32_t lower, upper;
                uint32_t cnt = 0;
                if ( (sscanf(optarg, "%d%*[,-]%d %n", &lower, &upper, &cnt) != 2) 
                        or (cnt != strlen(optarg)) )
                {
                    fprintf(stderr, "%s: Bad argument to '-A' option.\n", basename(argv[0]));
                    printUsage(basename(argv[0]));
                    return EXIT_FAILURE;
                }
                soBinAddIDs(lower, upper);
                break;
            }
            case 'r':          /* remove IDs from probe conf */
            {
                uint32_t lower, upper;
                uint32_t cnt = 0;
                if ( (sscanf(optarg, "%d-%d %n", &lower, &upper, &cnt) != 2) 
                        or (cnt != strlen(optarg)) )
                {
                    fprintf(stderr, "%s: Bad argument to '-R' option.\n", basename(argv[0]));
                    printUsage(basename(argv[0]));
                    return EXIT_FAILURE;
                }
                soBinRemoveIDs(lower, upper);
                break;
            }
            case 'h':
            {
                printUsage(progName);
                return 0;
            }
            default:
            {
                fprintf(stderr, "%s: Wrong option (\"-%c\".\n", progName, opt);
                printUsage(progName);
                return EXIT_FAILURE;
            }
        }
    }

    fprintf(fout, "\n\e[34;1mStarting simulation\e[0m\n\n");
    simInit(memSize, osSize, chunkSize, memPolicy);
    if (infile != NULL)
    {
        simLoad(infile);
    }else{
        simRandomFill(30,10);
    }
    simPrint(fout); pctPrint(fout); feqPrint(fout); swpPrint(fout); memPrint(fout);
    /* put your testing code here */

    /* for instance, consider that you are testing the feqInsert function */
    /*
    fprintf(fout, "\n\e[31;1m=================================================================\e[0m\n");
    fprintf(fout, "\e[34;1mTesting feqInsert\e[0m\n");
    fprintf(fout, "\e[31;1m=================================================================\e[0m\n\n");
    feqInsert(TERMINATE, 100, 9999);
    feqInsert(TERMINATE, 1000, 8888);
    feqPrint(fout);
    */

    /* memInit(0x3000, 0x300, 0x300, FirstFit);
    memInit(0x1000, 0x100, 0x100, BuddySystem);
    AddressSpaceProfile profile;
    profile.segmentCount = 1;
    profile.size[0] = 0x100;
    AddressSpaceMapping *map = memAlloc(1, &profile);
    if (map == NULL_ADDRESS)
    {
        fprintf(stderr, "Error allocating memory\n");
        return EXIT_FAILURE;
    }

    AddressSpaceProfile profile2;
    profile2.segmentCount = 1;
    profile2.size[0] = 0x100;
    AddressSpaceMapping *map2 = memAlloc(2, &profile2);
    if (map2 == NULL_ADDRESS)
    {
        fprintf(stderr, "Error allocating memory\n");
        return EXIT_FAILURE;
    }
    memPrint(fout);

    memBuddySystemAlloc(1, 0x500);
    memPrint(fout);
    
    
    memFirstFitAlloc(1, 0x100);
    memPrint(fout);
    Address endereco = (uint32_t)0x300;
    memFirstFitFree(endereco);
    memPrint(fout); 
    memFree(map);
    memPrint(fout);  */
    
    //simRun(0);

    while(!feqIsEmpty()){
        pauseSim();
        simStep();
        simPrint(fout); pctPrint(fout); feqPrint(fout); swpPrint(fout); memPrint(fout);
    }
   
    fprintf(fout, "\n\e[34;1mTerminating simulation\e[0m\n\n");
    simTerm();
}

//     fprintf(fout, "\n\e[31;1m=================================================================\e[0m\n");
//     fprintf(fout, "\e[34;1mTesting Sim\e[0m\n");
//     fprintf(fout, "\e[31;1m=================================================================\e[0m\n\n");

//     simRun(0);
//     simPrint(fout); pctPrint(fout); feqPrint(fout); swpPrint(fout); memPrint(fout);

//     /*  Testing getProcess 
//     // Teste com processo existente da nossa randomfill
//     fprintf(fout, "\n\e[31;1m=================================================================\e[0m\n");
//     fprintf(fout, "\e[34;1mTesting getProcess\e[0m\n");
//     fprintf(fout, "\e[31;1m=================================================================\e[0m\n\n");
//     ForthcomingProcess *process = simGetProcess(47711); 
//     fprintf(fout, "Process ID: %u\n", process->pid);
//     fprintf(fout, "Arrival Time: %u\n", process->arrivalTime);
//     fprintf(fout, "Lifetime: %u\n", process->lifetime);
//     fprintf(fout, "Segment Count: %u\n", process->addressSpace.segmentCount);
//     fprintf(fout, "Segment Sizes: ");
//     for (uint32_t i = 0; i < process->addressSpace.segmentCount; i++) {
//         fprintf(fout, "%u ", process->addressSpace.size[i]);
//     }
//     fprintf(fout, "\n"); */

//     /* Testing simRun*/
//     /* fprintf(fout, "\n\e[31;1m=================================================================\e[0m\n");
//     fprintf(fout, "\e[34;1mTesting simRun\e[0m\n");
//     fprintf(fout, "\e[31;1m=================================================================\e[0m\n\n");
//     simRun(1); // Test with 1 step
//     simRun(2); // Test with 2 steps
//     simRun(3); // Test with 3 steps
//     simRun(4); // Test with 4 steps
//     simRun(5); // Test with 5 steps */

//     fprintf(fout, "\n\e[34;1mTerminating simulation\e[0m\n\n");
//     simTerm();
// }


// /* prof.txt */

// /*simRun(2)
// (107) simStep()
// (206) feqIsEmpty()
// (205) feqPop()
// (106) simGetProcess(4513)
// (304) pctInsert(4513, 40, 740, 0x5fdff0)
// (504) memAlloc(4513, 0x5fdff0)
// (505) memFirstFitAlloc(4513, 0x700)
// (505) memFirstFitAlloc(4513, 0x700)
// (505) memFirstFitAlloc(4513, 0x700)
// (204) feqInsert(TERMINATE, 780, 4513)
// (309) pctUpdateState(4513, 1, 40)
// (107) simStep()
// (206) feqIsEmpty()
// (205) feqPop()
// (106) simGetProcess(65251)
// (304) pctInsert(65251, 150, 840, 0x5fe0d0)
// (504) memAlloc(65251, 0x5fe0d0)
// (505) memFirstFitAlloc(65251, 0x600)
// (505) memFirstFitAlloc(65251, 0x700)
// (505) memFirstFitAlloc(65251, 0x300)
// (204) feqInsert(TERMINATE, 990, 65251)
// (309) pctUpdateState(65251, 1, 150)
// */


// /*meu.txt*/

// /*(108) simRun(2)
// (107) simStep()
// (206) feqIsEmpty()
// (205) feqPop()
// (106) simGetProcess(4513)
// (304) pctInsert(4513, 40, 740, 0x5fdff0)
// (504) memAlloc(4513, 0x5fdff0)
// (505) memFirstFitAlloc(4513, 0x700)
// (505) memFirstFitAlloc(4513, 0x700)
// (505) memFirstFitAlloc(4513, 0x700)
// (204) feqInsert(TERMINATE, 780, 4513)
// (309) pctUpdateState(4513, 1, 40)
// (107) simStep()
// (206) feqIsEmpty()
// (205) feqPop()
// (106) simGetProcess(65251)
// (304) pctInsert(65251, 150, 840, 0x5fe0d0)
// (504) memAlloc(65251, 0x5fe0d0)
// (505) memFirstFitAlloc(65251, 0x600)
// (505) memFirstFitAlloc(65251, 0x700)
// (505) memFirstFitAlloc(65251, 0x300)
// (204) feqInsert(TERMINATE, 990, 65251)
// (309) pctUpdateState(65251, 1, 150)
// */
