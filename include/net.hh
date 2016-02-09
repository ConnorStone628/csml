#ifndef __NET__
#define __NET__

#include "node.hh"
#include "synapse.hh"

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

    // This is the loss function the net will use if you don't specify
    static double StandardLoss(const double true_value, const double predicted_value);
    static double StandardLossDerivative(const double true_value, const double predicted_value);

    // This is the activation function the net will use if you don't specify
    static double StandardActivationFunction(const double z);
    static double StandardActivationDerivative(const double z);
        
  protected:

    // Vector of layers of nodes
    std::vector< std::vector<node*> > nodes;

    // Bias node for the net
    node* bias_node;
    
    // Vector of steps of synapses
    std::vector< std::vector<synapse*> > synapses;

    // Function to calculate loss
    double (*loss)(double, double);
    double (*loss_derivative)(double, double);

    // Function to calculate the derivative of the loss
    double (*activation_function)(double);
    double (*activation_derivative)(double);
    
    // Instert a passive node into the net
    virtual void SetShape(const std::vector<unsigned int> _shape);

    // Insert a new synapse into the net
    virtual void AddSynapse(const unsigned int step, node* source_node, node* sink_node);

    // Propogate an input signal through the net
    virtual void Propogate(const std::vector<double> input_values);

    // Collect the output from the net
    virtual std::vector<double> Output();

    // Reset values on the net
    // i: input to nodes
    // o: output to nodes
    // w: weights of synapses (and deltas)
    // a: "all"
    // p: skip passive nodes
    virtual void Reset(const std::string scope = "i");
    
  public:

    // Default constructor
    net();
    
    // Constructor with activation function and its derivative
    net(double (*act_func)(double), double (*act_deriv)(double));

    // Destructor
    ~net();

  };

}
  
#endif
