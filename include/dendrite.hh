#ifndef __DENDRITE__
#define __DENDRITE__

#include <vector>
#include <map>
#include "GeneralMethods.hh"
#include "node.hh"

namespace CSML{

  class node;

  class dendrite{
  private:

    // Function that determines how the node collects the input signal
    double (*kernel_function)(std::vector<double*>*, std::vector<double*>*);
    double (*kernel_derivative_a)(unsigned int, std::vector<double*>*, std::vector<double*>*);
    double (*kernel_derivative_w)(unsigned int, std::vector<double*>*, std::vector<double*>*);

    // scale factor for weight updates
    double learning_rate;

    // Node which recieves a signal from this dendrite
    node* sink_node;

    // Nodes which provide signal for this dendrite
    std::vector<node*> source_nodes;

    // Weights associated with each source node ("activations" is the output of those nodes)
    std::vector<double*> weights, activations;

    // This maps the unique node_id's to their appropriate element of the weight vector
    std::map<unsigned int, unsigned int> nodeid_map;
    
    // Error at the top of this dendrite
    double delta;

  public:

    // Constructor
    dendrite(node* sink);

    // Destructor
    virtual ~dendrite();

    // Create a synapse connecting to the source node
    virtual void Synapse(node* source);

    // Swaps the activation kernel for this dendrite
    virtual void SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, std::vector<double*>*, std::vector<double*>*));

    // Update the weights for this dendrites synapses
    virtual void Update();

    // Apply the kernel function on the signal from the source nodes
    virtual double Kernel(){return (*this->kernel_function)(&this->activations, &this->weights);}

    // Compute the derivative of the kernel wrt one of the activations
    virtual double KernelDA(unsigned int id){return (*this->kernel_derivative_a)(this->nodeid_map[id], &this->activations, &this->weights);}

    // Compute the derivative of the kernel wrt one of the weights
    virtual double KernelDW(unsigned int id){return (*this->kernel_derivative_w)(this->nodeid_map[id], &this->activations, &this->weights);}

    // Get the error at the top of this dendrite
    virtual double GetDelta(){return this->delta;}
    virtual void SetDelta(double _delta){this->delta = _delta;}
    
    // Set the learning rate for this dendrites backpropagation step
    virtual void SetLearningRate(double rate){this->learning_rate = rate;}
    
  };

}

#endif
