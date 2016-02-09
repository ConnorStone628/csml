#include <vector>
#include "data.hh"
#include <iostream>

int main(){

  CSML::data X;

  std::vector< std::vector<double> > _X;

  _X.resize(10);

  for (int i = 0; i < _X.size(); ++i){
    for (int j = 0; j < 5; ++j){
      _X[i].push_back(2*j + i);
    }
  }

  X.AddElements(_X);

  std::cout << X.Save() << std::endl;
  
  return 0;
  
}
