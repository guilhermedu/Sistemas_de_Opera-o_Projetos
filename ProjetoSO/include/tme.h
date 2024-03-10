/**
 * \defgroup tme Data Types, Macros and Enumerations (tme)
 * \brief Declaration of data types, macros and enumerations common to two or more modules
 *
 * \author Artur Pereira - 2023
 *
 *
 */

#ifndef __SOMM23_TME__
#define __SOMM23_TME__

#include "dbc.h"
#include "exception.h"
#include "probing.h"
#include "binselection.h"

#include <stdint.h>

/** @{ */

// ================================================================================== //

/**
 * \brief Maximum number of simulated processes
 */
#define MAX_PROCESSES 100

/**
 * \brief Maximum number of segments per process
 */
#define MAX_SEGMENTS 4

/**
 * \brief Maximum number of memory blocks per process
 */
#define MAX_BLOCKS (MAX_SEGMENTS)

// ================================================================================== //

/**
 * \brief Indication that not time was already assigned
 */
#define NO_TIME 0xFFFFFFFF

/**
 * \brief Indication that profile allocation is not possible at the moment
 */
#define NO_MAPPING ((AddressSpaceMapping*)0x0)

/**
 * \brief Indication that profile allocation is not possible at all
 */
#define IMPOSSIBLE_MAPPING ((AddressSpaceMapping*)0xFFFFFFFF)

// ================================================================================== //

/**
 * \brief Possible states a process can be, after arriving to the system
 * \details
 */
enum ProcessState { 
    NEW,        ///< a process just arrived to the system
    ACTIVE,     ///< the process is executing
    SWAPPED,    ///< enough memory is not available yet, so process is waiting for space in swap area
    FINISHED,   ///< the process has already finished its execution
    DISCARDED   ///< the process has an address space bigger than the whole available main memory
};

// ================================================================================== //

/**
 * \brief Possible types of future events
 * \details
 */
enum FutureEventType { 
    ARRIVAL = 0b001,   ///< The arrival of a process to the system
    TERMINATE = 0b010  ///< A running process terminates its execution
};

// ================================================================================== //

/**
 * \brief The future event
 * \details
 *   Simulation is driven by events that will occur in the future.
 *   A <b>future event</b> is a tuple composed of:
 *   - the identification of the process generating the event
 *   - the type of event, being one of ARRIVAL ot TERMINATE
 *   - the time of occurrence of the event
 */
struct FutureEvent {
    uint32_t pid;           ///< Identification of a process
    FutureEventType type;   ///< The type of event that will be produced by the process
    uint32_t time;          ///< The time at which the event will occur
};

// ================================================================================== //

/**
 * \brief Possible memory allocation policies
 * \details
 */
enum AllocationPolicy { 
    FirstFit,        ///< First fit policy is used in the allocation procedure
    BuddySystem      ///< Buddy system policy is used in the allocation procedure
};

// ================================================================================== //

/*
 * \brief A memory address is represented by an integer value
 */
typedef uint32_t Address; ///< The representation of a memory address

// ================================================================================== //

/**
 * \brief Number and sizes of the memory segments a process comprises
 * \details
 *   The address space of a process is composed of one or more segments of memory.
 *   This data structure indicates the number of segments and the size of every one.
 *   The number of segments is limited to \c MAX_BLOCKS
 */
struct AddressSpaceProfile {
    uint32_t segmentCount;      ///< Number of segments in profile
    uint32_t size[MAX_BLOCKS];  ///< Array with sizes of the segments
};

// ================================================================================== //

/**
 * \brief Number and sizes of the main memory blocks assigned to a process
 * \details
 *   Every segment of a process needs to be mapped to a block in main memory.
 *   All the segments must be in main memory for the process to execute.
 *   This data structure indicates the number of blocks assigned to the process and the
 *   start address of every one.
 */
struct AddressSpaceMapping {
    uint32_t blockCount;            ///< Number of memory blocks im mapping
    Address address[MAX_BLOCKS];    ///< array with initial addresses of the blocks
};

// ================================================================================== //

/**
 * \brief Register of a swapped process
 */
struct SwappedProcess {
    uint32_t pid;                   ///< Identification of a process
    AddressSpaceProfile profile;    ///< Its address space profile
};

// ================================================================================== //

/** @} */

#endif /* __SOMM23_TME__ */
