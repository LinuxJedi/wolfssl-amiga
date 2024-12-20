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

#include <stabs.h>
#include <exec/types.h>
#include <proto/timer.h>
#include <proto/dos.h>
#include <wolfcrypt/test/test.h>
#include <wolfcrypt/benchmark/benchmark.h>

ABSDEF(potgo,  0x00dff034);
ABSDEF(pot1dat,0x00dff014);
ABSDEF(joy0dat,0x00dff00a);

volatile UWORD joy0dat;
volatile UWORD pot1dat;
volatile UWORD potgo;

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
    struct Device * TimerBase = DOSBase->dl_TimeReq->tr_node.io_Device;
    struct EClockVal clockval;
    UBYTE rng_out;
    UWORD old_pot = potgo;

    potgo &= 0x0fff;
    unsigned int m_result = ReadEClock(&clockval);
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

int main(void)
{
//    return wolfcrypt_test(NULL);
    return benchmark_test(NULL);
}
