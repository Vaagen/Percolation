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
  // Starting timer.
  double start_time=clock();
  double last_time=start_time;

  // Getting N
  int N = 1000;
  if(argc>1){
    std::istringstream ss(argv[1]);
    if (!(ss >> N)){
      std::cerr << "Invalid number " << argv[1] << '\n';
    }
  }
  std::cout << "Starting calculation for N=" << N << std::endl;

  // bool emptyFile = !fileExists(outputFileName);
  // std::ofstream outputFile(outputFileName,std::ios_base::app);
  // if(!outputFile.is_open()){
  //   std::cout << "Could not open file " << outputFileName << std::endl;
  //   return 0;
  // }
  // if(emptyFile){
    // outputFile << "N=" << N << ", repetitions=" << repetitions <<
    // ", col 0 is probability of tree, col 1 is avg(timesteps), col 2 is avg(burntTrees/totalTrees)." <<
    //  std::endl;
  // }

  // Parameters
  double initialFraction = 0.01;
  double infectionProb = 1;
  double relativeReinfectionProb = 0.5;
  double mutationProb = 0;
  // Model parameters
  int maxMutations = 10000;
  int maxTime =1000000;


  // Opening output file
  std::string outputFileName = "output/inf("+std::to_string(infectionProb)+")re("+
                std::to_string(relativeReinfectionProb)+")mut("+std::to_string(mutationProb)+
                ")init(" +std::to_string(initialFraction)+")N("+
                std::to_string(N)+").txt";
  std::ofstream outputFile(outputFileName,std::ios_base::app);
  if(!outputFile.is_open()){
    std::cout << "Could not open file " << outputFileName << std::endl;
    return 0;
  }

  int* isSick = new int[N*N];
  int* givenGerm = new int[N*N];
  double reinfectionProb = infectionProb*relativeReinfectionProb;
  bool* infectionJournal = new bool[N*N*maxMutations];
  // The journal is indexed with true/false at location i if person has had mutation i+1
  int numSick=0;

  // Initialize random generators
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> real_dis(0.0, 1.0);
  std::uniform_int_distribution<> int_dis(1, maxMutations);

  // Initialize simulation
  numSick = initializeEpidemic(N, initialFraction, maxMutations, isSick, givenGerm,
                                  infectionJournal, gen, real_dis);
  std::cout << "Finished initialization of epidemic."<< std::endl;
  last_time = printTime(start_time,last_time);


  // Propagate virus
  int t=0;
  double NN=N*N;
  while(numSick && t<maxTime){
    //std::cout << numSick/NN << std::endl;
    ++t;
    transmitPathogen(N, maxMutations, isSick, givenGerm, infectionJournal);
    numSick = infectPeople(N, infectionProb, reinfectionProb, mutationProb,
      maxMutations, isSick, givenGerm, infectionJournal, gen, real_dis, int_dis);
    // Output with col 0 timestep, col 1 fraction sick, col 2 fraction have been sick
    outputFile << t << '\t' << numSick/(NN)
               << '\t' << totalBeenSick(N,maxMutations,infectionJournal)/(NN)
               << '\n';
    // Plot simulation
    // plotPeople(N, isSick);
    //  // To stop in between every time step.
    //  std::cout << "Press enter to continue." << std::endl;
    //  getchar();
  }

  outputFile.close();

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
