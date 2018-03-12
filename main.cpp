#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "../matplotlib-cpp/matplotlibcpp.h"
#include <random>
#include <fstream>
#include <sys/stat.h>


namespace plt = matplotlibcpp;

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

int plantTrees(std::uniform_real_distribution<>& dis, std::mt19937& gen, double sowProbability, int N, bool isOccupied[], double isSick[],bool wasSick[],bool isDead[]){

  int numTrees = 0;
  for( int i=0; i<N*N; i++){
    if (dis(gen)<sowProbability){
      isOccupied[i]=1;
      numTrees++;
    }else{
      isOccupied[i]=0;
    }
    isSick[i]=wasSick[i]=isDead[i]=0; // Reset arrays when planting new forest.
  }
  return numTrees;
}

void plotTrees(int N, bool isOccupied[], const char marker[]){
  int numTrees=0;
  for( int i=0; i<N*N; i++){if(isOccupied[i]){numTrees++;}}
  std::vector<double> object_x(numTrees), object_y(numTrees);
  int j=0;
  int treesPlanted=0;
  for( int i=0; i<N*N; i++){
    if(isOccupied[i]){
      object_x.at(treesPlanted) = i%N;
      object_y.at(treesPlanted) = j;
      treesPlanted++;
    }
    if(i%N==(N-1)){
      j+=1;
    }
  }
  plt::plot(object_x,object_y,marker);
  double axis_lim_buffer = N/10;
  plt::ylim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  plt::xlim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  plt::draw();
  plt::pause(0.001);
}

void plotFire(int N, double isSick[]){
  int numBurningTrees=0;
  for( int i=0; i<N*N; i++){if(isSick[i]){numBurningTrees++;}}
  std::vector<double> object_x(numBurningTrees), object_y(numBurningTrees);
  int j=0;
  int treesBurning=0;
  for( int i=0; i<N*N; i++){
    if(isSick[i]){
      object_x.at(treesBurning) = i%N;
      object_y.at(treesBurning) = j;
      treesBurning++;
    }
    if(i%N==(N-1)){
      j+=1;
    }
  }
  plt::plot(object_x,object_y,"rv");
  double axis_lim_buffer = N/10;
  plt::ylim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  plt::xlim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  plt::draw();
  plt::pause(0.001);
}

void plotForest(int N, bool isOccupied[], double isSick[], bool isDead[]){
  int numTrees=0;
  for( int i=0; i<N*N; i++){if(isOccupied[i] && !isDead[i]){numTrees++;}}
  int numDeadTrees=0;
  for( int i=0; i<N*N; i++){if(isDead[i]){numDeadTrees++;}}
  std::vector<double> greenTree_x(numTrees), greenTree_y(numTrees);
  std::vector<double> deadTree_x(numDeadTrees), deadTree_y(numDeadTrees);
  int j=0;
  int greenTreesPlanted=0;
  int deadTreesPlanted=0;
  for( int i=0; i<N*N; i++){
    if(isOccupied[i]){
      if(!isDead[i]){
        greenTree_x.at(greenTreesPlanted) = i%N;
        greenTree_y.at(greenTreesPlanted) = j;
        greenTreesPlanted++;
      }else{
        deadTree_x.at(deadTreesPlanted) = i%N;
        deadTree_y.at(deadTreesPlanted) = j;
        deadTreesPlanted++;
      }
    }
    if(i%N==(N-1)){
      j+=1;
    }
  }
  plt::plot(greenTree_x,greenTree_y, "g^");
  plt::plot(deadTree_x,deadTree_y,"k^");

  int numBurningTrees=0;
  for( int i=0; i<N*N; i++){if(isSick[i]){numBurningTrees++;}}
  std::vector<double> object_x(numBurningTrees), object_y(numBurningTrees);
  j=0;
  int treesBurning=0;
  for( int i=0; i<N*N; i++){
    if(isSick[i]){
      object_x.at(treesBurning) = i%N;
      object_y.at(treesBurning) = j;
      treesBurning++;
    }
    if(i%N==(N-1)){
      j+=1;
    }
  }
  plt::plot(object_x,object_y,"rv");

  double axis_lim_buffer = N/10;
  plt::ylim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  plt::xlim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  plt::draw();
  plt::pause(0.001);
}

void ignite(int N, bool isOccupied[], double isSick[], bool wasSick[], bool isDead[]){
  for( int i=0; i<N; i++){
    if(isOccupied[i]){
      isSick[i] =1;
      wasSick[i]=1;
    }
  }
}

bool propagateFire(int N, bool isOccupied[], double isSick[], bool wasSick[], bool isDead[]){
  int NNm1 = N*(N-1);
  bool burning=false; // Set to false if no more trees ignited.
  for( int i=0; i<N*N; i++){
    if(isSick[i]){
      if( isOccupied[i-1] && !isDead[i-1]){
        isSick[i-1]=1;
      }
      if(i<NNm1 && isOccupied[i+N] && !isDead[i+N]){
        isSick[i+N]=1;
      }
    }else if(isOccupied[i] && !isDead[i]){ // If Sick it will be catched by previous if.
      if( (isSick[i-1] || (isSick[i+N]&&i<NNm1))){
        isSick[i]=1;
      }
    }
  }
  for( int i=0; i<N*N; i++){
    if(wasSick[i]){
      isDead[i] =1;
      isSick[i] =0;
      wasSick[i]=0;
    }else if(isSick[i]){ // Will set newly ignited trees to wasSick
      wasSick[i]=1;
      burning=true;
    }
  }
  return burning;
}

void timeStep(int N, bool isOccupied[], double isSick[], bool wasSick[], bool isDead[]){
  propagateFire( N, isOccupied, isSick, wasSick, isDead);
  plt::clf();
  plotTrees(N, isOccupied, "g^");
  plotTrees(N, isDead, "k^");
  plotFire(N, isSick);
  plt::pause(0.1);
}

int numSick(int N, double isSick[]){
  int totalSick=0;
  for( int i=0;i<N*N;i++){
    totalSick+= isSick[i]!=0;
  }
  return totalSick;
}

int numDead(int N, bool isDead[]){
  int totalDead=0;
  for( int i=0;i<N*N;i++){
    totalDead+= isDead[i];
  }
  return totalDead;
}

void monteCarlo(double result[2], double p, int repetitions, int N, bool isOccupied[], double isSick[], bool wasSick[], bool isDead[] ){
  double densOfBurntTrees =0;
  int totalSick =0;
  double numTrees=0;
  double numDeadTrees=0;
  double avgTimeSteps=0;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  for( int rep=0; rep<repetitions; rep++){
      numTrees = plantTrees( dis, gen, p, N, isOccupied, isSick, wasSick, isDead);
      if(numTrees){
        ignite(N, isOccupied, isSick, wasSick, isDead);
        bool burning=true;
        while(burning){
          burning = propagateFire( N, isOccupied, isSick, wasSick, isDead);
          avgTimeSteps++;
        }
        numDeadTrees = numDead(N, isDead);
        densOfBurntTrees+=numDeadTrees/numTrees;
      }
  }
  result[0]=avgTimeSteps/repetitions;
  result[1]=densOfBurntTrees/repetitions;
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
  // Getting number of repetitions
  int repetitions = N*N*2;
  if(argc>3){
    std::istringstream ss(argv[3]);
    if (!(ss >> repetitions)){
      std::cerr << "Invalid number " << argv[3] << '\n';
    }
  }

  std::cout << "Starting calculation for N=" << N << ", using " << repetitions << " repetitions." << std::endl;

  bool isOccupied[N*N+1];
  double isSick[N*N+1];
  bool isDead[N*N+1];
  bool wasSick[N*N+1];
  // Extra element to make loop more simple.
  isOccupied[N*N]=isSick[N*N]=wasSick[N*N]=isDead[N*N]=0;

  bool emptyFile = !fileExists(outputFileName);
  std::ofstream outputFile(outputFileName,std::ios_base::app);
  if(!outputFile.is_open()){
    std::cout << "Could not open file. N=" << N << std::endl;
    return 0;
  }
  if(emptyFile){
    outputFile << "N=" << N << ", repetitions=" << repetitions << ", col 0 is probability of tree, col 1 is avg(timesteps), col 2 is avg(burntTrees/totalTrees)." << std::endl;
  }

  // Run montecarlo for each probability.
  double result[100][2];
  int i=0;
  for(double p=0.01; p<1; p+=0.01){
    monteCarlo(result[i], p, repetitions, N, isOccupied, isSick, wasSick, isDead);
    outputFile << p << '\t' << result[i][0] << '\t' << result[i][1] << std::endl;
    i++;
  }
  outputFile.close();

  // To see fire propagation
  // std::random_device rd;
  // std::mt19937 gen(rd());
  // std::uniform_real_distribution<> dis(0.0, 1.0);
  // int numTrees = plantTrees( dis, gen, 0.6, N, isOccupied, isSick, wasSick, isDead);
  // std::cout << numTrees << std::endl;
  // std::cout << N*N << std::endl;
  // //plotForest(N, isOccupied, isSick, isDead);
  // //std::cout << "Press enter to continue." << std::endl;
  // //getchar();
  // ignite(N, isOccupied, isSick, wasSick, isDead);
  // //plotForest(N, isOccupied, isSick, isDead);
  // //std::cout << "Press enter to continue." << std::endl;
  // //getchar();
  // bool burning=true;
  // while(burning){
  //   burning = propagateFire( N, isOccupied, isSick, wasSick, isDead);
  //   plt::clf();
  //   plotForest(N, isOccupied, isSick, isDead);
  //   // To stop in between every time step.
  //   std::cout << "Press enter to continue." << std::endl;
  //   getchar();
  // }



  std::cout << "Reached end of main. N=" << N << std::endl;
  printTime(start_time,last_time);
  return 0;
}
