
#ifndef __DATA__
#define __DATA__

#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <sstream>

namespace CSML{
  class data{
  private:

    // Determines the type of label ascociated with a feature vector: UK, NONE, SINGLE, MULTI
    // UK: unknown
    // NONE: unlabeled data
    // LABELED: vector of labels per feature vector
    std::string labeltype;

    // The size of each feature/label vector
    unsigned int featuresize, labelsize;

  protected:

    // Feature vectors/labels/hypothesis
    std::vector< std::vector<double> > X, y, h;

  public:

    // Base Constructor
    data();

    // Destructor
    ~data();

    // Add this point to the set
    virtual void AddElement(const std::vector<double> _X, const std::vector<double> _y);
    virtual void AddElement(const std::vector<double> _X);

    // Let the learner guess at the labels
    virtual void SetHypothesis(unsigned int index, const std::vector<double> _h);
    
    // Get the number of elements in the data set
    virtual unsigned int GetSize(){return this->X.size();}
    virtual unsigned int GetLabelSize(){return this->labelsize;}
    virtual unsigned int GetFeatureSize(){return this->featuresize;}
    
    // Get an element from the data set
    virtual std::vector<double>* GetElement(unsigned int index){return &this->X[index];}
    virtual std::vector<double>* GetLabel(unsigned int index){return &this->y[index];}
    
    // Get the type of label ascociated with each feature vector
    virtual std::string GetLabelType(){return this->labeltype;}
  
    // Save the data
    virtual std::string Save();

    // Re-load the data
    virtual void Load(const std::string values);

  };

}
#endif
