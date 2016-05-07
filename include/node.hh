#ifndef __NODE__
#define __NODE__

#include "GeneralMethods.hh"
#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>

namespace CSML{

  ////////////////////////////////////////////////////////////////////////////////
  class node{
  private:

    // Function that determines how the node collects the input signal
    double (*kernel_function)(std::vector<double*>*, std::vector<double*>*);
    double (*kernel_derivative_a)(unsigned int, double, std::vector<double*>*, std::vector<double*>*);
    double (*kernel_derivative_w)(unsigned int, double, std::vector<double*>*, std::vector<double*>*);

    // Unique id for each node
    static unsigned int n_nodes;
    unsigned int nodeid;
    
    // This maps the unique node_id's to their appropriate element of the weight vector
    std::map<unsigned int, unsigned int> nodeid_map;

  protected:

    // Represents the signal comming into the node, this is what is passed to the activation function
    double *activation;

    // Error at this node, scale factor for weight updates
    double delta, learning_rate;

  public:

    // Dendrites which recieve signal from this node
    std::vector<node*> source_nodes, sink_nodes;

    // Weights associated with each source node ("activations" is the output of those nodes)
    std::vector<double*> weights, activations;

    // Initialize node
    node();
  
    // Destructor
    virtual ~node();

    // Create a synapse connecting to the source node
    virtual void Synapse(node* source);

    // Applies the output signal of this node to all of its output nodes
    virtual void Fire(){if(!this->source_nodes.empty()){*this->activation = this->Kernel();}};

    // Collect error
    virtual void BackPropogate();

    // Apply the kernel function on the signal from the source nodes
    virtual double Kernel(){return (*this->kernel_function)(&this->activations, &this->weights);}
    
    // Compute the derivative of the kernel wrt one of the activations
    virtual double KernelDA(unsigned int id){return (*this->kernel_derivative_a)(this->nodeid_map[id], *this->activation, &this->activations, &this->weights);}

    // Compute the derivative of the kernel wrt one of the weights
    virtual double KernelDW(unsigned int id){return (*this->kernel_derivative_w)(this->nodeid_map[id], *this->activation, &this->activations, &this->weights);}

    // Get this node parameters
    virtual unsigned int GetId(){return this->nodeid;}
    virtual double* GetActivation(){return this->activation;}
    virtual double* GetDelta(){return this->delta;}

    // Update the weights for this node
    virtual void Update();

    // Set node parameters
    virtual void SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, double, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, double, std::vector<double*>*, std::vector<double*>*));
    virtual void SetDelta(double _delta){this->delta = _delta;}
    virtual void SetLearningRate(double _learning_rate){this->learning_rate = _learning_rate;}
    
  };

}
  
#endif
