/*
 * DebugNew_brugr9.h
 *
 * A (not thread-safe) memory manager developed by using GNU C++ compiler g++.
 *
 * File name and line number handling as found in:
 * Macro to replace C++ operator new
 * http://stackoverflow.com/questions/619467/macro-to-replace-c-operator-new
 *
 *  Created on: 12.05.2015
 *      Author: Roland Bruggmann (brugr9)
 */

#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)

#ifndef DEBUGNEW_BRUGR9_H_
#define DEBUGNEW_BRUGR9_H_

/**
 * A global field for a file name.
 */
extern const char* __file__;

/**
 * A global field for a line number.
 */
extern size_t __line__;

/**
 * A macro for 'new'.
 *
 * The pre-compiler will replace any 'new' (operator) literally
 * by '(__file__=__FILE__,__line__=__LINE__,0) ? 0 : new'.
 *
 * By writing 'new' in debug mode -- e.g. <code>new MyClass();</code> -- it will be replaced by
 * <code>MyClass c = (__file__="./path/to/MyCode.cpp",__line__=42,0) ? 0 : new MyClass();</code>
 *
 * As (__file__="./path/to/MyCode.cpp",__line__=42,0) will be evaluated as <code>false</code>,
 * the code will call the 'real' operator <code>new</code> finally.
 */
#define new (__file__=__FILE__,__line__=__LINE__,0) ? NULL : new

/**
 * A macro for 'delete'.
 *
 * The pre-compiler will replace any 'delete' (operator) literally
 * by '(__file__=__FILE__,__line__=__LINE__,0) ? 0 : delete'.
 *
 * By writing 'delete' in debug mode -- e.g. <code>delete myClass;</code> -- it will be replaced by
 * <code>(__file__="./path/to/MyCode.cpp",__line__=42,0) ? 0 : delete myClass;</code>
 *
 * As (__file__="./path/to/MyCode.cpp",__line__=42,0) will be evaluated as <code>false</code>,
 * the code will call the 'real' operator <code>delete</code> finally.
 */
//#define delete (__file__=__FILE__,__line__=__LINE__,0) ? NULL : delete // TODO define delete (compiler says ... error: third operand to the conditional operator is of type ‘void’, but the second operand is neither a throw-expression nor of type ‘void’)

#endif /* DEBUGNEW_BRUGR9_H_ */

#endif /* DEBUG */

