#include "IntSet.h"
#include "nfa.h"
#include "headers.h"

extern NFA* NFA_from_file(char* );
extern void NFA_print(NFA*);


int main(int argc, char** argv){

  char* filename = argv[1];
  NFA* nfa = NFA_from_file(filename);
  NFA_print(nfa);
  bool result = NFA_execute(nfa, argv[2]);

  if(result){
    printf("\naccept input \"%s\"\n", argv[2]);
  }else{
    printf("\nreject input \"%s\"\n", argv[2]);
  }

  return 1;
}
