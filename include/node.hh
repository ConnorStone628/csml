#ifndef __NODE__
#define __NODE__

#include "dendrite.hh"
#include "GeneralMethods.hh"

#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>

namespace CSML{

  // Pre-declaration so pointers can be used
  class dendrite;

  ////////////////////////////////////////////////////////////////////////////////
  class node{
  private:

    // Function that determines how the node responds to the input signal
    double (*activation_function)(double);
    double (*activation_derivative)(double);

    // Unique id for each node
    static unsigned int n_nodes;
    unsigned int nodeid;
    
  public:

    // Represents the signal comming into the node, this is what is passed to the activation function
    double *input_signal, *output_signal;

    // Dendrite which provides signal to this node
    dendrite* source_dendrite;

    // Dendrites which recieve signal from this node
    std::vector<dendrite*> sink_dendrites;

    // Initialize node as a passive/input node
    node();
  
    // Destructor
    virtual ~node();

    // Applies the output signal of this node to all of its output nodes
    virtual void Fire();

    // Collect error from all sink dendrites
    virtual void BackPropogate();

    // Changes the node to be active
    virtual void SetActivationFunction(double (*act_func)(double), double (*act_deriv)(double));

    // Derivative of the activation function
    virtual double ActivationF(){return (*this->activation_function)(*this->input_signal);}
    virtual double ActivationD(){return (*this->activation_derivative)(*this->input_signal);}
    
    // Get this node's id
    virtual unsigned int GetId(){return this->nodeid;}
    
  };

}
  
#endif
