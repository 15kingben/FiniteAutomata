
#include "IntSet.h"
#include "headers.h"
#include "nfa.h"

NFA* NFA_new(int nstates){
  NFA* nfa = (NFA*)malloc(sizeof(NFA));
  nfa->current_states = IntSet_new();
  IntSet_add(nfa->current_states, NFA_START_STATE);
  nfa->states = (NFA_State*)malloc(nstates*(sizeof(NFA_State)));
  nfa->nstates = nstates;
  for(int i = 0; i < nstates; i++){
    for(int j = 0; j < NFA_NSYMBOLS; j++){
      nfa->states[i].transitions[j] = IntSet_new();
    }
  }
  return nfa;
}

void NFA_free(NFA* nfa){
  for(int i = 0; i < NFA_get_size(nfa); i++){
    for(int j = 0; j < NFA_NSYMBOLS; j++){
      IntSet_free(nfa->states[i].transitions[j]);
    }
  }
  free(nfa->states);
  IntSet_free(nfa->current_states);
}


int NFA_get_size(NFA* nfa){
  return nfa->nstates;
}


IntSet *NFA_get_transitions(NFA *nfa, int statenum, char symbol){
  return nfa->states[statenum].transitions[symbol];
}

void NFA_add_transition(NFA *nfa, int src, char symbol, int dst){
    IntSet_add(nfa->states[src].transitions[symbol], dst);
}

void NFA_add_transition_str(NFA *nfa, int src, char *str, int dst){
  for (int i = 0; i < strlen(str); i++){
    char symbol=str[i];
    IntSet_add(nfa->states[src].transitions[symbol], dst);
  }
}

void NFA_add_transition_all(NFA *nfa, int src, int dst){
  for(int i = 0; i < NFA_NSYMBOLS; i++){
    IntSet_add(nfa->states[src].transitions[i], dst);
  }
}


IntSet* NFA_get_current_states(NFA* nfa){
  return nfa->current_states;
}

void NFA_set_current_states(NFA* nfa, IntSet *states){
  //here were just assigning the pointer to an existing state
  nfa->current_states = states;
}

void NFA_set_current_state(NFA *nfa, int state){
  IntSet_add(nfa->current_states, state);
}

bool NFA_get_accepting(NFA *nfa, int statenum){
  return nfa->states[statenum].is_accepting;
}

void NFA_set_accepting(NFA *nfa, int statenum, bool value){
   nfa->states[statenum].is_accepting = value;
}

NFA* globalNFA;
bool globalAccept;

void stateAccepts(int state){
  if (globalNFA->states[state].is_accepting)
    globalAccept = true;
}

bool NFA_is_accepting(NFA *nfa){
  //im sure there is a better way to do this, if you know how please email me
  globalNFA = nfa;
  globalAccept = false;
  IntSet_iterate(nfa->current_states, &stateAccepts);
  return globalAccept;
}

void printState(int state){
  printf("%d, ", state);
}

void NFA_print(NFA *nfa){
  for (int i = 0; i < nfa->nstates; i++){
    printf("State %d(%d): ", i , NFA_get_accepting(nfa, i));
    for(int j = 0; j < NFA_NSYMBOLS; j++){
      if(IntSet_is_empty(nfa->states[i].transitions[j]))
        continue;
      printf("%c -> ", (char)j);
      IntSet_iterate(nfa->states[i].transitions[j], &printState);
      printf("| ");
    }
    printf("\n");
  }
}



/*This method inputs a NFA from a text file*/
NFA *  NFA_from_file(char* filename){
   FILE *fp;
   fp = fopen(filename, "r");
   if(fp == NULL){
     printf("Cannot find file: %s", filename);
     exit(0);
   }

   char delim[2] = " ";
   char buffer[1000];
   char *buff;
   int num;

   int numstates;
   fgets(buffer, sizeof(buffer), fp);
   buff = buffer;
   numstates = strtol(buff, &buff, 10);

   NFA* nfa = NFA_new(numstates);

     while (true) {

       if ( !fgets(buffer, sizeof(buffer), (FILE*)fp) ) {
          break;
        }

        buff = buffer;

        int state;
        int isAccepting;
        char * token;

        //state number
        token = strtok(buff, delim);
        sscanf(token,  "%d", &state);



        //Accepting _
        char* accString = strtok(NULL, delim);
        token = strtok(NULL, delim);
        sscanf(token, "%d", &isAccepting);



        NFA_set_accepting(nfa, state, isAccepting);

        token = strtok(NULL, delim);


        while(token != NULL){
          //Lamdas are in the loop this time since you can have multiple
          if (strcmp(token, "Lambda") == 0){
            int target;
            token = strtok(NULL, delim);
            sscanf(token,  "%d", &target);
            NFA_add_transition_all(nfa, state, target);

          }else if (strlen(token) <= 2){ //it's two because 1 char + null termination
            char symbol;
            sscanf(token,  "%c", &symbol);
            int target;
            token = strtok(NULL, delim);
            sscanf(token,  "%d", &target);
            NFA_add_transition(nfa, state, symbol, target);

          }else{  //string transition
            char dest[128];
            strcpy(dest, token);
            int target;
            token = strtok(NULL, delim);

            sscanf(token,  "%d", &target);

            NFA_add_transition_str(nfa, state, dest, target);

          }
//            printf("check transition| source: %d symbol: %c target: %d val %d\n", state, dest[0], target, IntSet_contains(nfa->states[state].transitions[dest[0]], target));

          token = strtok(NULL, delim);
        }

     }

   fclose(fp);

   return nfa;
 }


int NFA_tracing = 1;

IntSet* globalNextStates;
char globalNextSymbol;

void addToNextStates(int state){

  IntSet_union(globalNextStates, globalNFA->states[state].transitions[globalNextSymbol]);
}

bool NFA_Execute(NFA* nfa, char *input){
  globalNFA = nfa;
  for (int i = 0; i < strlen(input); i++){
    globalNextSymbol = input[i];
    globalNextStates = IntSet_new(); //seems like a mem leak but lets hope its not
    IntSet_iterate(nfa->current_states, &addToNextStates);
    if(IntSet_is_empty(globalNextStates))
      return false; //no transition

    if(NFA_tracing){
      printf("\ncurrent states: ");
      IntSet_iterate(nfa->current_states, &printState);
      printf("  |   next states: ");
      IntSet_iterate(globalNextStates, &printState);
    }

    IntSet_free(nfa->current_states);
    nfa->current_states = globalNextStates;
  }

  return NFA_is_accepting(nfa);
}



int main(int argc, char** argv){

  char* filename = argv[1];
  NFA* nfa = NFA_from_file(filename);
  NFA_print(nfa);
  bool result = NFA_Execute(nfa, argv[2]);

  if(result){
    printf("\naccept input \"%s\"\n", argv[2]);
  }else{
    printf("\nreject input \"%s\"\n", argv[2]);
  }

  return 1;
}
