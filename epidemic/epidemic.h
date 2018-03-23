// epidemic.h
#ifndef __epidemic_included__
#define __epidemic_included__

#include <random>
#include <iostream>

#include "../../matplotlib-cpp/matplotlibcpp.h"

int initializeEpidemic(int N, double initialFraction, int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[]);
void transmitPathogen(int N,int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[]);
int infectPeople(int N, double infectionProb, double reinfectionProb, double mutationProb, int maxMutations, int isSick[], int givenGerm[],bool infectionJournal[]);
int totalBeenSick(int N, int maxMutations, bool infectionJournal[]);
void plotPeople(int N, int isSick[]);
void plotHist();

#endif // #ifndef __epidemic_included__
