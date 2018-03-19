#include "epidemic.h"

void initializeEpidemic(int N, double initialFraction, int maxMutations,int isSick[],int givenGerm[],bool wasSick[],bool infectionJournal[]){
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
      isSick[i] = wasSick[i] = 1; // is sick with mutation 1
      infectionJournal[i*maxMutations] = 1; // has been sick with mutation 1 -1 = 0
      numSick++;
    }else{
      isSick[i]= wasSick[i] = 0;
    }
  }
  std::cout << numSick << std::endl;
}
