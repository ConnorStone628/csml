
#include "node.hh"

namespace CSML{

  unsigned int node::n_nodes = 0;

  ////////////////////////////////////////////////////////////////////////////////
  node::node(){

    this->source_dendrite = new dendrite(this);
    
    // Holds the total signal comming in from input synapses
    this->input_signal = new double;
    *this->input_signal = 0;

    // Holds the result of activation on the input_signal
    this->output_signal = new double;
    *this->output_signal = 0;

    // Set the functions
    this->activation_function = GM::StandardActivationF;
    this->activation_derivative = GM::StandardActivationD;

    // Each node gets a unique id
    this->nodeid = this->n_nodes;
    this->n_nodes++;
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  node::~node(){

    // Eliminate all newly created variables
    delete this->input_signal;
    delete this->output_signal;
    delete this->source_dendrite;

  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::Fire(){

    // Collect the signal from the previous layer
    *this->input_signal += this->source_dendrite->Kernel();

    // Apply the activation function to the input_signal
    *this->output_signal = this->ActivationF();

  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::BackPropogate(){

    double delta = 0;

    // Backpropogate error through the signal function
    for (unsigned int i = 0; i < this->sink_dendrites.size(); ++i){
      delta += this->sink_dendrites[i]->GetDelta()*this->sink_dendrites[i]->KernelDA(this->nodeid);//sink_indexes[i] fixme deleteme
    }

    // Backpropogate error through the node activation function
    this->source_dendrite->SetDelta(delta*this->ActivationD());

  }

  ////////////////////////////////////////////////////////////////////////////////
  void node::SetActivationFunction(double (*act_func)(double), double (*act_deriv)(double)){

    this->activation_function = act_func;
    this->activation_derivative = act_deriv;
    
  }

}
