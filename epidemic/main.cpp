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

double printMessageTime(std::string message, double start_time, double last_time){
  std::cout << message << std::endl;
  return printTime(start_time,last_time);
}


int main(int argc, char *argv[]){
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


  int* isSick = new int[N*N];
  int* givenGerm = new int[N*N];

  double initialFraction = 0.01;
  double infectionProb = 0.5;
  double relativeReinfectionProb = 0;
  double reinfectionProb = infectionProb*relativeReinfectionProb;
  double mutationProb = 0.1;
  int maxMutations = 1e3;
  bool* infectionJournal = new bool[N*N*maxMutations];
  // The journal is indexed with true/false at location i if person has had mutation i+1

  // Initialize, set values to zero and infect 1% with mutation 1
  initializeEpidemic(N, initialFraction, maxMutations, isSick, givenGerm, infectionJournal);

  // Propagate virus
  transmitPathogen(N, maxMutations, isSick, givenGerm, infectionJournal);
  infectPeople(N, infectionProb, reinfectionProb, mutationProb, maxMutations, isSick, givenGerm, infectionJournal);


  // Clean up
  delete[] isSick;
  delete[] givenGerm;
  delete[] infectionJournal;

  std::cout << "Reached end of main. N=" << N << std::endl;
  printTime(start_time,last_time);
  return 0;
}
