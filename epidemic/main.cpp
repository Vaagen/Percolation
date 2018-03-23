#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <sys/stat.h>
#include "epidemic.h"

#include "../../matplotlib-cpp/matplotlibcpp.h"
#include <random>

bool fileExists(const std::string& filename){
  //https://stackoverflow.com/questions/4316442/stdofstream-check-if-file-exists-before-writing
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

double printTime(double start_time, double last_time){
  double stop_time =clock();
  double lap_time  = (stop_time-last_time)/double(CLOCKS_PER_SEC);
  double total_time= (stop_time-start_time)/double(CLOCKS_PER_SEC);
  std::cout << "    Time since last update(sec): " << lap_time << std::endl;
  std::cout << "    Total time(sec): " << total_time << std::endl;
  return stop_time;
}

int main(int argc, char *argv[]){
  // Seed random
  srand (time(NULL));
  // // Visual comparison of generators
  // plotHist();
  // return(0);
  // Starting timer.
  double start_time=clock();
  double last_time=start_time;

  // Getting N
  int N = 20;
  if(argc>1){
    std::istringstream ss(argv[1]);
    if (!(ss >> N)){
      std::cerr << "Invalid number " << argv[1] << '\n';
    }
  }
  // Getting output file name.
  std::string outputFileName = "output.txt";
  if (argc>2){
    outputFileName = argv[2];
  }
  std::cout << "Starting calculation for N=" << N << std::endl;

  // bool emptyFile = !fileExists(outputFileName);
  // std::ofstream outputFile(outputFileName,std::ios_base::app);
  // if(!outputFile.is_open()){
  //   std::cout << "Could not open file " << outputFileName << std::endl;
  //   return 0;
  // }
  // if(emptyFile){
  //   // outputFile << "N=" << N << ", repetitions=" << repetitions << ", col 0 is probability of tree, col 1 is avg(timesteps), col 2 is avg(burntTrees/totalTrees)." << std::endl;
  // }

  double initialFraction = 0.01;
  double infectionProb = 0.4;
  double relativeReinfectionProb = 1;
  double mutationProb = 0;


  int maxMutations = 10;
  int maxTime =30;

  int* isSick = new int[N*N];
  int* givenGerm = new int[N*N];
  double reinfectionProb = infectionProb*relativeReinfectionProb;
  bool* infectionJournal = new bool[N*N*maxMutations];
  // The journal is indexed with true/false at location i if person has had mutation i+1
  int numSick=0;
  // Initialize, set values to zero and infect 1% with mutation 1
  numSick = initializeEpidemic(N, initialFraction, maxMutations, isSick, givenGerm, infectionJournal);
  std::cout << "Finished initialization of epidemic."<< std::endl;
  last_time = printTime(start_time,last_time);

  // Propagate virus
  int t=0;
  double NN=N*N;
  while(numSick && t<maxTime){
    //std::cout << numSick/NN << std::endl;
    ++t;
    transmitPathogen(N, maxMutations, isSick, givenGerm, infectionJournal);
    numSick = infectPeople(N, infectionProb, reinfectionProb, mutationProb, maxMutations, isSick, givenGerm, infectionJournal);

    plotPeople(N, isSick);
     // To stop in between every time step.
     std::cout << "Press enter to continue." << std::endl;
     getchar();
  }


  std::cout << "Total time steps = " << t << '\n';
  double fractionSick=0;
  fractionSick = totalBeenSick(N,maxMutations,infectionJournal)/(NN);
  std::cout << "Fraction have been sick: " << fractionSick << '\n';

  // Clean up
  delete[] isSick;
  delete[] givenGerm;
  delete[] infectionJournal;

  std::cout << "Reached end of main. N=" << N << std::endl;
  printTime(start_time,last_time);
  return 0;
}
