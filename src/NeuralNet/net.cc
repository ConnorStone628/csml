
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
    *this->bias_node->GetActivation() = 1;

    // Unique ID for this net
    this->netid = net::n_nets;
    net::n_nets++;

  }

  ////////////////////////////////////////////////////////////////////////////////
  net::~net(){

    // Delete bias node
    delete this->bias_node;

    // Delete the nodes in the layers
    this->ClearNodes();
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::ClearNodes(){

    // Delete all the nodes
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      this->nodes[i].clear();
    }
    this->nodes.clear();

  }
  
  ////////////////////////////////////////////////////////////////////////////////
  void net::SetShape(const std::vector<unsigned int> _shape){

    this->ClearNodes();
    
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
  std::vector<double> net::Output(){

    // Create a vector to hold the output values
    std::vector<double> output_signals(this->nodes[this->nodes.size() - 1].size());

    // collect the output signals from each output node
    for (unsigned int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
      output_signals[i] = *this->nodes[this->nodes.size() - 1][i]->GetActivation();
    }

    return output_signals;
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Propogate(const std::vector<double> input_values){

    // Write the input value to each input node
    for (unsigned int i = 0; i < this->nodes[0].size(); ++i){
      *this->nodes[0][i]->GetActivation() += input_values[i];
    }

    this->Propogate();
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Propogate(){

    // Fire each node to send its signal through its synapses
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	// Send the signal to its output nodes
	this->nodes[i][n]->Fire();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::BackPropogate(std:vector<double> true_values){

    unsigned int last_layer = this->nodes.size()-1;
    
      for (unsigned int n = 0; n < this->nodes[last_layer].size(); ++n){
      this->nodes[last_layer][n]->delta = this->loss_derivative(true_values[n], *this->nodes[last_layer][n]->GetActivation());
    }
    
    for (unsigned int i = last_layer; i >= 0; --i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->BackPropogate();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Reset(){

    // Reset output signals
    for (unsigned int i = 0; i < this->nodes.size(); ++i){    
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	*this->nodes[i][n]->GetActivation() = 0;
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
  void net::SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, std::vector<double*>*, std::vector<double*>*)){

    // Give each node the kernel function
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->source_dendrite->SetKernel(kernel, derivative_a, derivative_w);
      }
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  double net::Loss(std::vector<double> true_values){

    double total = 0;
    unsigned int last_layer = this->nodes.size()-1;
    
    for (unsigned int n = 0; n < true_values.size(); ++n){
      total += this->loss_function(true_values[n], *this->nodes[last_layer][n]->GetActivation());
    }

    return total;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void net::Update(){

    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->Update();
      }
    }
  }
  
}
