// epidemic.h
#ifndef __epidemic_included__
#define __epidemic_included__

#include <random>
#include <iostream>

void initializeEpidemic(int N, double initialFraction, int maxMutations,int isSick[],int givenGerm[],bool wasSick[],bool infectionJournal[]);
void transmitPathogen(int N,int maxMutations,int isSick[],int givenGerm[],bool wasSick[],bool infectionJournal[]);
void infectPeople(int N, double infectionProb, double reinfectionProb, double mutationProb, int maxMutations, int isSick[], int givenGerm[], bool wasSick[],bool infectionJournal[]);

#endif // #ifndef __epidemic_included__
