/**
 * \defgroup pct Process Control Table (pct)
 *
 * \details
 *   The Process Control Table (\c pct) is a mapping that associates a process with a tuple of data.
 *   The process itself is identified by a unique integer value, called Process Identification (\c PID).
 *   To every process, the following data, which represents the process control block (PCB),
 *   is considered:
 *   - the process identification (PID);
 *   - the state of execution, being one of NEW, ACTIVE, SWAPPED, FINISHED, or DISCARDED;
 *   - the time a which the process arrived to the system;
 *   - the lifetime of the process after it is transferred to main memory;
 *   - the time of process activation (transferred to the main memory),
 *     which corresponds to its start of execution;
 *   - the time the process ends its execution;
 *   - the process' address space profile;
 *   - the address mapping, that is where the different address space segments were allocated.
 *
 *   Note that, to be executed, a process needs its address space to be totally resident in memory.
 *   If no free memory, sufficient to host its address space, exist, 
 *   the process must wait until it is available.
 *   Thus, the time at which a process starts to be executed may be different from its arrival time.
 *
 *   The Process Control Table (\c PCT) module is responsible for storing this data
 *   allowing them to be accessed and updated.
 *
 *   The supporting data structure is a single linked list implemented from scratch in this module.
 *   Apart from a \c next pointer, every node contains a process control block (PCB), 
 *   that was already defined above.
 *   The list should be kept sorted in ascending order of the PID.
 *   
 *   The following table presents a list of the functions in this module, including:
 *   - the function name;
 *   - the function ID, that can be used to switch between the binary and group version;
 *   - an estimation of the effort required to implement it;
 *   - a brief description of the function role.
 *   <table>
 *   <tr><th>\c function <th align="center"> function ID <th align="center"> level <th>role
 *   <tr><td>\c pctInit() <td align="center"> 301 <td> 0 (trivial) <td> Initialize the support internal data structure
 *   <tr><td>\c pctTerm() <td align="center"> 302 <td> 2 (low) <td> Free and reset the support internal data structure
 *   <tr><td>\c pctPrint() <td align="center"> 303 <td> 4 (medium) <td> Print the internal state of the PCT table
 *   <tr><td>\c pctInsert() <td align="center"> 304 <td> 3 (low medium) <td> Insert a new entry in the PCT table
 *   <tr><td>\c pctGetLifetime() <td align="center"> 305 <td> 1 (very low) <td> Return the time of execution of a process
 *   <tr><td>\c pctGetAddressSpaceProfile() <td align="center"> 306 <td> 1 (very low) <td> Return the list of address space segments of the given process
 *   <tr><td>\c pctGetAddressSpaceMapping() <td align="center"> 307 <td> 1 (very low) <td> Return the list of memory blocks where the process was allocated 
 *   <tr><td>\c pctGetStateAsString() <td align="center"> 308 <td> 1 (very low) <td> Return the state as a string. given the state
 *   <tr><td>\c pctUpdateState() <td align="center"> 309 <td> 3 (low medium) <td> Sets the state of a process
 *   </table>
 *
 *  \author Artur Pereira - 2023
 */

#ifndef __SOMM23_PCT__
#define __SOMM23_PCT__

#include "tme.h"

#include <stdint.h>
#include <stdio.h>

/** @{ */

// ================================================================================== //

/**
 * \brief The Process Control Block data structure
 */
struct PctBlock {
    uint32_t pid;                       ///< PID of a process
    ProcessState state;                 ///< The state it is at a given moment
    uint32_t arrivalTime;               ///< The time of its arrival to the system
    uint32_t lifetime;                  ///< The amount of time it needs to be in memory for execution
    uint32_t activationTime;             ///< The time its address space was stored in main memory
    uint32_t finishTime;                ///< The time of its termination
    AddressSpaceProfile memProfile;     ///< Its address space profile
    AddressSpaceMapping memMapping;     ///< Its address space mapping
};

// ================================================================================== //

/**
 * \brief Node for the Process Control Table
 */
struct PctNode {
    PctBlock pcb;           ///< The process control block
    struct PctNode *next;   ///< Pointer to the next node
};

// ================================================================================== //

extern PctNode *pctHead;    ///< Pointer to head of list 

// ================================================================================== //

/**
 * \brief Initializes the internal data structure of the PCT module
 * \details
 *   The module's internal data structure, defined in file \c frontend/pct.cpp, 
 *   should be initialized properly
 *
 *   This is a quite trivial function.
 */
void pctInit();

// ================================================================================== //

/**
 * \brief Reset the internal data structure of the PCT module to the initial state
 * \details
 *   The dynamic memory used by the module's linked list must be released
 *   and the supporting data structure reset to the initial state.
 */
void pctTerm();

// ================================================================================== //

/**
 * \brief Print the internal state of the PCT module
 * \details
 *  The current state of the process control table (PCT) must be
 *  printed to the given file stream.
 *
 *  The following must be considered:
 *  - The linked-list elements should be printed in natural order.
 *  - The output must be the same as the one produced by the binary version.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] fout File stream where printing must be written to
 */
void pctPrint(FILE *fout);

// ================================================================================== //

/**
 * \brief Add a new entry to the PCT table
 * \details
 *  A new entry should be created and added to the linked-list that implements 
 *  the process control table.
 *
 *  The following must be considered:
 *  - the list's elements should be sorted in ascending order of the PID;
 *  - field \c state should be put at \c NEW
 *  - Field \c activationTime should be put at \c NO_TIME
 *  - Field \c finishTime should be put at \c NO_TIME
 *  - Field \c AddressSpaceMapping should be put with an empty mapping
 *  - The \c EINVAL exception should be thrown, if an entry for the given pid already exists
 *  - All exceptions must be of the type defined in this project (Exception)
 *  
 * \param [in] pid Id of process associated to the new entry
 * \param [in] time Time at which the process is submitted (arrival)
 * \param [in] lifetime Time the process takes to run, after it is in main memory
 * \param [in] memProfile Process' address space profile
 */
void pctInsert(uint32_t pid, uint32_t time, uint32_t lifetime, AddressSpaceProfile *memProfile);

// ================================================================================== //

/**
 * \brief Get process execution duration
 * \details
 *  The following must be considered:
 *  - The \c EINVAL exception should be thrown, if an entry for the given pid does not exist.
 *  - All exceptions must be of the type defined in this project (Exception).
 *  
 * \param [in] pid PID of the process
 * \return the process' lifetime
 */
uint32_t pctGetLifetime(uint32_t pid);

// ================================================================================== //

/**
 * \brief Get apointer to the process' address space profile
 * \details
 *  The following must be considered:
 *  - The \c EINVAL exception should be thrown, if an entry for the given pid does not exist.
 *  - All exceptions must be of the type defined in this project (Exception).
 *  
 * \param [in] pid PID of the process
 * \return a pointer to the process address space size
 */
AddressSpaceProfile *pctGetAddressSpaceProfile(uint32_t pid);

// ================================================================================== //

/**
 * \brief Get a pointer to the process' memory space mapping
 * \details
 *  The following must be considered:
 *  - The \c EINVAL exception should be thrown, if an entry for the given pid does not exist.
 *  - All exceptions must be of the type defined in this project (Exception).
 *  
 * \param [in] pid PID of the process
 * \return a pointer to the process' address space mapping
 */
AddressSpaceMapping *pctGetAddressSpaceMapping(uint32_t pid);

// ================================================================================== //

/**
 * \brief Return the process' state as a C-string (const char*)
 * \details
 *  The following must be considered:
 *  - The \c EINVAL exception should be thrown, if an entry for the given pid does not exist.
 *  - All exceptions must be of the type defined in this project (Exception).
 *  
 * \param [in] pid PID of the process
 * \return The process state as a string
 */
const char *pctGetStateAsString(uint32_t pid);

// ================================================================================== //

/**
 * \brief Update process state
 * \details
 *   Update the data of the given process.
 *   Fields to be updated depend on the new state of the process.
 *
 *   The following must be considered:
 *   - If the new state is ACTIVE, \c time refers to the activation time
 *   - If the new state is FINISHED, \c time refers to the finish time
 *   - If the new state is SWAPPED or DISCARDED, \c time is irrelevant
 *   - Argument \c mapping only applies when new state is ACTIVE
 *   - The \c EINVAL exception should be thrown, if an entry for the given pid does not exist.
 *   - All exceptions must be of the type defined in this project (Exception).
 *  
 * \param [in] pid PID of the process
 * \param [in] state The process new state
 * \param [in] time The time associated to the change of state
 * \param [in] mapping Pointer to the mappinf, if state is ACTIVE
 */
void pctUpdateState(uint32_t pid, ProcessState state, uint32_t time = NO_TIME, AddressSpaceMapping *mapping = NULL);

// ================================================================================== //

/** @} */

#endif /* __SOMM23_PCT__ */

