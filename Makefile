# bzstream, C++ iostream classes wrapping the bzlib compression library.
# This is a fork of the library gzstream.
# 
# File			 : test_bunzip.C
# Revision		 : $Revision: 1.0 $
# Revision_date : $Date: 2019/01/12 02:06:31 $
#
# Original Author and License notice below.
#
# ============================================================================
# gzstream, C++ iostream classes wrapping the zlib compression library.
# Copyright (C) 2001  Deepak Bandyopadhyay, Lutz Kettner
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# ============================================================================
# 
# File          : Makefile
# Revision      : $Revision: 1.3 $
# Revision_date : $Date: 2001/10/04 15:09:28 $
# Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
# 
# ============================================================================

# ----------------------------------------------------------------------------
# adapt these settings to your need:
# add '-DBZSTREAM_NAMESPACE=name' to CPPFLAGS to place the classes
# in its own namespace. Note, this macro needs to be set while creating
# the library as well while compiling applications based on it.
# As an alternative, bzstream.C and bzstream.h can be edited.
# ----------------------------------------------------------------------------

# CXX      = CC -n32 -LANG:std   # for SGI Irix 6.5, MIPSpro CC version 7.30
CXX      = g++   # for Linux RedHat 6.1, g++ version 2.95.2

CPPFLAGS = -I. -O -DBZSTREAM_NAMESPACE=bz
LDFLAGS  = -L. -lbzstream -lbz2
AR       = ar cr

# ----------------------------------------------------------------------------
# plain simple rules to make and cleanup the library:
# make default;   compiles the library
# make test;      compiles and executes test. O.K. message marks success.
# make clean;     removes temporary files
# make cleanall;  removes temporary files, the library, and programs
# ----------------------------------------------------------------------------

default: libbzstream.a

test:    test_bzip test_bunzip
	./test_bzip COPYING.LIB bz.tmp.bz2
	bunzip2 bz.tmp.bz2
	diff COPYING.LIB bz.tmp
	bzip2 bz.tmp
	./test_bunzip bz.tmp.bz2 bz.tmp
	diff COPYING.LIB bz.tmp
	rm bz.tmp.bz2 bz.tmp
	# *** O.K. Test finished successfully. ***

install: libbzstream.a test
	cp libbzstream.a /usr/lib
	cp bzstream.h /usr/include
	chmod o+r /usr/include/bzstream.h
	# *** O.K. Installation finished successfully. ***

bzstream.o : bzstream.C bzstream.h
	${CXX} ${CPPFLAGS} -c -o bzstream.o bzstream.C

test_bzip.o : test_bzip.C bzstream.h
	${CXX} ${CPPFLAGS} -c -o test_bzip.o test_bzip.C

test_bunzip.o : test_bunzip.C bzstream.h
	${CXX} ${CPPFLAGS} -c -o test_bunzip.o test_bunzip.C

libbzstream.a : bzstream.o
	${AR} libbzstream.a bzstream.o

test_bzip : test_bzip.o libbzstream.a
	${CXX} -o test_bzip test_bzip.o ${LDFLAGS}

test_bunzip : test_bunzip.o libbzstream.a
	${CXX} -o test_bunzip test_bunzip.o ${LDFLAGS}

clean :
	rm *.o

cleanall :
	rm *.o libbzstream.a test_bzip test_bunzip

uninstall:
	rm /usr/include/bzstream.h /usr/lib/libbzstream.a
# ============================================================================
# EOF

