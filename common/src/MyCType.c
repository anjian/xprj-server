#include <common/inc/MyCType.h>

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#define _CTYPE_DATA_0_127 \
	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C, \
	X_TRAIT_C,	X_TRAIT_C|X_TRAIT_S, X_TRAIT_C|X_TRAIT_S, X_TRAIT_C|X_TRAIT_S,	X_TRAIT_C|X_TRAIT_S,	X_TRAIT_C|X_TRAIT_S,	X_TRAIT_C,	X_TRAIT_C, \
	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C, \
	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C,	X_TRAIT_C, \
	X_TRAIT_S|X_TRAIT_B,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P, \
	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P, \
	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_N, \
	X_TRAIT_N,	X_TRAIT_N,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P, \
	X_TRAIT_P,	X_TRAIT_U|X_TRAIT_X,	X_TRAIT_U|X_TRAIT_X,	X_TRAIT_U|X_TRAIT_X,	X_TRAIT_U|X_TRAIT_X,	X_TRAIT_U|X_TRAIT_X,	X_TRAIT_U|X_TRAIT_X,	X_TRAIT_U, \
	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U, \
	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U, \
	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_U,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P, \
	X_TRAIT_P,	X_TRAIT_L|X_TRAIT_X,	X_TRAIT_L|X_TRAIT_X,	X_TRAIT_L|X_TRAIT_X,	X_TRAIT_L|X_TRAIT_X,	X_TRAIT_L|X_TRAIT_X,	X_TRAIT_L|X_TRAIT_X,	X_TRAIT_L, \
	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L, \
	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L, \
	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_L,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_P,	X_TRAIT_C

#define _CTYPE_DATA_128_255 \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0, \
	0,	0,	0,	0,	0,	0,	0,	0









static const char _ctype_[1 + 256] = {
	_CTYPE_DATA_0_127,
	_CTYPE_DATA_128_255
};


int x_tolower(int c)
{
    if(c > 0 && (c & 0xff) <= 0xff)
    {
        if(x_isupper(c))
        {
            return c - 'A' + 'a' ;
        }
    }
    return c;
}

int x_ctype_lookup (int c)
{
    if(c > 0 && (c & 0xff) <= 0xff)
    {
        return _ctype_[c];
    }
    return 0;
}

