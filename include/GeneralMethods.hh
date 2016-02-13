#ifndef __GENERALMETHODS__
#define __GENERALMETHODS__

#include <sstream>
#include <string>
#include <math.h>

namespace CSML{
  namespace GM{
    
    // This is the signal propogation kernel used if unspecified
    inline double StandardSignalKernelF(std::vector<double*>* a, std::vector<double*>* w){
      double total = 0;
      for (unsigned int i = 0; i < a->size(); ++i){total += (*(*a)[i])*(*(*w)[i]);}
      return total;
    }
    inline double StandardSignalKernelDa(unsigned int index, std::vector<double*>* a, std::vector<double*>* w){return *(*w)[index];}
    inline double StandardSignalKernelDw(unsigned int index, std::vector<double*>* a, std::vector<double*>* w){return *(*a)[index];}

    // This is the loss function the net will use if you don't specify
    inline double StandardLossF(double true_value, double predicted_value){return pow(predicted_value - true_value, 2);}
    inline double StandardLossDt(double true_value, double predicted_value){return 2*(true_value - predicted_value);}
    inline double StandardLossDp(double true_value, double predicted_value){return 2*(predicted_value - true_value);}
    
    // This is the activation function the net will use if you don't specify
    inline double StandardActivationF(double z){return 1.0/(1+exp(-1*z));}
    inline double StandardActivationD(double z){return exp(-1*z)/pow(1+exp(-1*z),2);}
    
  }
}

#endif
