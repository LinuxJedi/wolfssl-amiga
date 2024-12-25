/* Copyright (C) 2024 Andrew (LinuxJedi) Hutchings
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#include <stdio.h>
#include <stabs.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <proto/timer.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include "amirng.h"

ABSDEF(potgo,  0x00dff034);
ABSDEF(pot1dat,0x00dff014);
ABSDEF(joy0dat,0x00dff00a);

volatile UWORD joy0dat;
volatile UWORD pot1dat;
volatile UWORD potgo;
ULONG last_time = 0;
struct timerequest* timereq = NULL;
struct MsgPort *port = NULL;
struct Device * TimerBase = NULL;

int init_rng()
{
	port = CreateMsgPort();
	if (!port) {
		printf("Couldn't create message port!\n");
		return 1;
	}
	timereq = (struct timerequest*) AllocMem(sizeof(struct timerequest), MEMF_CLEAR|MEMF_PUBLIC);
	if (!timereq) {
		printf("Couldn't alloc timerequest!\n");
		return 1;
	}

	uint32_t error = OpenDevice("timer.device", UNIT_WAITECLOCK, &timereq->tr_node, 0);
	if (error > 0) {
		printf("Couldn't open timer.device %d!\n", error);
		return 1;
	}
	TimerBase = (struct Device*) timereq->tr_node.io_Device;
	timereq->tr_node.io_Message.mn_ReplyPort = port;
	timereq->tr_node.io_Command = TR_ADDREQUEST;
	return 0;
}
void free_rng()
{
	if (timereq) {
		CloseDevice(&timereq->tr_node);
		FreeMem(timereq, sizeof(struct timerequest));
	}
	if (port) {
		DeleteMsgPort(port);
	}
}

/* Cannot use h/v sync positions for entropy because:
 * 1. The OS might be using the CIA timers for something else
 * 2. The multitasking switching is synced to them
 * Instead we use the low byte of these XOR'd:
 * - The joystick port analog input (16bit, floating analog input)
 * - The mouse position counter (16bit counter, increases with mouse movement)
 * - The low long word of Eclock counter (32bit counter, runs at ~700Hz)
 */
UBYTE amiga_rng(void)
{
    struct EClockVal clockval;
    UBYTE rng_out;
    UWORD old_pot = potgo;
    potgo = 0x0fff;
	do {
		ReadEClock(&clockval);
	} while (clockval.ev_lo == last_time);
	last_time = clockval.ev_lo;
    rng_out = pot1dat ^ joy0dat ^ clockval.ev_lo;
    potgo = old_pot;
    return rng_out;
}

int amiga_rng_gen_block(unsigned char* output, unsigned int sz)
{
    for (unsigned int i = 0; i < sz; i++)
    {
        output[i] = amiga_rng();
    }
    return 0;
}
