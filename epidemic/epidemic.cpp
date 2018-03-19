#include "epidemic.h"

void initializeEpidemic(int N, double initialFraction, int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[]){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  int numSick = 0;
  for(int i=0; i<N*N; ++i){
    givenGerm[i]=0;
    for(int j=0; j<maxMutations; ++j){
      infectionJournal[i*maxMutations + j]=0;
    }

    if (dis(gen)<initialFraction){
      isSick[i] = 1; // is sick with mutation 1
      infectionJournal[i*maxMutations] = 1; // has been sick with mutation 1 -1 = 0
      numSick++;
    }else{
      isSick[i] = 0;
    }
  }
  std::cout << numSick << std::endl;
}

void transmitPathogen(int N,int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[]){
  // give germ to neighbors
  //first element
  if(isSick[0]){ //give neighbors pathogen
    givenGerm[N*N-1] = givenGerm[N*(N-1)] = givenGerm[1] = givenGerm[N] = isSick[0];
  }
  //first line (except first element)
  for(int i=1;i<N; ++i){
    if(isSick[i]){
      givenGerm[i-1] = givenGerm[N*(N-1)+i] = givenGerm[i+1] = givenGerm[N+i] = isSick[i];
    }
  }
  //middle lines
  for(int i=N;i<N*(N-1);++i){
    if(isSick[i]){
      givenGerm[i-1] = givenGerm[i-N] = givenGerm[i+1] = givenGerm[i+N] = isSick[i];
    }
  }
  //last line (except last element)
  for(int i=N*(N-1);i<(N*N-1);++i){
    if(isSick[i]){
      givenGerm[i-1] = givenGerm[i-N] = givenGerm[i+1] = givenGerm[i%N] = isSick[i];
    }
  }
  //last element
  if(isSick[N*N-1]){
    givenGerm[N*N-2] = givenGerm[N*(N-1)-1] = givenGerm[0] = givenGerm[N-1] = isSick[N*N-1];
  }
}

void infectPeople(int N, double infectionProb, double reinfectionProb, double mutationProb, int maxMutations, int isSick[], int givenGerm[],bool infectionJournal[]){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> real_dis(0.0, 1.0);
  std::uniform_int_distribution<> int_dis(1, maxMutations);

  for(int i=0; i<N*N; ++i){
    if(givenGerm[i]){ // If aquired pathogen
      // Mutate strain with probability mutationProb
      if (real_dis(gen)<mutationProb){
        givenGerm[i] = int_dis(gen);
      }
      // Infect with correct probability
      if(infectionJournal[i*maxMutations + givenGerm[i]-1]){// if had strain before
        if (real_dis(gen)<reinfectionProb){ // gets sick
          isSick[i] = givenGerm[i];
          givenGerm[i] = 0;
        }else{ // does not get sick
          isSick[i] = 0;
          givenGerm[i] = 0;
        }
      }else{ // not had strain before
        if (real_dis(gen)<infectionProb){ // gets sick
          isSick[i] = givenGerm[i];
          givenGerm[i] = 0;
          infectionJournal[i*maxMutations + givenGerm[i]-1] = 1; // aquires immunity
        }else{ // does not get sick
          isSick[i] = 0;
          givenGerm[i] = 0;
        }
      }
    }else{ // not aquired pathogen, time to get healthy if sick (or stay healthy)
      isSick[i]=0;
    }
  }
}
