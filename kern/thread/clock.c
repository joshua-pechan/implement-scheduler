/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <types.h>
#include <lib.h>
#include <cpu.h>
#include <wchan.h>
#include <clock.h>
#include <thread.h>
#include <current.h>

/*
 * Time handling.
 *
 * This is pretty primitive. A real kernel will typically have some
 * kind of support for scheduling callbacks to happen at specific
 * points in the future, usually with more resolution than one second.
 *
 * A real kernel also has to maintain the time of day; in OS/161 we
 * skimp on that because we have a known-good hardware clock.
 */

/*
 * Timing constants. These should be tuned along with any work done on
 * the scheduler.
 */
#define SCHEDULE_HARDCLOCKS 4 /* Reschedule every 4 hardclocks. */
#define MIGRATE_HARDCLOCKS 16 /* Migrate every 16 hardclocks. */

/*
 * Once a second, everything waiting on lbolt is awakened by CPU 0.
 */
static struct wchan *lbolt;
static struct spinlock lbolt_lock;

/*
 * Setup.
 */
void hardclock_bootstrap(void)
{
	spinlock_init(&lbolt_lock);
	lbolt = wchan_create("lbolt");
	if (lbolt == NULL)
	{
		panic("Couldn't create lbolt\n");
	}
}

/*
 * This is called once per second, on one processor, by the timer
 * code.
 */
void timerclock(void)
{
	/* Just broadcast on lbolt */
	spinlock_acquire(&lbolt_lock);
	wchan_wakeall(lbolt, &lbolt_lock);
	spinlock_release(&lbolt_lock);
}

/*
 * This is called HZ times a second (on each processor) by the timer
 * code.
 */
void hardclock(void)
{
	/*
	 * Collect statistics here as desired.
	 */

	curcpu->c_hardclocks++;
	if ((curcpu->c_hardclocks % MIGRATE_HARDCLOCKS) == 0)
	{
		thread_consider_migration();
	}
	// if ((curcpu->c_hardclocks % SCHEDULE_HARDCLOCKS) == 0) {
	// 	schedule();
	// }
	// thread_timeryield();
}

/*
 * Suspend execution for n seconds.
 */
void clocksleep(int num_secs)
{
	spinlock_acquire(&lbolt_lock);
	while (num_secs > 0)
	{
		wchan_sleep(lbolt, &lbolt_lock);
		num_secs--;
	}
	spinlock_release(&lbolt_lock);
}
