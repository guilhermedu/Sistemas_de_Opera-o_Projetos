/**
 *  \mainpage SO+FSO group work, academic year 2023-2024
 *
 *  \brief Simulating an FCFS processor scheduler based on contiguous memory allocation
 *    
 *  \author Artur Pereira - 2023
 *
 *  \details
 *    The purpose of this assignment is to implement a simulation system that simulates the activity
 *    of an FCFS processor scheduler based on a <b>contiguous memory allocation</b> approach.
 *    It is assumed that the system contains an infinity number of processors 
 *    and finite amount of memory to host the processors' address spaces.
 *    Thus, any process can be put running when it arrives to the system as long as
 *    enough memory to host its address space is available.
 *    Otherwise, it is put in a swap area while waiting for the availability of memory.
 *    Every process is considered to be composed of a single CPU burst, 
 *    whose lifetime in memory is known in advance.
 *
 * \anchor keyterms
 *
 *    The following terms are key for a good understanding of the system:
 *    - <b>pid</b> - process indentification, a positive, greater than zero integer value 
 *      that uniquely identifies a process;
 *    - <b>arrival time</b> - the time at which a process arrives to the system;
 *    - <b>lifetime in memory</b> - the interval of time a process must stay in main memory in order 
 *      to complete its execution;
 *    - <b>activation time</b> - the time at which a process' address space is put in main memory;
 *    - <b>finish time</b> - the time at which a process completes its execution, 
 *      and so the memory it uses can be released;
 *    - <b>address space profile</b> - the number of segments the address space of a process
 *      comprises and their sizes;
 *    - <b>block of memory</b> - a contiguous area in main memory;
 *    - <b>address space mapping</b> - the set of blocks in main memory used to host the 
 *      address space of a process;
 *    - <b>process state</b> - the state at which a process is at a given moment in time, being one
 *      of NEW, ACTIVE, SWAPPED, FINISHED, or DISCARDED
 *
 *    The system is composed of several main modules:
 *    - \c pct, which holds information about the processes being simulated, after their arrival to the system
 *    - \c mem, which deals with the allocation / deallocation of memory used by processes
 *    - \c feq, which deals with the future events that command the simulation
 *    - \c swp, which holds information about the swapped processes
 *    - \c sim, which provides means to make and control the simulation
 *
 *    There is also a number of auxiliary modules:
 *    - \c probing, which provides a probing mechanism
 *    - \c binselection, which allows to a binary version of a function
 *    - \c exception, which provides a way to throw exceptions
 * 
 * The simulation is driven by an input file which defines the arrival time of a list
 * of processes, along with their time of execution and memory needs for their 
 * address spaces.
 * The simulation must show how the scheduling takes place and where in memory were the
 * address spaces stored.
 *
 * Only modules \c pct, \c feq, \c mem, \c swp and \c sim are to be developed in this assignment.
 */

#ifndef __SOMM23__
#define __SOMM23__

/** 
 * \defgroup somm23 Main modules
 *  \brief 
 *    modules to be developed
 *
 * \defgroup tme Data Types, Macros and Enumerations (tme)
 * \ingroup somm23
 * \brief
 *   Declaration of data types, macros and enumerations common to two or more modules
 *   
 * \defgroup sim Simulation Management (sim)
 * \ingroup somm23
 * \brief 
 *   The <b>Simulation management</b> module provides means to run the simulation
 *
 * \defgroup feq Future Event Queue (feq)
 * \ingroup somm23
 * \brief 
 *   The <b>Future Event Queue (feq)</b> module deals with the events that 
 *   command the simulation.
 *
 * \defgroup pct Process Control Table (pct)
 * \ingroup somm23
 * \brief 
 *   The <b>Process Control Table (pct)</b> module holds information about
 *   the processes being simulated.
 *
 * \defgroup swp Swapped Process Queue (swp)
 * \ingroup somm23
 * \brief 
 *   The <b>Swap Process Queue (swp)</b> module deals with the processes that are swapped 
 *
 * \defgroup mem Memory Management (mem)
 * \ingroup somm23
 * \brief
 *   The <b>Memory Management (\c mem)</b> module deals with the allocation / deallocation
 *   of memory used by processes.
 *
 * <br>
 *
 * \defgroup aux Auxiliary modules
 * \brief Modules to aid in the development
 *
 * \defgroup exception Exception 
 * \ingroup aux
 * \brief 
 *   The <b>Exception</b> module provides a way to avoid defensive programming
 *   in the other components of the whole somm23 system.
 *
 * \defgroup probing Probing
 * \ingroup aux
 * \brief
 *   The <b>Probing toolkit</b> module provides a way to print probing
 *   messages that can be turned on/off
 *
 * \defgroup binselection BinSelection
 * \ingroup aux
 * \brief
 *   The <b>BinSelection toolkit</b> module provides a way to swith in run-time between
 *   the binary and group versions of the somm23 functions.
 *
 * \defgroup dbc DbC 
 * \ingroup aux
 * \brief Design-by-Contract module.
 *
 */

#include "dbc.h"
#include "exception.h"
#include "probing.h"
#include "binselection.h"

#include "tme.h"
#include "pct.h"
#include "feq.h"
#include "swp.h"
#include "mem.h"
#include "sim.h"

#endif /* __SOMM23__ */
