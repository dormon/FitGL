#include<LoadTextFile.h>
#include<fstream>
#include<string>

std::string loadFile(std::string const&fileName){
  std::ifstream f(fileName.c_str());
  if(!f.is_open()){
    std::cerr<<"file: "<<fileName<<" does not exist!"<<std::endl;
    return 0;
  }
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  f.close();
  return str;
}

