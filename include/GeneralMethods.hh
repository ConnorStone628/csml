#ifndef __GENERALMETHODS__
#define __GENERALMETHODS__

#include <sstream>
#include <string>
#include <math.h>

namespace CSML{
  namespace GM{
    
    // This is the signal propogation kernel used if unspecified
    inline double StandardKernelF(std::vector<double*>* a, std::vector<double*>* w){
      double total = 0;
      for (unsigned int i = 0; i < a->size(); ++i){total += (*(*a)[i])*(*(*w)[i]);}
      return 1.0/(1+exp(-total));
    }
    inline double StandardKernelDa(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      double z = log(k/(1-k));
      return *(*w)[index]/(2 + exp(z) + exp(-z));
    }
    inline double StandardKernelDw(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      double z = log(k/(1-k));
      return *(*a)[index]/(2 + exp(z) + exp(-z));
    }

    // This is the loss function the net will use if you don't specify
    inline double StandardLossF(double true_value, double predicted_value){return pow(predicted_value - true_value, 2);}
    inline double StandardLossDt(double true_value, double predicted_value){return 2*(true_value - predicted_value);}
    inline double StandardLossDp(double true_value, double predicted_value){return 2*(predicted_value - true_value);}
    
  }
}

#endif
