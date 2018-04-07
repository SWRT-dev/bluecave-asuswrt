/* Definitions for MIPS running Linux-based GNU systems with ELF format.
   Copyright (C) 1998-2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define GLIBC_DYNAMIC_LINKER "/lib/ld.so.1"

#if TARGET_ENDIAN_DEFAULT == 0 /* LE */
#define MUSL_DYNAMIC_LINKER_E "%{EB:;:el}"
#else
#define MUSL_DYNAMIC_LINKER_E "%{EL:el}"
#endif
#define MUSL_DYNAMIC_LINKER "/lib/ld-musl-mips" MUSL_DYNAMIC_LINKER_E "%{msoft-float:-sf}.so.1"
