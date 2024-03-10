/**
 * \defgroup mem Memory Management (mem)
 *
 * \details
 *   Initially, the memory is empty, meaning that no process address space is hosted there.
 *   Thus, memory can be seen as a single region with a given dimension.
 *   In order for a process to be executed, one or more blocks of memory must be assigned to it,
 *   in order to host the process' address space segments.
 *   Every segment must be allocated in a continuous block of main memory.
 *
 *   The initial empty region is splitted such that there is a block for
 *   every segment of the process.
 *   To more processes have their address spaces hosted in memory, the empty part is
 *   successively divided.
 *   The way this division is done depends on the allocation policy used.
 *
 *   A running process eventually terminates its execution.
 *   Then, its assigned blocks of memory can be released, becoming available to other processes.
 *   The list of free blocks must be updated to include the released memory.
 *   Every released block may be adjacent to a existent free block or not.
 *   If it is adjacent, a merging must take place.
 *   Otherwise, a new element must be added to the list.
 *
 *   For the first fit approach,
 *   one can see the memory as a set of used blocks plus a set of available
 *   blocks, managing it with two lists:
 *   (a) a list of blocks allocated and assigned to processes;
 *   (b) list of free (not used) blocks.
 *   Both lists must be sorted in ascending order of memory addresses.
 *
 *   For the buddy system approach,
 *   the memory is splitted into halves until a block has enough size to host a segment
 *   but half of it has not.
 *
 *   In order to minimize its external fragmentation, memory is managed in chunks bigger
 *   than a single byte.
 *   This means that the amount of memory assigned to the process is the round up
 *   of the required amount to the chunk size.
 *
 *   The following table presents a list of the functions in this module, including:
 *   - the function name;
 *   - the function ID, that can be used to switch between the binary and group version;
 *   - an estimation of the effort required to implement it;
 *   - a brief description of the function role.
 *   <table>
 *   <tr> <th> \c function <th align="center"> function ID <th align="center"> level <th>role
 *   <tr> <td> \c memInit() <td align="center"> 501 <td> 1 (very low) + 1 (very low) + 2 (low) <td> Initialize the internal data structure of the MEM module
 *   <tr> <td> \c memTerm() <td align="center"> 502 <td> 1 (very low) + 1 (very low) + 2 (low) <td> Free dynamic memory used by the allocation algorithm and reset supporting data structures
 *   <tr> <td> \c memPrint() <td align="center"> 503 <td> 1 (low) + 2 (low) + 3 (low medium) <td> Print the internal state of the memory management module
 *   <tr> <td> \c memAlloc() <td align="center"> 504 <td> 5 (medium high) <td> Try to allocate the address space profile of a process, using the active allocation policy
 *   <tr> <td> \c memFirstFitAlloc() <td align="center"> 505 <td> 6 (high) <td> Try to allocate a block of memory of the given size, using the first fit algorithm
 *   <tr> <td> \c memBuddySystemAlloc() <td align="center"> 506 <td> 6 (high) <td> Try to allocate a block of memory of the given size, using the buddy system algorithm
 *   <tr> <td> \c memFree() <td align="center"> 507 <td> 3 (low medium) <td> Free a previously allocated address space mapping
 *   <tr> <td> \c memFirstFitFree() <td align="center"> 508 <td> 6 (high) <td> Free a previously (first fit) allocated block of memory
 *   <tr> <td> \c memBuddySystemFree() <td align="center"> 509 <td> 6 (high) <td> Free a previously (buddy system) allocated block of memory
 *   </table>
 *
 *  \author Artur Pereira - 2023
 */


#ifndef __SOMM23_MEM__
#define __SOMM23_MEM__

#include "tme.h"

#include <stdint.h>

/** @{ */

// ================================================================================== //

/**
 * \brief Indication that block allocation is not possible at the moment
 */
#define NULL_ADDRESS 0x0

// ================================================================================== //

/**
 * \brief Global memory parameters
 */
struct MemParameters {
    uint32_t chunkSize;      ///< The number of bytes of the unit of allocation
    uint32_t totalSize;      ///< The total amount of memory in bytes
    uint32_t kernelSize;     ///< The amount of memory used by the operating system
    AllocationPolicy policy; ///< The allocation policy in use
};

// ================================================================================== //

/**
 * \brief A memory block
 */
struct MemBlock {
    uint32_t pid;       ///< The PID of the process using the block; 0 if free
    uint32_t size;      ///< The size in bytes of the block
    Address address;    ///< The start address of the block
};

// ================================================================================== //

/**
 * \brief The node to support the linked lists used by the first fit algorithm
 */
struct MemListNode {
    MemBlock block;                 ///< A block o memory
    struct MemListNode *prev;       ///< A pointer to the previous node
    struct MemListNode *next;       ///< A pointer to the next node
};

// ================================================================================== //

/**
 * \brief Possible types of buddy system nodes
 * \details
 */
enum MemTreeNodeType { 
    FREE,       ///< represents a free block
    OCCUPIED,   ///< represents a block assigned to a process
    SPLITTED,   ///< represents a block that was splitted into two buddy blocks
};

// ================================================================================== //

/**
 * \brief The node to support the binary tree used by the buddy system algorithm
 */
struct MemTreeNode {
    MemTreeNodeType state;      ///< The state of the node: one of FREE, OCCUPIED, SPLITTED
    MemBlock block;                 ///< A block o memory
    struct MemTreeNode *left;       ///< A pointer to the left side sub-tree
    struct MemTreeNode *right;      ///< A pointer to the right side sub-tree
};

// ================================================================================== //

extern MemParameters memParameters;     ///< Global memory management parameters

extern MemListNode *memFreeHead;        ///< Head of the free list for first fit algorithm
extern MemListNode *memOccupiedHead;    ///< Head of the occupied list for first fit algorithm

extern MemTreeNode *memTreeRoot;        ///< Root of the buddy system tree

// ================================================================================== //

/**
 * \brief Initialize the internal data structure of the MEM module
 * \details
 *  The module's internal data structure, defined in file \c frontend/mem.cpp, 
 *  should be initialized appropriately.
 *  This data structure is composed of 4 variables:
 *  - \c memParameters, which holds global parameters;
 *  - \c memFreeHead and \c memOccupiedHead, which are the supporting variables for the first fit allocation policy,
 *    corresponding to two double linked-lists, 
 *    one for free blocks and another to blocks in used by processes;
 *  - \c memTreeRoot, which is the supporting variable for the buddy system allocation policy,
 *    corresponding to a binary tree, whose leaves represent both free and in-used blocks.
 *
 *  The following must be considered:
 *  - If policy is \c FirstFit, \c memTreeRoot must be put at NULL and \c memFreeHead and
 *    \c memOccupiedHead must be initialized properly.
 *  - If policy is \c BuddySystem, \c memFreeHead and \c memOccupiedHead must be put at NULL and
 *    \c memTreeRoot must be initialized properly.
 *  - The operating system should occupy the lower part of the available main memory.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] memSize Total amount of memory, in bytes, available
 * \param [in] memSizeOS The amount of memory used by the operating system, in bytes
 * \param [in] chunkSize The unit of allocation, in bytes
 * \param [in] policy The allocation policy to be used
 */
void memInit(uint32_t memSize, uint32_t memSizeOS, uint32_t chunkSize, AllocationPolicy policy);

// ================================================================================== //

/**
 * \brief Reset the internal data structure of the MEM module to the initial state
 * \details
 *   The dynamic memory used by the module's linked lists or binary tree must be released
 *   and the supporting data structure reset to the initial state.
 */
void memTerm();

// ================================================================================== //

/**
 * \brief Print the internal state of the MEM module
 * \details
 *  Two tables must be printed to the given file stream, 
 *  one containing the free blocks and the other the occupied blocks:
 *  - If the active policy is the first fit allocation, 
 *    the first table corresponds to the linked-list of free blocks,
 *    while the second one corresponds to the linked list of occupied blocks.
 *  - If the active policy is buddy system allocation,
 *    the binary tree must be traversed twice, one to print the free blocks
 *    and another to print the occupied blocks.
 *
 *  The following must be considered:
 *  - For the first fit policy, the linked-lists must be printed in natural order
 *  - For the buddy system policy, the binary tree must be travered in a depth-first approach
 *  - In both cases, the printing must appear in ascending order of block addresses.
 *  - The output must be the same as the one produced by the binary version.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] fout File stream where to send output 
 */
void memPrint(FILE *fout);

// ================================================================================== //

/**
 * \brief Try to allocate the address space profile of a process
 * \details
 *  This is the front end allocation function, that uses the \c memFirstFitAlloc or
 *  \c memBuddySystemAlloc, depending on the active allocation policy
 *
 *  The following must be considered:
 *  - Each segment size must be rounded up to the smallest multiple of the chunk size.
 *  - For each segment of the given profile, following the profile order,
 *    the appropriate block allocation function must be called,
 *    in order to fill an address space mapping.
 *  - If one of the segments fail to be allocated, the function should free the previously
 *    allocated blocks, before return NO_MAPPING.
 *  - If the memory required to allocate the whole address space exceds the total memory for
 *    processes, IMPOSSIBLE_MAPPING should be returned.<br>
 *    Note that the memory required depends on the allocation policy:
 *    - for the \c FirstFit policy, every allocated block has a size equal to the rounded up size
 *      of its corresponding segment;
 *    - for the \c BuddySystem policy, the allocated block may be bigger than the rounded up
 *      size, because of the division into halves.
 *    
 * \param [in] pid PID of the process requesting memory
 * \param [in] profile Pointer to a variable containing the process' address space profile
 * \return a valid pointer if succeed; 
 *   NO_MAPPING if not enough available memory at the moment; 
 *   IMPOSSIBLE_MAPPING if the memory requested is bigger then the whole memory available.
 */
AddressSpaceMapping *memAlloc(uint32_t pid, AddressSpaceProfile *profile);

// ================================================================================== //

/**
 * \brief Try to allocate a block of memory of the given size, using the first fit algorithm
 * \details
 *  This function may assume that the given size was already rounded up by the 
 *  front end allocation function.
 *
 *  The following must be considered:
 *  - Both linked-lists must be kept in ascending order of block addresses
 *  - The first free block, big enough to accommodate the requested size, must be used.
 *  - When a free block is splitted, the lower sub-block should be used for the allocation,
 *    and the upper sub-block should remain free.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] pid PID of the process requesting memory
 * \param [in] size Size of the block to be allocated, in bytes
 * \return The start address of the block allocated or \c NULL_ADDRESS if no block was found
 */
Address memFirstFitAlloc(uint32_t pid, uint32_t size);

// ================================================================================== //

/**
 * \brief Try to allocate a block of memory of the given size, using the buddy system algorithm
 * \details
 *  The buddy system allocation policy makes use of splitting memory into halves to try to give a best fit.
 *  It rilies on a binary tree to represents the state of the memory. 
 *  In this tree, leaf-nodes represent free and occupied blocks, while branch-nodes represent
 *  blocks that were splitted.
 *
 *  This function may assume that the given size was already rounded up by the 
 *  front end allocation function.
 *
 *  The following must be considered:
 *  - The leaf-nodes, when seen from left to right, represent blocks, free or occupied,
 *    appearing in ascending order of block addresses.
 *  - The first, best fit free block, according to a left-right, depth-first search, must be used.
 *  - The previous block, may have to be splitted, in acordance with the buddy system approach.
 *  - When a free block is splitted, the lower sub-block should be used for the allocation,
 *    and the upper sub-block should remain free.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] pid PID of the process requesting memory
 * \param [in] size Size of the block to be allocated, in bytes
 * \return The start address of the block allocated or \c NULL_ADDRESS if no block was found
 */
Address memBuddySystemAlloc(uint32_t pid, uint32_t size);

// ================================================================================== //

/**
 * \brief Free a previously allocated address space mapping
 * \details
 *  This is the front end free function, that uses the \c memFirstFitFree or
 *  \c memBuddySystemFree, depending on the active allocation policy,
 *  to free all blocks of the given mapping.
 *
 *  The following must be considered:
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] mapping Pointer to a variable containig the address space mapping to be freed
 */
void memFree(AddressSpaceMapping *mapping);

// ================================================================================== //

/**
 * \brief Free a previously allocated (first fit) block of memory
 * \details
 *
 *  The following must be considered:
 *  - If the block to be freed is contiguous to an empty block, merging must take place.
 *  - If address is not valid, the EINVAL exceptions must be thrown.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] address Start address of the block to be freed
 */
void memFirstFitFree(Address address);

// ================================================================================== //

/**
 * \brief Free a previously allocated (buddy system) block of memory
 * \details
 *
 *  The following must be considered:
 *  - If the block to be freed is contiguous to an empty block, merging must take place.
 *  - If address is not valid, the EINVAL exceptions must be thrown.
 *  - In case of an error, an appropriate exception must be thrown.
 *  - All exceptions must be of the type defined in this project (Exception).
 *
 * \param [in] address Start address of the block to be freed
 */
void memBuddySystemFree(Address address);

// ================================================================================== //

/** @} */

#endif /* __SOMM23_MEM__ */


