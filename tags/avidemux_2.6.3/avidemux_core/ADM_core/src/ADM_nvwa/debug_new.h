// -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim:tabstop=4:shiftwidth=4:expandtab:

/*
 * Copyright (C) 2004-2010 Wu Yongwei <adah at users dot sourceforge dot net>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software.  If you use this
 *    software in a product, an acknowledgement in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must
 *    not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 * This file is part of Stones of Nvwa:
 *      http://sourceforge.net/projects/nvwa
 *
 */

/**
 * @file    debug_new.h
 *
 * Header file for checking leaks caused by unmatched new/delete.
 *
 * @version 4.7, 2010/01/08
 * @author  Wu Yongwei
 *
 */

#ifndef _DEBUG_NEW_H
#define _DEBUG_NEW_H

#include <new>
#include <stdio.h>
#include "ADM_core6_export.h"

/**
 * @def _DEBUG_NEW_REDEFINE_NEW
 *
 * Macro to indicate whether redefinition of \c new is wanted.  If one
 * wants to define one's own <code>operator new</code>, or to call
 * <code>operator new</code> directly, it should be defined to \c 0 to
 * alter the default behaviour.  Unless, of course, one is willing to
 * take the trouble to write something like:
 * @code
 * # ifdef new
 * #   define _NEW_REDEFINED
 * #   undef new
 * # endif
 *
 * // Code that uses new is here
 *
 * # ifdef _NEW_REDEFINED
 * #   ifdef DEBUG_NEW
 * #     define new DEBUG_NEW
 * #   endif
 * #   undef _NEW_REDEFINED
 * # endif
 * @endcode
 */
#ifndef _DEBUG_NEW_REDEFINE_NEW
#define _DEBUG_NEW_REDEFINE_NEW 1
#endif

/**
 * @def _DEBUG_NEW_TYPE
 *
 * Macro to indicate which variant of \c #DEBUG_NEW is wanted.  The
 * default value \c 1 allows the use of placement new (like
 * <code>%new(std::nothrow)</code>), but the verbose output (when
 * \c #new_verbose_flag is \c true) looks worse than some older
 * versions (no file/line information for allocations).  Define it
 * to \c 2 to revert to the old behaviour that records file and line
 * information directly on the call to <code>operator new</code>.
 */
#ifndef _DEBUG_NEW_TYPE
#define _DEBUG_NEW_TYPE 1
#endif

/* Prototypes */
int check_leaks();
int check_mem_corruption();

ADM_CORE6_EXPORT void* operator new(size_t size, const char* file, int line);
ADM_CORE6_EXPORT void* operator new[](size_t size, const char* file, int line);
ADM_CORE6_EXPORT void operator delete(void* pointer, const char* file, int line) throw();
ADM_CORE6_EXPORT void operator delete[](void* pointer, const char* file, int line) throw();
ADM_CORE6_EXPORT size_t getSizeFromPointer(void* pointer);

#if defined(_MSC_VER) && _MSC_VER < 1300
// MSVC 6 requires the following declarations; or the non-placement
// new[]/delete[] will not compile.
void* operator new[](size_t) throw(std::bad_alloc);
void operator delete[](void*) throw();
#endif

/* Control variables */
extern bool new_autocheck_flag; // default to true: call check_leaks() on exit
extern bool new_verbose_flag;   // default to false: no verbose information
extern FILE* new_output_fp;     // default to stderr: output to console
extern ADM_CORE6_EXPORT const char* new_progname;// default to NULL; should be assigned argv[0]

/**
 * @def DEBUG_NEW
 *
 * Macro to catch file/line information on allocation.  If
 * #_DEBUG_NEW_REDEFINE_NEW is \c 0, one can use this macro directly;
 * otherwise \c new will be defined to it, and one must use \c new
 * instead.
 */
#if _DEBUG_NEW_TYPE == 1
#define DEBUG_NEW __debug_new_recorder(__FILE__, __LINE__) ->* new
#else
#define DEBUG_NEW new(__FILE__, __LINE__)
#endif

# if _DEBUG_NEW_REDEFINE_NEW
#   define new DEBUG_NEW
# endif
# ifdef _DEBUG_NEW_EMULATE_MALLOC
#   include <stdlib.h>
#   ifdef new
#     define malloc(s) ((void*)(new char[s]))
#   else
#     define malloc(s) ((void*)(DEBUG_NEW char[s]))
#   endif
#   define free(p) delete[] (char*)(p)
# endif

/**
 * Recorder class to remember the call context.
 *
 * The idea comes from <a href="http://groups.google.com/group/comp.lang.c++.moderated/browse_thread/thread/7089382e3bc1c489/85f9107a1dc79ee9?#85f9107a1dc79ee9">Greg Herlihy's post</a> in comp.lang.c++.moderated.
 */
class __debug_new_recorder
{
    const char* _M_file;
    const int   _M_line;
    void _M_process(void* pointer);
public:
    /**
     * Constructor to remember the call context.  The information will
     * be used in __debug_new_recorder::operator->*.
     */
    __debug_new_recorder(const char* file, int line)
        : _M_file(file), _M_line(line) {}
    /**
     * Operator to write the context information to memory.
     * <code>operator->*</code> is chosen because it has the right
     * precedence, it is rarely used, and it looks good: so people can
     * tell the special usage more quickly.
     */
    template <class _Tp> _Tp* operator->*(_Tp* pointer)
    { _M_process(pointer); return pointer; }
private:
    __debug_new_recorder(const __debug_new_recorder&);
    __debug_new_recorder& operator=(const __debug_new_recorder&);
};

/**
 * Counter class for on-exit leakage check.
 *
 * This technique is learnt from <em>The C++ Programming Language</em> by
 * Bjarne Stroustup.
 */
class __debug_new_counter
{
    static int _S_count;
public:
    __debug_new_counter();
    ~__debug_new_counter();
};
/** Counting object for each file including debug_new.h. */
static __debug_new_counter __debug_new_count;

#endif // _DEBUG_NEW_H
