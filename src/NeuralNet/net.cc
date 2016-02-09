
#include "net.hh"

namespace CSML{

  ////////////////////////////////////////////////////////////////////////////////
  net::net(){

    //Seed the random number generator
    srand(time(NULL));
  
    // Initialize the loss function with the standard loss
    this->loss = StandardLoss;
    this->loss_derivative = StandardLossDerivative;

    // Initialize the activation function to standard
    this->activation_function = StandardActivationFunction;
    this->activation_derivative = StandardActivationDerivative;

    // Create the bias node
    this->bias_node = new node("BN");
    *this->bias_node->input_signal = 1;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  net::net(double (*act_func)(double), double (*act_deriv)(double)){

    //Seed the random number generator
    srand(time(NULL));
  
    // Initialize the loss function with the standard loss
    this->loss = StandardLoss;
    this->loss_derivative = StandardLossDerivative;

    // Initialize the activation function to standard
    this->activation_function = act_func;
    this->activation_derivative = act_deriv;
    
    // Create the bias node
    this->bias_node = new node("BN");
    *this->bias_node->input_signal = 1;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  net::~net(){

    // Delete bias node
    delete this->bias_node;
    
    // Delete all the nodes
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      this->nodes[i].clear();
    }

    // Delete all the synapses
    for (unsigned int i = 0; i < this->synapses.size(); ++i){
      for (unsigned int s = 0; s < this->synapses[i].size(); ++s){
	// The weights must be deleted separately
	delete this->synapses[i][s]->weight;
	delete this->synapses[i][s]->weight_delta;
      }
      this->synapses[i].clear();
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::SetShape(const std::vector<unsigned int> _shape){

    std::stringstream ss;
    
    this->nodes.resize(_shape.size());

    for (unsigned int i = 0; i < _shape.size(); ++i){
      for (unsigned int n = 0; n < _shape[i]; ++n){
	ss.str("");
	ss << "AL" << i << "N" << n;
	this->nodes[i].push_back(new node(ss.str(), this->activation_function, this->activation_derivative));
      }
    }
    
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  void net::AddSynapse(const unsigned int step, node* source, node* sink){

    // Ensure there are enough steps to include this synapse, add a step if necessary
    if (step + 1 >= this->synapses.size()){
      this->synapses.resize(step+1);
    }

    // Add the synapse to its step
    this->synapses[step].push_back(new synapse);

    // Initialize the synapse with its source node, sink node, and weight parameters
    this->synapses[step][this->synapses[step].size()-1]->source_node = source;
    this->synapses[step][this->synapses[step].size()-1]->sink_node = sink;
    this->synapses[step][this->synapses[step].size()-1]->weight = new double;
    this->synapses[step][this->synapses[step].size()-1]->weight_delta = new double;

    // Initialize a random weight, and a zero delta
    *this->synapses[step][this->synapses[step].size()-1]->weight = -1.0 + (rand() % 10000 + 0.5)/5000.0;
    *this->synapses[step][this->synapses[step].size()-1]->weight_delta = 0;

    // Inform the source and sink nodes of this synapse
    source->sink_synapses.push_back(this->synapses[step][this->synapses[step].size()-1]);
    sink->source_synapses.push_back(this->synapses[step][this->synapses[step].size()-1]);
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector<double> net::Output(){

    // Create a vector to hold the output values
    std::vector<double> output_signals(this->nodes[this->nodes.size() - 1].size());

    // collect the output signals from each output node
    for (unsigned int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
      output_signals[i] = *(this->nodes[this->nodes.size() - 1][i]->output_signal);
    }

    return output_signals;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Propogate(const std::vector<double> input_values){

    // Checks that you have given the righ tnumber of inputs
    if (input_values.size() != this->nodes[0].size()){throw 1;}

    // Write the input value to each input node
    for (unsigned int i = 1; i < this->nodes[0].size(); ++i){
      *(this->nodes[0][i]->input_signal) = input_values[i-1];
    }

    // Fire the bias node
    this->bias_node->Fire();
    
    // Fire each node to send its signal through its synapses
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	// Apply the activation function
	this->nodes[i][n]->Activate();
	// Send the signal to its output nodes
	this->nodes[i][n]->Fire();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Reset(const std::string scope){

    // reset input signals
    if (scope.find("i") != std::string::npos || scope.find("a") != std::string::npos){
      for (unsigned int i = 0; i < this->nodes.size(); ++i){    
	for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	  if (scope.find("p") != std::string::npos && this->nodes[i][n]->passive){continue;}
	  *(this->nodes[i][n]->input_signal) = 0;
	}
      }
    }

    // Reset output signals
    if (scope.find("o") != std::string::npos || scope.find("a") != std::string::npos){
      for (unsigned int i = 0; i < this->nodes.size(); ++i){    
	for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	  if (scope.find("p") != std::string::npos && this->nodes[i][n]->passive){continue;}
	  *(this->nodes[i][n]->output_signal) = 0;
	}
      }
    }

    // Reset weights
    if (scope.find("w") != std::string::npos || scope.find("a") != std::string::npos){
      for (unsigned int i = 0; i < this->synapses.size(); ++i){    
	for (unsigned int n = 0; n < this->synapses[i].size(); ++n){
	  *(this->synapses[i][n]->weight) = 0;
	  *(this->synapses[i][n]->weight_delta) = 0;
	}
      }      
    }
    
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  double net::StandardLoss(const double true_value, const double predicted_value){

    // Squared difference
    return pow(true_value - predicted_value, 2);
  
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  double net::StandardLossDerivative(const double true_value, const double predicted_value){

    // Just the derivative
    return 2*(predicted_value - true_value);
  
  }

  ////////////////////////////////////////////////////////////////////////////////  
  double net::StandardActivationFunction(const double z){

    // sigmoid
    return 1.0/(1+exp(-1*z));
    
  }

  ////////////////////////////////////////////////////////////////////////////////  
  double net::StandardActivationDerivative(const double z){

    // signmoid derivative
    return exp(-1*z)/pow(1+exp(-1*z),2);
    
  }  
  
}
