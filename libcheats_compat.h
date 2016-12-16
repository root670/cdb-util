/*
 * libcheats_compat.h - Compatibility defines
 *
 * Copyright (C) 2009-2012 Mathias Lafeldt <mathias.lafeldt@gmail.com>
 *
 * This file is part of libcheats.
 *
 * libcheats is licensed under the terms of the MIT License. See LICENSE file.
 */

#ifndef _LIBCHEATS_COMPAT_H_
#define _LIBCHEATS_COMPAT_H_

#ifdef HAVE_SYS_QUEUE_H
#include <sys/queue.h>
#else
#include "queue.h"
#endif /* HAVE_SYS_QUEUE_H */

//#ifdef HAVE_STDINT_H
#include <stdint.h>
/*#else
typedef char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
#endif*/ /* HAVE_STDINT_H */

#endif /* _LIBCHEATS_COMPAT_H_ */
