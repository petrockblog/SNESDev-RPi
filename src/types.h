/*
 * SNESDev - User-space driver for the RetroPie GPIO Adapter for the Raspberry Pi.
 *
 * (c) Copyright 2012-2013  Florian Müller (contact@petrockblock.com)
 *
 * SNESDev homepage: https://github.com/petrockblog/SNESDev-RPi
 *
 * Permission to use, copy, modify and distribute SNESDev in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * SNESDev is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for SNESDev or software derived from SNESDev.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so everyone can benefit from the modifications
 * in future versions.
 *
 * Raspberry Pi is a trademark of the Raspberry Pi Foundation.
 */

#ifndef TYPES_H_
#define TYPES_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef signed char S8;
typedef unsigned char U8;
typedef signed int S16;
typedef unsigned int U16;
typedef signed long int S32;
typedef unsigned long int U32;
typedef signed long long int S64;
typedef unsigned long long int U64;
typedef float FLOAT;
typedef double DOUBLE;

typedef U32 TIME_T;
typedef U32 GPIOWIDTH_T;

#define U32_MIN   (0)
#define U32_MAX   (4294967295)

#define TIMET_MIN (U32_MIN)
#define TIMET_MAX (U32_MAX)

#endif /* TYPES_H_ */
