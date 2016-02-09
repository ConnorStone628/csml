
#ifndef __LEARNER__
#define __LEARNER__

#include <vector>
#include <string>
#include <iostream>

namespace CSML{

  class data;

  class learner{
  
  public:

    // Train algorithm using this data
    virtual void Learn(data* _X) =0;

    // Use this learner to predict the class for each feature vector
    virtual void Predict(data* _X) =0;
  
    // Set a parameter for the learner
    virtual void SetParam(std::string param, double value) =0;
    virtual void SetParam(std::string param, int value) =0;
    virtual void SetParam(std::string param, std::string value) =0;
    virtual void SetParam(std::string param, std::vector<double> value) =0;
    virtual void SetParam(std::string param, std::vector< std::vector<double> > value) =0;

    // Get a parameter from the learner
    virtual double GetParamD(std::string param) =0;
    virtual int GetParamI(std::string param) =0;
    virtual std::string GetParamS(std::string param) =0;
    virtual std::vector<double> GetParamV(std::string param) =0;
    virtual std::vector< std::vector<double> > GetParamVV(std::string param) =0;

    // Save the current state of the learner
    virtual std::string Save() =0;

    // Load a saved state string
    virtual void Load(std::string state) =0;
  
  };

}
  
#endif
