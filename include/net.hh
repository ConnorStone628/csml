#ifndef __NET__
#define __NET__

#include "node.hh"
#include "dendrite.hh"
#include "GeneralMethods.hh"

#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

namespace CSML{

  class net{
  private:
    
    // Unique id for each net
    static unsigned int n_nets;
    unsigned int netid;
    
  protected:

    // Vector of layers of nodes
    std::vector< std::vector<node*> > nodes;

    // Bias node for the net
    node* bias_node;
    
    // Function to calculate loss
    double (*loss_function)(double, double);
    double (*loss_derivative)(double, double);

    // Instert a passive node into the net
    virtual void SetShape(const std::vector<unsigned int> _shape);

    // Insert a new synapse into the net
    virtual void AddSynapse(node* source_node, node* sink_node);

    // Propogate an input signal through the net
    virtual void Propogate(const std::vector<double> input_values);
    virtual void Propogate();

    // Collect the output from the net
    virtual std::vector<double> Output();

    // Reset values on the net
    // i: input to nodes
    // o: output to nodes
    // b: do this for the bias node as well
    // a: "all"
    virtual void Reset(const std::string scope = "i");

    // Connects the input layer of this net to the output layer of the sourcenet
    virtual void Connect(net* sourcenet);
    
    // Sets the activation function for every node
    virtual void SetActivationFunction(double (*act_func)(double), double (*act_deriv)(double));

    // Sets the kernel funciton for every dendrite
    virtual void SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, std::vector<double*>*, std::vector<double*>*));
    
  public:

    // Default constructor
    net();

    // Destructor
    ~net();

    // Returns the shape of the net
    virtual std::vector<unsigned int> Shape();

    // Returns a vector of nodes at the selected layer
    virtual std::vector<node*> GetNodes(const unsigned int layer){return this->nodes[layer];}

    // Returns the id of this net
    unsigned int GetId(){return this->netid;}

  };

}
  
#endif
