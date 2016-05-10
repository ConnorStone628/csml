
#include "node.hh"

namespace CSML{

  unsigned int node::n_nodes = 0;

  ////////////////////////////////////////////////////////////////////////////////
  node::node(){

    // Holds the result of activation on the input_signal
    this->activation = new double;
    *this->activation = 0;

    // Set the default kernel
    this->kernel_function = GM::LogisticKernelF;
    this->kernel_derivative_a = GM::LogisticKernelDa;
    this->kernel_derivative_w = GM::LogisticKernelDw;

    // Set the default learning rate
    this->learning_rate = 0.1;
    // zero the delta
    this->delta = 0;
    
    // Each node gets a unique id
    this->nodeid = this->n_nodes;
    this->n_nodes++;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  node::~node(){

    // Eliminate all newly created variables
    delete this->activation;

  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::Synapse(node* source){

    // Every node that provides for the signal
    this->source_nodes.push_back(source);
    // Weight vector for the kernel
    this->weights.push_back(new double);
    // Output from the source nodes, for the kernel
    this->activations.push_back(source->GetActivation());

    // Map the space in the vector to the unique id of the node
    this->nodeid_map[source->GetId()] = this->weights.size()-1;
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  void node::BackPropogate(){

    if (this->sink_nodes.empty()){return;}
    
    this->delta = 0;

    // Backpropogate error through the signal function
    for (unsigned int i = 0; i < this->sink_nodes.size(); ++i){
      this->delta += this->sink_nodes[i]->GetDelta()*this->sink_nodes[i]->KernelDA(this->nodeid);
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, double, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, double, std::vector<double*>*, std::vector<double*>*)){

    this->kernel_function = kernel;
    this->kernel_derivative_a = derivative_a;
    this->kernel_derivative_w = derivative_w;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::Update(){

    if (this->source_nodes.empty()){return;}

    // BackPropogate through the kernel to update each weight
    for (unsigned int i = 0; i < this->weights.size(); ++i){
      *this->weights[i] -= this->learning_rate*this->delta*this->KernelDW(i);
    }
    
  }
  
}
