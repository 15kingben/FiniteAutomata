How to build:
use include Makefile:  make

Running:
DFA:
./dfa <filename> <inputsequence>
e.g.:  ./dfa EvenOnes.txt 10011010

NFA:
./nfa <filename> <inputsequence>
e.g.: ./nfa NFA_washington.txt washh

NFA-DFAconverter:
./NFA-DFAconverter <filename> <inputsequence>
e.g.: ./NFA-DFAconverter NFA_EndMan.txt manohman

Notes:
-The dfas are defined in text files as follows:
<# of states>
<state#> <"Accepting"> <Accepting val (0/1)> <"Lambda"> <Lambda target> <"Transitions"> <Symbol> <symbol target> <symbol k> <symbol k target>

-The nfas differ in that they accept any number of Lambda states, do not include the word Transitions to signify the transitions, and accept strings in addition to symbols

-Using the nfa-dfa converter on NFA_washington.txt generates ~26,000 states and takes a long time, but it does work

-NFA_re is a NFA that accepts the regular expression: a|(bc*) as input
