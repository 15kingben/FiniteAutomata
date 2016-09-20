How to build:
use include Makefile:  make

Running:
DFA:
./dfa <filename> <inputsequence>
e.g.:  ./dfa EvenOnes.txt 10011010



Notes:
-The dfas are defined in text files as follows:
<# of states>
<state#> <"Accepting"> <Accepting val (0/1)> <"Lambda"> <Lambda target> <"Transitions"> <Symbol> <symbol target> <symbol k> <symbol k target>

-The nfas differ in that they accept any number of Lambda states, do not include the word Transitions to signify the transitions, and accept strings in addition to symbols

-If you want to test the empty string, use ""
