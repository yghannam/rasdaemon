# SPDX-License-Identifier: GPL-2.0
if WITH_PFA
noinst_LIBRARIES = libpfa.a
libpfa_a_SOURCES = core.c ghes.c amd.c dsm.c
libpfa_a_CFLAGS = -I$(top_srcdir) $(SQLITE3_CFLAGS) $(LIBTRACEEVENT_CFLAGS)
endif
