#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){

  // create an empty structure (null)
  nlohmann::json j;

  // add a number that is stored as double (note the implicit conversion of j to an object)
  j["pi"] = 3.141;

  // add a Boolean that is stored as bool
  j["happy"] = true;

  // add a string that is stored as std::string
  j["name"] = "Niels";

  // add another null object by passing nullptr
  j["nothing"] = nullptr;

  // add an object inside the object
  j["answer"]["everything"] = 42;

  // add an array that is stored as std::vector (using an initializer list)
  j["list"] = { 1, 0, 2 };

  // add another object (using an initializer list of pairs)
  j["object"] = { {"currency", "USD"}, {"value", 42.99} };

  // write to file
  std::ofstream ofs("test.json");
  ofs << j.dump(4) << std::endl;
  ofs.close();

  // read and print
  std::ifstream ifs("test.json");
  nlohmann::json jj;
  ifs >> jj;
  std::cout<< jj.dump(4) <<std::endl;
  ifs.close();

  std::cout<< "Test Success!" <<std::endl;
  return 0;
}
