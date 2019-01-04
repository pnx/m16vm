/* syscall.c
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
#include <stdio.h>
#include "syscall.h"

void vm_syscall(int16_t number, int8_t op, uint16_t* regs) {

	if (number == SYS_WRITE) {
		vm_syscall_write(op, regs[15]);
	}
}

void vm_syscall_write(int8_t op, int16_t value) {

	switch(op) {
	case SYSW_INT16 :
		printf("%i", value);
		break;
	case SYSW_INT8 :
		printf("%i", (int8_t) value);
		break;
	case SYSW_CHAR :
	default:
		printf("%c", (unsigned char) value);
		break;
	}
}
