
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
    // SINGLE: single label per feature vector
    // MULTI: vector of labels per feature vector
    std::string labeltype;

    // The size of each label vector, if labeltype == MULTI, else -1
    unsigned int labelsize;
  
    // The size of each feature vector
    unsigned int featuresize;

  protected:

    // Feature vectors
    std::vector< std::vector<double> > X;

    // Feature labels:
    // 1D vector of labels
    std::vector<double> y1;
    // 2D vector of labels
    std::vector< std::vector<double> > y2;

    // Feature label hypothesis
    // 1D vector of label hypothesis
    std::vector<double> h1;
    // 2D vector of label hypothesis
    std::vector< std::vector<double> > h2;
    
  public:

    // Base Constructor
    data();

    // Destructor
    ~data();

    // Add this point to the set
    virtual void AddElement(const std::vector<double> _X, const std::vector<double> _y);
    virtual void AddElement(const std::vector<double> _X, const double _y);
    virtual void AddElement(const std::vector<double> _X);

    // Add these data points to the set
    virtual void AddElements(const std::vector< std::vector<double> > _X, const std::vector< std::vector<double> > _y);
    virtual void AddElements(const std::vector< std::vector<double> > _X, const std::vector<double> _y);
    virtual void AddElements(const std::vector< std::vector<double> > _X);

    // Let the learner guess at the labels
    virtual void SetHypothesis(const std::vector< std::vector<double> > _h);
    virtual void SetHypothesis(const std::vector<double> _h);
    
    // Process the data:
    // Normalize each feature vector
    virtual void NormalizeData(const std::string normtype = "L2");
    // Scale the feature vector
    virtual void ScaleData(const double scale);
    // Apply a user defined function of the data
    virtual void ProcessData(std::vector<double> (*f)(std::vector<double>));
    // Return the mean of the labels, if labeltype == MULTI you can specify the index of the label
    virtual double MeanLabel(const int l = -1);
    // Return the standard deviation of the labels, if labeltype == MULTI you can specify the index of the label
    virtual double STDLabel(const int l = -1);
    // Calculate squared loss on hypothesis
    virtual double TotalLoss();
    virtual std::vector<double> FeatureLoss();

    // Get access to the data
    virtual std::vector< std::vector<double> >* GetFeatures();
    virtual std::vector< std::vector<double> >* GetLabelsVV();
    virtual std::vector<double>* GetLabelsV();

    // Get the type of label ascociated with each feature vector
    virtual std::string GetLabelType();
  
    // Save the data
    virtual std::string Save();

    // Re-load the data
    virtual void Load(const std::string values);
  
  };

}
#endif
