
#include "dendrite.hh"

namespace CSML{

  ////////////////////////////////////////////////////////////////////////////////
  dendrite::dendrite(node* sink){

    // Connect to the sink node
    this->sink_node = sink;

    // Set the default kernels
    this->kernel_function = GM::StandardSignalKernelF;
    this->kernel_derivative_a = GM::StandardSignalKernelDa;
    this->kernel_derivative_w = GM::StandardSignalKernelDw;

    // Set the default learning rate
    this->learning_rate = 1;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  dendrite::~dendrite(){
    this->weights.clear();
  }

  ////////////////////////////////////////////////////////////////////////////////
  void dendrite::Synapse(node* source){

    // Every node that provides for the signal
    this->source_nodes.push_back(source);
    // Weight vector for the kernel
    this->weights.push_back(new double);
    // Output from the source nodes, for the kernel
    this->activations.push_back(source->output_signal);

    // Connect the source node to this dendrite
    source->sink_dendrites.push_back(this);
    this->nodeid_map[source->GetId()] = this->weights.size()-1;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void dendrite::SetKernel(double (*kernel)(std::vector<double*>*, std::vector<double*>*), double (*derivative_a)(unsigned int, std::vector<double*>*, std::vector<double*>*), double (*derivative_w)(unsigned int, std::vector<double*>*, std::vector<double*>*)){
    this->kernel_function = kernel;
    this->kernel_derivative_a = derivative_a;
    this->kernel_derivative_w = derivative_w;
  }

  ////////////////////////////////////////////////////////////////////////////////
  void dendrite::Update(){

    // BackPropogate through the kernel to update each weight
    for (unsigned int i = 0; i < this->weights.size(); ++i){
      *this->weights[i] -= this->learning_rate*this->delta*this->KernelDW(i);
    }
    
  }
  
}
