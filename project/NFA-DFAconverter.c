#include "IntSet.h"
#include "headers.h"
#include "nfa.h"
#include "dfa.h"

// extern void printState(int);
//
// extern IntSet* globalNextStates;
// extern char globalNextSymbol;
//
// extern NFA* globalNFA;
// extern bool globalAccept;
//
// extern void addToNextStates(int);
// extern void stateAccepts(int);
extern void NFA_print(NFA*);
extern NFA * NFA_from_file(char*);

bool DEBUG = false;

void makeNextStateTransitions(IntSet* [], int , int , NFA*, DFA* ,int *);


//crappy implentation of pow only defined for non-negative numbers
int int_pow(int x, int y){
  int total = 1;

  for (int i = 0; i < y; i++){
    total *= x;
  }
  return total;
}

bool global_oneAccepts = false;
NFA* global_boolNFA;

void containsAccepting(int state){
  if(global_boolNFA->states[state].is_accepting){
    global_oneAccepts = true;
  }
}

IntSet*** global_NFATransitions;

void genNFATransitions(NFA* nfa){
  global_NFATransitions = (IntSet***)malloc(NFA_get_size(nfa)*sizeof(IntSet**));
  for(int i = 0; i < NFA_get_size(nfa); i++){
    global_NFATransitions[i] =  (IntSet**)malloc(NFA_NSYMBOLS * sizeof(IntSet*));
  }

  for(int i = 0; i < NFA_get_size(nfa); i++){
    for(int j = 0; j < NFA_NSYMBOLS; j++){
      IntSet* stateTransitions = IntSet_new();
      IntSet_union(stateTransitions, nfa->states[i].transitions[j]);
      printf("\nState: %d Symbol %c Transitions: ", i, (char)j);
      IntSet_print(stateTransitions);
      global_NFATransitions[i][j] = stateTransitions;
    }
  }
}


DFA* convert_NFA_DFA(NFA* nfa, int *newDFASize ){
  int maxDFAStates = int_pow(2, NFA_get_size(nfa));
  printf("maxDFAStates: %d\n", maxDFAStates );
  IntSet* newStates[maxDFAStates/4]; //I would malloc but IntSet makes it difficult
  //newStates = malloc(maxDFAStates * sizeof(struct IntSet));


  int currentState = 0;
  int nextNewState = 1;


  DFA* dfa = DFA_new(maxDFAStates);

  //start w/ {0}
  newStates[0] = IntSet_new();
  IntSet_add(newStates[0], NFA_START_STATE);
  while(currentState < nextNewState){
    makeNextStateTransitions(newStates, maxDFAStates, currentState, nfa, dfa, &nextNewState);
    currentState++;
    if(DEBUG)
      printf("current state: %d, nextNewState: %d\n", currentState, nextNewState);
  }
  *newDFASize = currentState;

  //set accepting if one accepts
  for(int i = 0; i < currentState; i++){
    global_oneAccepts = false;
    global_boolNFA = nfa;
    IntSet_iterate(newStates[i], &containsAccepting);
    dfa->states[i].is_accepting = global_oneAccepts;
  }

  return dfa;
}

int findEquivalentState(IntSet* states[], int size, IntSet* target, int *nextNewState){
  for(int i = *nextNewState - 1; i >= 0; i--){ //more efficient b/c locality?

    //if we already have state {x,y,z...}
    if(IntSet_equals(states[i], target)){
      return i;
    }
  }

  //we didnt find state {x,y,z...}, better create it
  if(DEBUG)
    printf("nextNewState: %d\n", *nextNewState);
  states[(*nextNewState)++] = target;
  return (*nextNewState) - 1;
}



IntSet * global_nextStateSet;
NFA* global_nfa;
DFA* global_dfa;
int global_Symbol;




IntSet* global_IntSetdest;

void copyItems(int state){
  IntSet_add(global_IntSetdest, state);
}

void IntSet_copy(IntSet* dest, IntSet* src){
  global_IntSetdest = dest;
  IntSet_iterate(src, &copyItems);
}

void makeNextDFAState(int state){
  IntSet_copy(global_nextStateSet, global_NFATransitions[state][global_Symbol]);

  //IntSet_union(global_nextStateSet, NFA_get_transitions(global_nfa, state, global_Symbol));
}

void makeNextStateTransitions(IntSet* states[], int size, int currentState, NFA* nfa, DFA* dfa, int *nextNewState){
  IntSet* currentStateSet = states[currentState];
//  if(DEBUG){
    printf("DFA State:  %d  corresponds to the NFA States:  ", currentState);
    IntSet_print(currentStateSet);
//  }


  for(int i = 0; i < NFA_NSYMBOLS; i++){

    IntSet* nextStateSet = IntSet_new();
    global_nextStateSet = nextStateSet;
    // global_dfa = dfa;
    // global_nfa = nfa;
    global_Symbol = i;
    IntSet_iterate(currentStateSet, &makeNextDFAState);

    if(DEBUG){
      printf("char: %c nextStateSet: ", (char)i);
      IntSet_print(nextStateSet);
    }


    int dfa_target = findEquivalentState(states, size, nextStateSet, nextNewState);
    if(DEBUG)
    printf("DFA_Target: %d\n", dfa_target);
    DFA_set_transition(dfa, currentState, i, dfa_target);
  }
}


DFA* reduceSizeDFA(DFA* oldDFA, int dfaSize){
  DFA* newDFA = DFA_new(dfaSize);
  for(int i = 0; i < dfaSize; i++){
    for(int j = 0; j < DFA_NSYMBOLS; j++){
      newDFA->states[i].transitions[j] = oldDFA->states[i].transitions[j];
    }
    newDFA->states[i].is_accepting = oldDFA->states[i].is_accepting;
  }
  return newDFA;
}


int main(int argc, char** argv){

  char* filename = argv[1];
  NFA* nfa = NFA_from_file(filename);


  int newDFASize;

  genNFATransitions(nfa);
  printf("Summary of Conversion:\n");
  DFA* dfa = convert_NFA_DFA(nfa, &newDFASize);

  DFA* newdfa = reduceSizeDFA(dfa, newDFASize);

  printf("Now printing old NFA: \n");
  NFA_print(nfa);
  printf("\nNow printing new DFA\n");
  //DFA_print(newdfa);

  printf("Execution on both platforms:\nNFA:\n\n");

  bool result = NFA_execute(nfa, argv[2]);

  if(result){
    printf("\naccept input \"%s\"\n", argv[2]);
  }else{

    printf("\nreject input \"%s\"\n", argv[2]);
  }

  printf("\n\nNow DFA:\n\n");
  result = DFA_execute(dfa, argv[2]);
  if(result){
    printf("\naccept input \"%s\"\n", argv[2]);
  }else{

    printf("\nreject input \"%s\"\n", argv[2]);
  }

  return 1;
}
