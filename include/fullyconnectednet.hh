#ifndef __FULLYCONNECTEDNET__
#define __FULLYCONNECTEDNET__

#include <sstream>
#include "net.hh"
#include "learner.hh"
#include "data.hh"

namespace CSML{

  class fullyconnectednet : protected net, public learner {

  protected:

    // Scaling parameter for weight updates
    double learning_rate;

    // Number of times to cycle through the data
    unsigned int itterations;
    
    // Calculate error along the top row
    void TopErrors(const std::vector<double> true_values);

    // Back-Propogate errors through the net
    void BackPropogateErrors();
  
  public:

    // Constructor
    fullyconnectednet(const std::vector<unsigned int> nodes_per_layer);

    // Destructor
    ~fullyconnectednet();

    // From "learner.hh"
    // Fit the data
    void Learn(data* _X);

    // Predict from data
    void Predict(data* _X);
    
    void SetParam(const std::string param, const double value);

    void SetParam(const std::string param, const int value);
    
  };

}
#endif


