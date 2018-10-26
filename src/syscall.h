/* syscall.h
*
*   Copyright (C) 2012,2014-2015   Henrik Hautakoski <henrik.hautakoski@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
*   MA 02110-1301, USA.
*/

#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

#define SYSW_INT16  0x0
#define SYSW_INT8   0x1
#define SYSW_CHAR   0x2

void syscall_write(int16_t value, int8_t op);

#endif /* SYSCALL_H */
