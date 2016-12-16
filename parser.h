/*
 * parser.h - Parse cheats in text format
 *
 * Copyright (C) 2009-2012 Mathias Lafeldt <mathias.lafeldt@gmail.com>
 *
 * This file is part of libcheats.
 *
 * libcheats is licensed under the terms of the MIT License. See LICENSE file.
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include "cheatlist.h"

char parse_error_text[256];
int parse_error_line;

int parse_stream(gamelist_t *list, FILE *stream);
int parse_buf(gamelist_t *list, const char *buf);

#endif /* _PARSER_H_ */
