#include "dfa.h"
#include "headers.h"
#include "IntSet.h"

extern DFA* DFA_from_file(char*);

int main(int argc, char **argv) {
  DFA* testdfa;


  char* filename = argv[1];
  testdfa = DFA_from_file( filename);
  DFA_print(testdfa);


  char seq[1000] = "";
  if(argv[2] == NULL){
    //seq = "";
  }else{
    strcpy(seq, argv[2]);
  }

  printf("Result: %d\n", DFA_execute(testdfa, seq));
  DFA_free(testdfa);
  return 1;
}
