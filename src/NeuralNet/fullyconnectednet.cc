
#include "fullyconnectednet.hh"

namespace CSML{
  
  ////////////////////////////////////////////////////////////////////////////////
  fullyconnectednet::fullyconnectednet(const std::vector<unsigned int> nodes_per_layer){

    std::stringstream ss;
    this->learning_rate = 1;

    // Add the nodes
    this->SetShape(nodes_per_layer);

    // Set the first layer to be passive
    for (unsigned int i = 0; i < nodes_per_layer[0]; ++i){
      ss.str("");
      ss << "PL0N" << i;
      this->nodes[0][i]->SetPassive(ss.str());
    }

    // Add synapses
    for (unsigned int i = 1; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++ n){
	// Synapse with the bias node
	this->AddSynapse(i, this->bias_node, this->nodes[i][n]);
	// Add the synapses to the previous layer
	for (unsigned int s = 0; s < this->nodes[i-1].size(); ++s){
	  this->AddSynapse(i, this->nodes[i-1][s], this->nodes[i][n]);
	}
      }
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  fullyconnectednet::~fullyconnectednet(){}

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::TopErrors(const std::vector<double> true_values){

    double delta; // Holds the change in weight during calculation
    int final_layer = this->nodes.size() - 1; // last layer in the net
  
    // Calculate the error at the output nodes
    for (unsigned int n = 0; n < this->nodes[final_layer].size(); ++n){
      // (Learning rate)(derivative of loss function)(derivative of activation function)
      delta = this->learning_rate*this->loss_derivative(true_values[n], *this->nodes[final_layer][n]->output_signal)*(*this->nodes[final_layer][n]->activation_rate);
      for (unsigned int s = 0; s < this->nodes[final_layer][n]->source_synapses.size(); ++s){
	// each synapse records its weight delta which is delta*(activation of the source node for this synapse)
	*this->nodes[final_layer][n]->source_synapses[s]->weight_delta = delta*(*this->nodes[final_layer][n]->source_synapses[s]->source_node->output_signal);
      }
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::BackPropogateErrors(){

    // Backpropogate the error through the net
    for (unsigned int i = this->nodes.size() - 2; i > 0; --i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->BackPropogateError(this->learning_rate);
      }
    }
  
    // Apply the change in weight to each synapse
    for (unsigned int i = 0; i < this->synapses.size(); ++i){
      for (unsigned int a = 0; a < this->synapses[i].size(); ++a){
	*this->synapses[i][a]->weight -= *this->synapses[i][a]->weight_delta;
      }
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::Learn(data* _X){

    std::vector< std::vector<double> >* X = _X->GetFeatures();
    std::vector< std::vector<double> >* y = _X->GetLabelsVV();

    for (unsigned int i = 0; i < this->itterations; ++i){
      for (unsigned int f = 0; f < X->size(); ++f){
	this->Reset("ip");
	this->Propogate((*X)[f]);
	this->TopErrors((*y)[f]);
	this->BackPropogateErrors();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::Predict(data* _X){
    
    std::vector< std::vector<double> >* X = _X->GetFeatures();
    std::vector< std::vector<double> > h(X->size());
    
    for (unsigned int i = 0; i < X->size(); ++i){
	this->Reset("i");
	this->Propogate((*X)[i]);
	h[i] = this->Output();
    }
    
    _X->SetHypothesis(h);

  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::SetParam(const std::string param, const double value){
    if (param == "learningrate" && value > 0){
      this->learning_rate = value;
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::SetParam(const std::string param, const int value){
    if (param == "itterations" && value > 0){
      this->itterations = value;
    }
  }
  
}
