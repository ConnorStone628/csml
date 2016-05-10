#ifndef __GENERALMETHODS__
#define __GENERALMETHODS__

#include <sstream>
#include <string>
#include <math.h>

namespace CSML{
  namespace GM{
    
    // This is the signal propogation kernel used if unspecified
    // Takes the inner product of activations and weights then applies the logistic function
    inline double LogisticKernelF(std::vector<double*>* a, std::vector<double*>* w){
      double total = 0;
      for (unsigned int i = 0; i < a->size(); ++i){total += (*(*a)[i])*(*(*w)[i]);}
      return 1.0/(1+exp(-total));
    }
    inline double LogisticKernelDa(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      return *(*w)[index]*k*(1-K);
    }
    inline double LogisticKernelDw(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      return *(*a)[index]*k*(1-K);
    }

    // Takes the inner product of the activations and weights then applies the ramp function
    inline double ReLUKernelF(std::vector<double*>* a, std::vector<double*>* w){
      double total = 0;
      for (unsigned int i = 0; i < a->size(); ++i){total += (*(*a)[i])*(*(*w)[i]);}
      if (total <= 0){
	return 0;
      }else{
	return total;
      }
    }
    inline double ReLUKernelDa(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      if (k <= 0){
	return 0;
      }else{
	return *(*w)[index];
      }
    }
    inline double ReLUKernelDw(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      if (k <= 0){
	return 0;
      }else{
	return *(*a)[index];
      }
    }

    // Takes the inner product of the activations and weights then applies the hyperbolic tangent function
    inline double HypTanKernelF(std::vector<double*>* a, std::vector<double*>* w){
      double total = 0;
      for (unsigned int i = 0; i < a->size(); ++i){total += (*(*a)[i])*(*(*w)[i]);}
      return (exp(total) - 1)/(exp(total) + 1);
    }
    inline double HypTanKernelDa(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      double z = log((1+k)/(1-k));
      return *(*w)[index]*2/(2 + exp(z) + exp(-z));
    }
    inline double HypTanKernelDw(unsigned int index, double k, std::vector<double*>* a, std::vector<double*>* w){
      double z = log((1+k)/(1-k));
      return *(*a)[index]*2/(2 + exp(z) + exp(-z));
    }

    
    // This is the loss function the net will use if you don't specify
    inline double StandardLossF(double true_value, double predicted_value){return pow(predicted_value - true_value, 2);}
    inline double StandardLossDt(double true_value, double predicted_value){return 2*(true_value - predicted_value);}
    inline double StandardLossDp(double true_value, double predicted_value){return 2*(predicted_value - true_value);}
    
  }
}

#endif
