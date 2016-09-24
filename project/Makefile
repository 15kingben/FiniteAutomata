#
# File: Makefile
# Creator: George Ferguson
# Created: Thu Jun 30 11:00:49 2016
# Time-stamp: <Wed Sep  7 18:01:09 EDT 2016 ferguson>
#

PROGRAMS = IntSetTest LinkedListTest DFAdriver NFAdriver NFA-DFAconverter

CFLAGS = -g 

programs: $(PROGRAMS)

IntSetTest: IntSet.o

LinkedListTest: LinkedList.o

NFA-DFAconverter: nfa.o dfa.o IntSet.o

DFAdriver: dfa.o

NFAdriver: nfa.o IntSet.o

clean:
	-rm $(PROGRAMS) *.o
	-rm -r *.dSYM
