// epidemic.h
#ifndef __epidemic_included__
#define __epidemic_included__

#include <random>
#include <iostream>

#include "../../matplotlib-cpp/matplotlibcpp.h"

int initializeEpidemic(int N, double initialFraction, int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[], std::mt19937 gen, std::uniform_real_distribution<> real_dis);
void transmitPathogen(int N,int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[]);
int infectPeople(int N, double infectionProb, double reinfectionProb, double mutationProb, int maxMutations, int isSick[], int givenGerm[],bool infectionJournal[], std::mt19937& gen, std::uniform_real_distribution<> real_dis, std::uniform_int_distribution<> int_dis);
int totalBeenSick(int N, int maxMutations, bool infectionJournal[]);
void plotPeople(int N, int isSick[]);
void plotHist();

#endif // #ifndef __epidemic_included__
