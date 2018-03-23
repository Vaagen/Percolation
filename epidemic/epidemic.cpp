#include "epidemic.h"

int initializeEpidemic(int N, double initialFraction, int maxMutations,int isSick[],int givenGerm[],bool infectionJournal[], std::mt19937 gen, std::uniform_real_distribution<> real_dis){
  // std::random_device rd;
  // std::mt19937 gen(rd());
  // std::uniform_real_distribution<> dis(0.0, 1.0);
  // // Use with dis(gen)
  int numSick = 0;
  for(int i=0; i<N*N; ++i){
    givenGerm[i]=0;
    for(int j=0; j<maxMutations; ++j){
      infectionJournal[i*maxMutations + j]=0;
    }

    if (real_dis(gen)<initialFraction){
      isSick[i] = 1; // is sick with mutation 1
      infectionJournal[i*maxMutations] = 1; // has been sick with mutation 1 -1 = 0
      numSick++;
    }else{
      isSick[i] = 0;
    }
  }
  return numSick;
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

int infectPeople(int N, double infectionProb, double reinfectionProb, double mutationProb, int maxMutations, int isSick[], int givenGerm[],bool infectionJournal[], std::mt19937 gen, std::uniform_real_distribution<> real_dis, std::uniform_int_distribution<> int_dis){
  int numSick=0;
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
          numSick++;
        }else{ // does not get sick
          isSick[i] = 0;
        }
      }else{ // not had strain before
        if (real_dis(gen)<infectionProb){ // gets sick
          isSick[i] = givenGerm[i];
          infectionJournal[i*maxMutations + givenGerm[i]-1] = 1; // aquires immunity
          numSick++;
        }else{ // does not get sick
          isSick[i] = 0;
        }
      }
    }else{ // not aquired pathogen, time to get healthy if sick (or stay healthy)
      isSick[i]=0;
    }
    givenGerm[i]=0;
  }
  return numSick;
}

int totalBeenSick(int N, int maxMutations, bool infectionJournal[]){
  bool thisGuyWasSick =0;
  int mutationNum = 1; // note: shifted in journal
  int totalSick=0;
  for(int i=0;i<N*N; ++i){
    while(!thisGuyWasSick && (mutationNum <= maxMutations)){
      thisGuyWasSick= infectionJournal[i*maxMutations + mutationNum-1];
      mutationNum++;
    }
    if(thisGuyWasSick){
      totalSick++;
    }
    thisGuyWasSick=0;
    mutationNum=1;
  }
  return totalSick;
}

void plotPeople(int N, int isSick[]){
  int numSick=0;
  for( int i=0; i<N*N; i++){if(isSick[i]){numSick++;}}
  std::vector<double> everyone_x(N*N), everyone_y(N*N);
  std::vector<double> sick_x(numSick), sick_y(numSick);
  int j=0;
  int sickAdded=0;
  for( int i=0; i<N*N; i++){
    if(isSick[i]){
      sick_x.at(sickAdded)=i%N;
      sick_y.at(sickAdded)=j;
      sickAdded++;
    }
    everyone_x.at(i)=i%N;
    everyone_y.at(i)=j;
    if(i%N==(N-1)){
      j+=1;
    }
  }
  matplotlibcpp::plot(everyone_x,everyone_y, "b.");
  matplotlibcpp::plot(sick_x,sick_y,"ro");

  double axis_lim_buffer = N/10;
  matplotlibcpp::ylim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  matplotlibcpp::xlim(-axis_lim_buffer, (N-1)+axis_lim_buffer);
  matplotlibcpp::draw();
  matplotlibcpp::pause(0.001);
}

void plotHist(){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> real_dis(0.0, 1.0);
  std::vector<double> X(101), Y1(101), Y2(101);
  for(int i=0;i<=100;++i){
    X.at(i)=i;
  }
  double randomNum=0;
  int index=0;
  for(int i=0;i<100000;++i){
    randomNum = ((double) rand() / (RAND_MAX));
    index = round(randomNum*100);
    Y1.at(index)++;
    randomNum = real_dis(gen);
    index = round(randomNum*100);
    Y2.at(index)++;
  }
  matplotlibcpp::plot(X,Y1,"r*");
  matplotlibcpp::plot(X,Y2,"b*");
  matplotlibcpp::draw();
  matplotlibcpp::pause(0.001);
  std::cout << "Press enter to continue." << std::endl;
  getchar();

  // Now to see sequence/time dependancy
  int length=100;
  std::vector<double> X_t(length), Y1_t(length), Y2_t(length);
  double randNUM=0;
  double twistNUM=0;
  for(int t=0;t<length;++t){
    randNUM = ((double) rand() / (RAND_MAX));
    twistNUM = real_dis(gen);
    X_t.at(t)=t;
    Y1_t.at(t)=randNUM;
    Y2_t.at(t)=twistNUM;
  }
  matplotlibcpp::clf();
  matplotlibcpp::plot(X_t,Y1_t,"r*");
  matplotlibcpp::plot(X_t,Y2_t,"b*");
  matplotlibcpp::show();
  matplotlibcpp::pause(0.001);
  std::cout << "Press enter to continue." << std::endl;
  getchar();
}
