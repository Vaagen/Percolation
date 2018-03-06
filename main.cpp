#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>



double printTime(double start_time, double last_time){
  double stop_time =clock();
  double lap_time  = (stop_time-last_time)/double(CLOCKS_PER_SEC);
  double total_time= (stop_time-start_time)/double(CLOCKS_PER_SEC);
  std::cout << "    Time since last update(sec): " << lap_time << std::endl;
  std::cout << "    Total time(sec): " << total_time << std::endl;
  return stop_time;
}

double printMessage(std::string message, double start_time, double last_time){
  std::cout << message << std::endl;
  return printTime(start_time,last_time);
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



last_time = printMessage("Reached end of main.", start_time, last_time);
return 0;
}
