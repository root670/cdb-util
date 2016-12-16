/*
 * mystring.h - Small library of string functions
 *
 * Copyright (C) 2009-2012 Mathias Lafeldt <mathias.lafeldt@gmail.com>
 *
 * This file is part of libcheats.
 *
 * libcheats is licensed under the terms of the MIT License. See LICENSE file.
 */

#ifndef _MYSTRING_H_
#define _MYSTRING_H_

#include <stddef.h>

/* Some character defines */
#define NUL    0x00
#define TAB    0x09
#define LF    0x0A
#define CR    0x0D
#define SPACE    0x20

size_t chr_idx(const char *s, char c);
char *term_str(char *s, int(*callback)(const char *));
int trim_str(char *s);
int is_empty_str(const char *s);
int is_empty_substr(const char *s, size_t count);

#endif /* _MYSTRING_H_ */
