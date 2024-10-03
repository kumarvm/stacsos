/* SPDX-License-Identifier: MIT */

/* StACSOS - Kernel
 *
 * Copyright (c) University of St Andrews 2024
 * Tom Spink <tcs6@st-andrews.ac.uk>
 */
#include <stacsos/kernel/sched/alg/rr.h>

using namespace stacsos::kernel::sched;
using namespace stacsos::kernel::sched::alg;

/* Called by the scheduling core when a thread becomes eligible to run on the CPU. 
 *
 * &tcb - address of thread control block (unit of scheduling)
 */
void round_robin::add_to_runqueue(tcb &tcb) { 
    runqueue_.append(&tcb);
}

/* Called by the scheduling core when a thread is no longer eligible to run. 
 *
 * &tcb - address of thread control block (unit of scheduling)
 */
void round_robin::remove_from_runqueue(tcb &tcb) { 
    runqueue_.remove(&tcb);
}

/* Called by the scheduler core when it is time for a new task to run.
 *
 * Dequeues the first task and appends it to the end of the runqueue, then returns the new first task of the runqueue.
 * The quantum (allowance of CPU time) for the round robin is allocated in core.cpp (core::run()) (100Hz).
 * *current - pointer to current thread control block (unit of scheduling)
 */
tcb *round_robin::select_next_task(tcb *current) {

    //For the case when there are no tasks to do
    if (runqueue_.empty()) {
		return nullptr;
	}

    //For the case when the runqueue is of size 1
	if (runqueue_.count() == 1) {
		return runqueue_.first();
	}

    tcb* first = runqueue_.dequeue();
    runqueue_.append(first); //Could have used .append() or .enqueue(); both append the element to the end of the list (as per the stacsos specification).

    return first; 
}
