#include "nlohmann/json.hpp"
#include "plotter.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
  if(argc==1){
    std::cout << "Error: No input!" << std::endl;
    return 1;
  }
  else if(argc!=2){
    std::cout << "Error: Too many arguments!" << std::endl;
    return 1;
  }

  std::cout << "Input json file: " << argv[1] << std::endl;

  std::ifstream ifs(argv[1]);
  nlohmann::json j;
  ifs >> j;

  // std::cout<< j.dump(4) <<std::endl;

  plotter p(j);
  p.Init();
  p.Exec();
  p.End();

  return 0;
};
