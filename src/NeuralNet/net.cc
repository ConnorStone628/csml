
#include "net.hh"

namespace CSML{

  unsigned int net::n_nets = 0;

  ////////////////////////////////////////////////////////////////////////////////
  net::net(){

    //Seed the random number generator
    srand(time(NULL));
  
    // Initialize the loss function with the standard loss
    this->loss_function = GM::StandardLossF;
    this->loss_derivative = GM::StandardLossDp;

    // Create the bias node
    this->bias_node = new node();
    *this->bias_node->input_signal = 1;

    // Unique ID for this net
    this->netid = net::n_nets;
    net::n_nets++;

  }

  ////////////////////////////////////////////////////////////////////////////////
  net::~net(){

    // Delete bias node
    delete this->bias_node;
    
    // Delete all the nodes
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      this->nodes[i].clear();
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::SetShape(const std::vector<unsigned int> _shape){

    // Clear the net
    this->nodes.clear();
    
    // Set the number of layers
    this->nodes.resize(_shape.size());

    // add the right number of nodes in each layer
    for (unsigned int i = 0; i < _shape.size(); ++i){
      for (unsigned int n = 0; n < _shape[i]; ++n){
	this->nodes[i].push_back(new node());
      }
    }
    
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  void net::AddSynapse(node* source, node* sink){

    // Tell the dendrite to add the given source
    sink->source_dendrite->Synapse(source);
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector<double> net::Output(){

    // Create a vector to hold the output values
    std::vector<double> output_signals(this->nodes[this->nodes.size() - 1].size());

    // collect the output signals from each output node
    for (unsigned int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
      output_signals[i] = *this->nodes[this->nodes.size() - 1][i]->output_signal;
    }

    return output_signals;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Propogate(const std::vector<double> input_values){

    // Checks that you have given the righ tnumber of inputs
    if (input_values.size() != this->nodes[0].size()){throw 1;}

    // Write the input value to each input node
    for (unsigned int i = 0; i < this->nodes[0].size(); ++i){
      *this->nodes[0][i]->input_signal += input_values[i];
    }

    // Fire the bias node
    this->bias_node->Fire();
    
    // Fire each node to send its signal through its synapses
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	// Send the signal to its output nodes
	this->nodes[i][n]->Fire();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Propogate(){

    // Fire the bias node
    this->bias_node->Fire();
    
    // Fire each node to send its signal through its synapses
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	// Send the signal to its output nodes
	this->nodes[i][n]->Fire();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Reset(const std::string scope){

    // reset input signals
    if (scope.find("i") != std::string::npos || scope.find("a") != std::string::npos){
      if (scope.find("b") != std::string::npos){*this->bias_node->input_signal = 0;}
      for (unsigned int i = 0; i < this->nodes.size(); ++i){    
	for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	  *this->nodes[i][n]->input_signal = 0;
	}
      }
    }

    // Reset output signals
    if (scope.find("o") != std::string::npos || scope.find("a") != std::string::npos){
      if (scope.find("b") != std::string::npos){*this->bias_node->output_signal = 0;}      
      for (unsigned int i = 0; i < this->nodes.size(); ++i){    
	for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	  *this->nodes[i][n]->output_signal = 0;
	}
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Connect(net* sourcenet){

    // Get the output layer of the source net
    std::vector<node*> sourcenodes = sourcenet->GetNodes(sourcenet->Shape().size() -1);

    // Fully connect the output layer with this net's input layer
    for (unsigned int i = 0; i < sourcenodes.size(); ++i){
      for (unsigned int o = 0; o < this->nodes[0].size(); ++o){
	this->AddSynapse(sourcenodes[i], this->nodes[0][o]);
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  std::vector<unsigned int> net::Shape(){

    // Create vector to hold the shape
    std::vector<unsigned int> S(this->nodes.size());

    // Add the number of nodes in each layer
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      S[i] = this->nodes[i].size();
    }
    
    return S;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::SetActivationFunction(double (*act_func)(double), double (*act_deriv)(double)){

    // Give each node the activation function
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->SetActivationFunction(act_func, act_deriv);
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, std::vector<double*>*, std::vector<double*>*)){

    // Give each node the kernel function
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->source_dendrite->SetKernel(kernel, derivative_a, derivative_w);
      }
    }

  }

  
}
