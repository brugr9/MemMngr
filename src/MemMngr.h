/*
 * MemMngr.h
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

#define DEBUG
#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)

#include <iostream>
#include <stdlib.h>
using namespace std;
#include <string>
#include <assert.h>

#ifndef MEMMNGR_H_
#define MEMMNGR_H_

/**
 * Forward declaration: List template class.
 */
template<class T> class List;

/**
 * A doubly-linked node template class.
 */
template<class T> class Node {

	/**
	 * This class is intended to be used in a doubly-linked list.
	 */
	friend class List<T> ;

private:

	/**
	 * No-void-constructor.
	 */
	Node(void);

public:

	/**
	 * Constructor with an element as parameter.
	 *
	 * @param element The element.
	 */
	Node(const T& element) :
			itsElement(element), itsPrevious(NULL), itsNext(NULL) {
		;
		// Nothing to do.
	}

	/**
	 * Node destructor.
	 */
	virtual ~Node(void) {
		init();
	}

private:

	/**
	 * A field for an element of type <code>T</code> pointer.
	 */
	T* itsElement;

	/**
	 * A field for a previous node pointer.
	 */
	Node<T>* itsPrevious;

	/**
	 * A field for a next node pointer.
	 */
	Node<T>* itsNext;

public:

	/**
	 * Initialises all fields.
	 */
	void init(void) {
		itsElement = NULL;
		itsPrevious = NULL;
		itsNext = NULL;
	}

	/**
	 * {@inheritDoc}
	 */
	Node<T>& operator=(const Node<T>& node) {
		swap(node);
		return *this;
	}

	/**
	 * Swaps this node with a node as given (copy-and-swap).
	 *
	 * @param node The node to swap with.
	 */
	void swap(const Node<T>& node) {
		if (this != &node) {
			itsElement = node.itsElement;
			itsPrevious = node.itsPrevious;
			itsNext = node.itsNext;
		}
	}

};

/**
 * A doubly-linked list template class.
 */
template<class T> class List {

public:

	/**
	 * Main void constructor.
	 */
	List(void) :
			itsSize(0), itsFirst(NULL), itsLast(NULL) {
		;
		// Nothing to do.
	}

	/**
	 * Doubly linked list destructor.
	 *
	 * Removes all elements from the list by calling the method <code>clear()</code>
	 * (does *not* free the memory used by this elements).
	 * @see clear()
	 */
	virtual ~List(void) {
		clear();
	}

protected:

	/**
	 * A field for its size (number of nodes).
	 */
	unsigned int itsSize;

	/**
	 * A field for a first node pointer.
	 */
	Node<T>* itsFirst;

	/**
	 * A field for a last node pointer.
	 */
	Node<T>* itsLast;

public:

	/**
	 * Initialises all fields.
	 */
	void init(void) {
		itsSize = 0;
		itsFirst = NULL;
		itsLast = NULL;
	}

	/**
	 * Adds an element to the end of the list.
	 * Returns <code>true</code> if the element was added, <code>false</code> otherwise.
	 *
	 * @param element the element to add
	 * @return <code>true</code> if the element was added, <code>false</code> otherwise
	 */
	bool add(const T& element) throw (bad_alloc) {

		// assert that counter 'itsSize' is growing
		assert(itsSize < (itsSize + 1));

		// allocate some memory for the node
		Node<T>* node = static_cast<Node<T>*>(malloc(sizeof(Node<T> )));
		if (!node) {
			throw bad_alloc();
		}
		node->init();
		node->itsElement = (T*) element;

		// pointer nesting
		switch (itsSize) {
		case 0:
			itsFirst = node;
			break;
		case 1:
			itsFirst->itsNext = node;
			node->itsPrevious = itsFirst;
			break;
		default:
			itsLast->itsNext = node;
			node->itsPrevious = itsLast;
			break;
		}
		itsLast = node;

		// update size counter
		itsSize++;
		return true;
	}

	/**
	 * Returns a pointer to the first node.
	 *
	 * @return A pointer to the first node.
	 */
	const Node<T>* first(void) const {
		return (const Node<T>*) itsFirst;
	}

	/**
	 * Returns a pointer to the last node.
	 *
	 * @return A pointer to the last node.
	 */
	const Node<T>* last(void) const {
		return (const Node<T>*) itsLast;
	}

	/**
	 * Returns the previous node of a given one.
	 *
	 * @param node A node.
	 * @return The previous node of the given one.
	 */
	const Node<T>* previous(const Node<T>* node) const {
		return (const Node<T>*) node->itsPrevious;
	}

	/**
	 * Returns the next node of a given one.
	 *
	 * @param node A node.
	 * @return The next node of the given one.
	 */
	const Node<T>* next(const Node<T>* node) const {
		return (const Node<T>*) node->itsNext;
	}

	/**
	 * Returns a pointer to the element of a given node.
	 *
	 * @param node The node to get the element pointer from.
	 * @return The pointer to the element of the node as given
	 */
	const T* getElement(const Node<T>* node) const {
		return (const T*) node->itsElement;
	}

	/**
	 * Returns <code>true</code> if the element got removed, <code>false</code> otherwise.
	 * If the element is not found, <code>false</code> will be returned.
	 * This method does **not** free the memory used by the element.
	 *
	 * @param element The element to remove
	 * @return <code>true</code> if the element got removed, <code>false</code> otherwise
	 */
	bool remove(const T& element) {

		// start at last node and find element as given
		Node<T>* current = itsLast;
		while ((T*) element != current->itsElement) {
			current = current->itsPrevious;
			if (!current) {
				return false;
			}
		}

		// pointer nesting
		Node<T>* previous = current->itsPrevious;
		Node<T>* next = current->itsNext;
		if (previous) {
			previous->itsNext = next;
		} else {
			itsFirst = next;
		}
		if (next) {
			next->itsPrevious = previous;
		} else {
			itsLast = previous;
		}
		previous = NULL;
		next = NULL;

		// current node
		current->Node < T > ::~Node();
		free(current);
		current = NULL;

		// update size
		itsSize--;
		return true;
	}

	/**
	 * Removes all elements from the list starting with its last element.
	 * This method does **not** free the memory used by the elements.
	 */
	void clear(void) {
		T* element = NULL;
		do {
			element = itsLast->itsElement;
		} while (remove(*element));
	}

	/**
	 * Returns the size of this list.
	 *
	 * If this list contains more than <code>size_t</code> elements,
	 * returns <code>size_t</code>.
	 *
	 * @return the size of this list
	 */
	const unsigned int size(void) const {
		return itsSize;
	}
};

/**
 * An enumeration of memory types.
 */
enum MemType {

	/**
	 * A flag for a memory of type 'single'.
	 */
	Single = 0,

	/**
	 * A flag for a memory of type 'array'.
	 */
	Array = 1
};

/**
 * A memory entry structure.
 */
typedef struct MemEntry {

	/**
	 * A field for a memory pointer.
	 */
	void* itsMemPtr;

	/**
	 * A field for its memory size.
	 */
	size_t itsSize;

	/**
	 * A field for its memory type.
	 */
	MemType itsType;

	/**
	 * A field for its allocation number.
	 */
	unsigned int itsAllocNumber;

	/**
	 * A field for a pointer to the file name
	 * where the memory was allocated from.
	 */
	const char* itsFile;

	/**
	 * A field for a line number
	 * where the memory was allocated from.
	 */
	size_t itsLine;

	/**
	 * Initialises all fields.
	 */
	void init(void) {
		itsMemPtr = NULL;
		itsSize = 0;
		itsType = Single;
		itsAllocNumber = 0;
		itsFile = NULL;
		itsLine = 0;
	}

} MemEntry_t;

/**
 * A list of memory entry pointers.
 */
class MemList: public List<MemEntry_t*> {

public:

	/**
	 * Returns an element (memory entry pointer) found by a memory pointer as key.
	 *
	 * @param memPtr The pointer to the memory.
	 * @return The pointer to the <code>MemEntry_t</code> if found, NULL otherwise.
	 */
	const MemEntry_t* getEntry(const void* memPtr) const {

		MemEntry_t* entry = NULL;

		// find element by starting at last node
		MemEntry_t* candidate = NULL;
		const Node<MemEntry_t*>* node = itsLast;

		while (node) {
			candidate = (MemEntry_t*) getElement(node);
			if (memPtr == candidate->itsMemPtr) {
				entry = candidate;
				break;
			} else
				node = previous(node);
		}

		return entry;
	}

};

/**
 * Memory manager.
 */
class MemMngr {

public:

	/**
	 * Main void constructor.
	 */
	MemMngr(void) :
			itsAllocCounter(0), itsMemSize(0), itsMemSizeMax(0) {

		itsMemEntries = static_cast<MemList*>(malloc(sizeof(MemList)));
		if (!itsMemEntries) {
			throw bad_alloc();
		}
		itsMemEntries->init();

		cout << "MemMngr::MemMngr()" << endl;
	}

	/**
	 * Memory manager destructor.
	 */
	virtual ~MemMngr(void) {
		atExit();
		itsMemEntries->MemList::~MemList();
		free(itsMemEntries);
	}

private:

	/**
	 * A field for an allocation counter.
	 */
	unsigned int itsAllocCounter;

	/**
	 * A field for the size of actually managed memory.
	 */
	unsigned int itsMemSize;

	/**
	 * A field for the maximum size of memory used.
	 */
	unsigned int itsMemSizeMax;

	/**
	 * A field for an allocation limit of at least 1GB.
	 */
	static const size_t ALLOCLIMIT = 1073741824;

	/**
	 * A field for a list of memory entries to manage.
	 */
	MemList* itsMemEntries;

public:

	/**
	 * Allocates memory.
	 *
	 * @param type The memory type.
	 * @param size The size of memory to allocate.
	 * @param file The file name where the memory is allocated from.
	 * @param line The line number where the memory is allocated from.
	 * @return A pointer to the newly allocated memory.
	 */
	void* memAlloc(MemType type, size_t size, const char* file, size_t line)
			throw (bad_alloc) {

		// assert allocation limit
		assert(itsMemSize + size < ALLOCLIMIT);

		// allocate some memory for memory management #:-/
		MemEntry_t* entry = static_cast<MemEntry_t*>(malloc(sizeof(MemEntry_t)));
		if (!entry) {
			throw bad_alloc();
		}
		entry->init();

		// allocate some memory as requested
		void* memPtr = malloc(size);
		if (!memPtr) {
			throw bad_alloc();
		}

		// update statistics
		itsMemSize += size;
		itsMemSizeMax = max(itsMemSize, itsMemSizeMax);
		itsAllocCounter++;

		// manage entry
		entry->itsMemPtr = memPtr;
		entry->itsSize = size;
		entry->itsType = type;
		entry->itsAllocNumber = itsAllocCounter;
		entry->itsFile = file;
		entry->itsLine = line;
		itsMemEntries->add(entry);

		// print info to standard-out
		printAlloc(*entry);

		// done.
		return memPtr;
	}

	/**
	 * Frees some memory.
	 *
	 * @param type The memory type.
	 * @param p A pointer to the memory to free.
	 * @param file The file name where freeing memory is called from.
	 * @param line The line number where freeing memory is called from.
	 */
	void memFree(MemType type, void* p, const char* file, size_t line) throw () {

		// manage entry
		MemEntry_t* entry = (MemEntry_t*) itsMemEntries->getEntry(p);
		itsMemEntries->remove(entry);
		itsMemSize -= entry->itsSize;

		// print info to standard-out
		printFree(type, *entry, file, line);

		// free memory
		free(entry);
		entry = NULL;
		free(p);
		p = NULL;
	}

private:

	/**
	 * Prints an entry to standard-out (memory allocation formatted).
	 *
	 * @param entry The entry to print to standard-out.
	 */
	void printAlloc(const MemEntry_t& entry) const {
		cout << "operator new";
		entry.itsType == Array ? cout << "[]" : cout << "  ";
		cout << "(" << entry.itsSize << "):\t" << entry.itsMemPtr << ": #"
				<< entry.itsAllocNumber << endl;
	}

	/**
	 * Prints an entry to standard-out (free-memory formatted).
	 *
	 * @param type The memory type.
	 * @param entry The entry to print to standard-out.
	 * @param file The file name where freeing memory was called from.
	 * @param line The line number where freeing memory was called from.
	 */
	void printFree(const MemType& type, const MemEntry_t& entry,
			const char* file, size_t& line) const {
		cout << "operator delete";
		(type == Array) ? cout << "[]" : cout << "  ";
		cout << ":" << "\t" << entry.itsMemPtr << ", " << "#"
				<< entry.itsAllocNumber;
		if (type != entry.itsType) {
			cout << ", *** Wrong delete ***";
			// cout << "\t" << file << ":" << line; // TODO when solved 'define delete': print file and line here
		}
		cout << endl;
	}

private:

	/**
	 * Memory leak reporting: prints to console some information
	 * about memory usage and leakage.
	 */
	void atExit(void) const {
		unsigned int leaks = itsMemEntries->size();

		cout << "--------------------------------------------------------"
				<< endl;
		cout << "MemMngr::atExit()" << endl;

		// print statistics
		cout << "No. of memory leaks: " << "\t" << leaks << endl;
		cout << "Total size of leaks: " << "\t" << itsMemSize << " bytes"
				<< endl;
		cout << "Max. memory used: " << "\t" << itsMemSizeMax << " bytes"
				<< endl;

		// print leaks starting from last allocation
		if (leaks) {
			cout << endl << "Memory leaks:" << endl;

			// print a line per entry
			MemEntry_t* entry = NULL;
			const Node<MemEntry_t*>* node = itsMemEntries->last();
			do {
				// get and print entry
				entry = (MemEntry_t*) itsMemEntries->getElement(node);
				cout << "#" << entry->itsAllocNumber << ",";
				cout << "\t" << entry->itsMemPtr << ", size: "
						<< entry->itsSize;
				cout << "\t" << entry->itsFile << ":" << entry->itsLine;
				cout << endl;

				// free memory -- avoid leaks
				free(entry->itsMemPtr);
				free(entry);

				// next node
				node = itsMemEntries->previous(node);
			} while (node);
		}

		// done.
		cout << "--------------------------------------------------------"
				<< endl;
	}

}
;

#endif /* MEMMNGR_H_ */

#endif /* DEBUG */
