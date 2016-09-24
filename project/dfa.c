#include "headers.h"
#include "dfa.h"



/*This method inputs a DFA from a text file*/
DFA *  DFA_from_file(char* filename){
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

   DFA* dfa = DFA_new(numstates);

     while (TRUE) {

       if ( !fgets(buffer, sizeof(buffer), (FILE*)fp) ) {
          break;
        }

        buff = buffer;

        int state;
        int numLam;
        int isAccepting;
        char * token;

        //state number
        token = strtok(buff, delim);
        sscanf(token,  "%d", &state);

        //Accepting _
        char* accString = strtok(NULL, delim);
        token = strtok(NULL, delim);
        sscanf(token, "%d", &isAccepting);

        DFA_set_accepting(dfa, state, isAccepting);

        //Lambda _
        char* lamString = strtok(NULL, delim);
        token = strtok(NULL, delim);
        sscanf(token, "%d", &numLam);

        //Transitions
        lamString = strtok(NULL, delim);

        DFA_set_transition_all(dfa, state, numLam);

        token = strtok(NULL, delim);

        while(token != NULL){
          char symbol;
          sscanf(token,  "%c", &symbol);
          int target;
          token = strtok(NULL, delim);
          sscanf(token,  "%d", &target);

          DFA_set_transition(dfa, state, symbol, target);
          token = strtok(NULL, delim);
        }
     }

   fclose(fp);

   return dfa;
 }

DFA* DFA_new(int nstates){
  DFA* newdfa = (DFA*)malloc(sizeof(DFA));
  newdfa->nstates = nstates;
  newdfa->current_state = DFA_START_STATE;
  newdfa->states = (malloc(nstates * sizeof(DFA_State)));
   for(int i = 0; i < nstates; i++){
     for(int j = 0; j < DFA_NSYMBOLS; j++){
       newdfa->states[i].transitions[j] = DFA_NO_TRANSITION;
     }
   }
   return newdfa;
}

void DFA_free(DFA *dfa){
  free(dfa->states);
  free(dfa);
}


int DFA_get_size(DFA* dfa){
  return dfa->nstates;
}


int DFA_get_transition(DFA* dfa, int statenum, char symbol){
  return dfa->states[statenum].transitions[symbol];
}

void DFA_set_transition(DFA* dfa, int src, char symbol, int dst){
  dfa->states[src].transitions[symbol] = dst;
}

void DFA_set_transition_str(DFA* dfa, int src, char *str, int dst){
  for (int i = 0; i < strlen(str); i++){
    char symbol=str[i];
    dfa->states[src].transitions[symbol] = dst;
  }
}

void DFA_set_transition_all(DFA* dfa, int src, int dst){
  for (int i = 0; i < DFA_NSYMBOLS; i++){
    dfa->states[src].transitions[i] = dst;
  }
}


int DFA_get_current_state(DFA *dfa){
    return dfa->current_state;
}

void DFA_set_current_state(DFA *dfa, int statenum){
    dfa->current_state = statenum;
}

int DFA_get_accepting(DFA *dfa, int statenum){
    return dfa->states[statenum].is_accepting;
}

void DFA_set_accepting(DFA *dfa, int statenum, int value){
    dfa->states[statenum].is_accepting = value;
}

int DFA_is_accepting(DFA *dfa){
    return dfa->states[dfa->current_state].is_accepting;
}

void DFA_print(DFA *dfa){
  for (int i = 0; i < dfa->nstates; i++){
    printf("State %d(%d): ", i , DFA_get_accepting(dfa, i));
    for(int j = 0; j < DFA_NSYMBOLS; j++){
      if(dfa->states[i].transitions[j] != -1){
        printf("%c -> %d | ", (char)j, dfa->states[i].transitions[j]);
      }
    }
    printf("\n");
  }
}

int DFA_execute(DFA* dfa, char *input){
  char nextSymbol = input[0];
  int nextState;
  int i = 0;
  while(nextSymbol != '\0'){
    int currState = DFA_get_current_state(dfa);
    nextState = DFA_get_transition(dfa, currState, nextSymbol);
    printf("Current State: %d  Input Symbol %c Next State: %d\n", currState, nextSymbol, nextState);
    if(nextState == -1)
      return FALSE;
    DFA_set_current_state(dfa, nextState);
    nextSymbol = input[++i];
  }
  return DFA_is_accepting(dfa);
}
