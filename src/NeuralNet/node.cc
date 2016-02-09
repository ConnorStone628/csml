
#include "node.hh"
#include <iostream>

namespace CSML{

  ////////////////////////////////////////////////////////////////////////////////
  node::node(const std::string node_name){

    this->name = node_name;
  
    this->input_signal = new double;
  
    // Input node has no activation, thus the output is the same as the input
    this->output_signal = input_signal;
    *this->input_signal = 0;

    this->activation_rate = new double;
    *this->activation_rate = 0;

    // Set the functions
    this->activation_function = NULL;
    this->activation_derivative = NULL;
    
    // Record that this is a passive node
    this->passive = true;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  node::node(const std::string node_name, double (*act_func)(double), double (*act_deriv)(double)){

    this->name = node_name;
  
    // Holds the total signal comming in from input synapses
    this->input_signal = new double;
    *this->input_signal = 0;

    // Holds the result of activation on the input_signal
    this->output_signal = new double;
    *this->output_signal = 0;

    // Derivative of output_signal at the current input_signal
    this->activation_rate = new double;
    *this->activation_rate = 0;

    // Set the functions
    this->activation_function = act_func;
    this->activation_derivative = act_deriv;

    // Record that this is an active node
    this->passive = false;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  node::~node(){

    // Eliminate all newly created variables
    delete this->activation_rate;
    delete this->input_signal;
    if (!this->passive){
      delete this->output_signal;
    }

    for (unsigned int i = 0; i < this->extra_params.size(); ++i){
      delete this->extra_params[i];
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::Activate(){
  
    if (this->passive) {return;}
    // Apply the activation function to the input_signal
    *this->output_signal = (*this->activation_function)(*this->input_signal);
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::Fire(){
  
    // Send the output signal through all output synapses
    for (unsigned int i = 0; i < this->sink_synapses.size(); ++i){
      *this->sink_synapses[i]->sink_node->input_signal += (*this->sink_synapses[i]->weight)*(*this->output_signal);
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::BackPropogateError(const double learning_rate){

    double delta = 0;

    if (this->passive){
      *this->activation_rate = 0;
      for (unsigned int so = 0; so < this->source_synapses.size(); ++so){
	*this->source_synapses[so]->weight_delta = 0;
      }
      return;
    }else{
      *this->activation_rate = (*this->activation_derivative)(*this->input_signal);
    }
    
    for (unsigned int so = 0; so < this->source_synapses.size(); ++so){
      delta = 0;
      for (unsigned int si = 0; si < this->sink_synapses.size(); ++si){
	// Add the error from the above layer of weights. this is the weight on each synapse, multiplied by its error/delta
	delta += (*this->sink_synapses[si]->weight_delta)*(*this->sink_synapses[si]->weight);
      }
      // Scale by the learning rate, the derivative of the activation function, and the activation that passed through the synapse
      *this->source_synapses[so]->weight_delta = delta*learning_rate*(*this->activation_rate)*(*this->source_synapses[so]->source_node->output_signal);
    }
    
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  void node::SetActive(const std::string node_name, double (*act_func)(double), double (*act_deriv)(double)){

    if (!this->passive){return;}

    this->passive = false;
    this->activation_function = act_func;
    this->activation_derivative = act_deriv;

    this->output_signal = new double;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::SetPassive(const std::string node_name){

    if (this->passive){return;}
    
    delete this->output_signal;
    this->output_signal = this->input_signal;
    this->passive = true;

    this->activation_function = NULL;
    this->activation_derivative = NULL;

  }
  
}
