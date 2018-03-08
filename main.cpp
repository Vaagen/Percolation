#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "../matplotlib-cpp/matplotlibcpp.h"
#include <random>

namespace plt = matplotlibcpp;

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

void plantTrees(int N, bool isOccupied[], double sowProbability){

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  int cumsum = 0;
  for( int i=0; i<N*N; i++){
    if (dis(gen)<=sowProbability){
      isOccupied[i]=1;
      cumsum++;
    }else{
      isOccupied[i]=0;
    }
  }
  std::cout << cumsum << std::endl;
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
  for( int i=0; i<N*N; i++){
    isDead[i]=0;
    isSick[i]=0;
    wasSick[i]=0;
    if(i<N && isOccupied[i]){
      isSick[i] =1;
      wasSick[i]=1;
    }
  }
}

void propagateFire(int N, bool isOccupied[], double isSick[], bool wasSick[], bool isDead[]){
  for( int i=0; i<N*N; i++){
    if(isSick[i]){
      if(i<1 && isOccupied[i-1] && !isDead[i-1]){
        isSick[i-1]=1;
      }else if(i<N*(N-1) && isOccupied[i+N] && !isDead[i+N]){
        isSick[i+N]=1;
      }
    }else if(isOccupied[i] && !isDead[i]){ // If Sick it will be catched by previous if.
      if( (isSick[i+1] || isSick[i-N])){
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
    }
  }
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

int main(int argc, char *argv[]){
  // Starting timer.
  double start_time=clock();
  double last_time=start_time;

  // Checking for input.
  // if (argc <2){
  //   std::cout << "You have to supply input." << std::endl;
  //   return 0;
  // }
  // Getting integer input
  // int integerInput
  // std::istringstream ss(argv[1]);
  // if (!(ss >> integerInput)){
  //   std::cerr << "Invalid number " << argv[1] << '\n';
  // }
  // Getting output file name.
  std::string outputFile = "output.txt";
  if (argc>2){
    outputFile = argv[2];
  }

  int N = 100;
  bool isOccupied[N*N];
  double isSick[N*N];
  bool isDead[N*N];
  bool wasSick[N*N];

  plantTrees(N, isOccupied, 0.7);
  plotTrees(N, isOccupied, "g^");
  std::cout << "Press enter to continue." << std::endl;
  getchar();

  ignite(N, isOccupied, isSick, wasSick, isDead);
  plotFire(N, isSick);
  std::cout << "Press enter to continue." << std::endl;
  getchar();

  int totalSick = numSick(N,isSick);
  while(totalSick){
    propagateFire( N, isOccupied, isSick, wasSick, isDead);
    plt::clf();
    plotForest(N, isOccupied, isSick, isDead);
    totalSick = numSick(N,isSick);
  }

  last_time = printMessageTime("Reached end of main.", start_time, last_time);
  return 0;
}
