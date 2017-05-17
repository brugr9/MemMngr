/*
 * MemMngr_brugr9.h
 *
 * A (not thread-safe) memory manager. We used the GNU C++ compiler g++.
 *
 * File name and line number handling as found in:
 * Macro to replace C++ operator new
 * http://stackoverflow.com/questions/619467/macro-to-replace-c-operator-new
 *
 *  Created on: 12.05.2015
 *      Author: Roland Bruggmann (brugr9)
 */

#include "MemMngr_brugr9.h"

#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)

#include <new>
#undef new
#undef delete

/**
 * A global field for a static memory manager.
 */
static MemMngr memMngr;

/**
 * A global field for a static 'unknown'.
 */
static const char* UNKNOWN = "<unknown>";

/**
 * A global field for a file name.
 */
const char* __file__ = UNKNOWN;

/**
 * A global field for a line number.
 */
size_t __line__ = 0;

/**
 * New-operator.
 *
 * @param size size of element to allocate
 * @return pointer to the newly allocated element.
 * @throws bad_alloc
 */
void* operator new(std::size_t size) throw (std::bad_alloc) {
	void* ptr = memMngr.memAlloc(Single, size, __file__, __line__);
	__file__ = UNKNOWN;
	__line__ = 0;
	return ptr;
}

/**
 * New-array-operator.
 *
 * @param size size of element to allocate
 * @return pointer to the newly allocated array of elements
 * @throws bad_alloc
 */
void* operator new[](std::size_t size) throw (std::bad_alloc) {
	void* ptr = memMngr.memAlloc(Array, size, __file__, __line__);
	__file__ = UNKNOWN;
	__line__ = 0;
	return ptr;
}

/**
 * Delete-operator.
 *
 * @param p pointer to the element
 * @throws ()
 */
void operator delete(void* p) throw () {
	memMngr.memFree(Single, p, __file__, __line__);
	__file__ = UNKNOWN;
	__line__ = 0;
}

/**
 * Delete-array-operator.
 *
 * @param p pointer to the array
 * @throws ()
 */
void operator delete[](void* p) throw () {
	memMngr.memFree(Array, p, __file__, __line__);
	__file__ = UNKNOWN;
	__line__ = 0;
}

#endif /* DEBUG */
