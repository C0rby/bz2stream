// bzstream, C++ iostream classes wrapping the bzlib compression library.
// This is a fork of the library gzstream.
// 
// File			 : bzstream.h
// Revision		 : $Revision: 1.0 $
// Revision_date : $Date: 2019/01/12 01:58:42 $
//
// Original Author and License notice below.
//
// ============================================================================
// gzstream, C++ iostream classes wrapping the zlib compression library.
// Copyright (C) 2001  Deepak Bandyopadhyay, Lutz Kettner
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ============================================================================
//
// File          : gzstream.h
// Revision      : $Revision: 1.5 $
// Revision_date : $Date: 2002/04/26 23:30:15 $
// Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
// 
// Standard streambuf implementation following Nicolai Josuttis, "The 
// Standard C++ Library".
// ============================================================================

#ifndef BZSTREAM_H
#define BZSTREAM_H 1

// standard C++ with new header file names and std:: namespace
#include <iostream>
#include <fstream>
#include <bzlib.h>

#ifdef BZSTREAM_NAMESPACE
namespace BZSTREAM_NAMESPACE {
#endif

// ----------------------------------------------------------------------------
// Internal classes to implement bzstream. See below for user classes.
// ----------------------------------------------------------------------------

class bzstreambuf : public std::streambuf {
private:
    static const int bufferSize = 47+256;    // size of data buff
    // totals 512 bytes under g++ for igzstream at the end.

    BZFILE*          file;               // file handle for compressed file
    char             buffer[bufferSize]; // data buffer
    char             opened;             // open/close state of stream
    int              mode;               // I/O mode

    int flush_buffer();
public:
    bzstreambuf() : opened(0) {
        setp( buffer, buffer + (bufferSize-1));
        setg( buffer + 4,     // beginning of putback area
              buffer + 4,     // read position
              buffer + 4);    // end position      
        // ASSERT: both input & output capabilities will not be used together
    }
    int is_open() { return opened; }
    bzstreambuf* open( const char* name, int open_mode);
    bzstreambuf* close();
    ~bzstreambuf() { close(); }
    
    virtual int     overflow( int c = EOF);
    virtual int     underflow();
    virtual int     sync();
};

class bzstreambase : virtual public std::ios {
protected:
    bzstreambuf buf;
public:
    bzstreambase() { init(&buf); }
    bzstreambase( const char* name, int open_mode);
    ~bzstreambase();
    void open( const char* name, int open_mode);
    void close();
    bzstreambuf* rdbuf() { return &buf; }
};

// -----------------------------------------------------------------------------
// User classes. Use ibzstream and obzstream analogously to ifstream and
// ofstream respectively. They read and write files based on the bz* 
// function interface of the bzlib. Files are compatible with bzip2 compression.
// -----------------------------------------------------------------------------

class ibzstream : public bzstreambase, public std::istream {
public:
    ibzstream() : std::istream( &buf) {} 
    ibzstream( const char* name, int open_mode = std::ios::in)
        : bzstreambase( name, open_mode), std::istream( &buf) {}  
    bzstreambuf* rdbuf() { return bzstreambase::rdbuf(); }
    void open( const char* name, int open_mode = std::ios::in) {
        bzstreambase::open( name, open_mode);
    }
};

class obzstream : public bzstreambase, public std::ostream {
public:
    obzstream() : std::ostream( &buf) {}
    obzstream( const char* name, int mode = std::ios::out)
        : bzstreambase( name, mode), std::ostream( &buf) {}  
    bzstreambuf* rdbuf() { return bzstreambase::rdbuf(); }
    void open( const char* name, int open_mode = std::ios::out) {
        bzstreambase::open( name, open_mode);
    }
};

#ifdef BZSTREAM_NAMESPACE
} // namespace BZSTREAM_NAMESPACE
#endif

#endif // BZSTREAM_H
// ============================================================================
// EOF //

